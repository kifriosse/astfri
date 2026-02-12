#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/visitors/SymbolTableBuilder.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <ranges>
#include <string>
#include <vector>

namespace astfri::csharp
{
struct SourceFile;

StmtFactory& SymbolTableBuilder::stmtFact_    = StmtFactory::get_instance();
regs::QueryReg& SymbolTableBuilder::queryReg_ = regs::QueryReg::get();

SymbolTableBuilder::SymbolTableBuilder(
    std::vector<std::unique_ptr<SourceFile>>& srcs,
    SymbolTable& symbTable
) :
    srcs_(srcs),
    typeTrs_(symbTable),
    symbTable_(symbTable),
    lang_(tree_sitter_c_sharp())
{
}

void SymbolTableBuilder::reg_user_types()
{
    auto process = [this](const TSQueryMatch& match)
    {
        const TSNode node        = match.captures[0].node;
        const RegHandler handler = RegManager::get_reg_handler(node);
        handler(this, node);
    };

    for (auto& src : srcs_)
    {
        auto& [context, file, tree] = *src;
        if (! tree)
            continue;

        currentSrc_ = src.get();
        typeTrs_.set_current_src(src.get());
        util::for_each_match(
            ts_tree_root_node(tree),
            regs::QueryType::TopLevel,
            process
        );
    }
    currentSrc_ = nullptr;
    typeTrs_.set_current_src(nullptr);
}

void SymbolTableBuilder::reg_using_directives()
{
    auto process = [this](const TSQueryMatch& match) -> void
    { reg_using_directive(match.captures[0].node); };

    for (auto& src : srcs_)
    {
        typeTrs_.set_current_src(src.get());
        currentSrc_       = src.get();
        const TSNode root = ts_tree_root_node(src->tree);
        util::for_each_match(root, regs::QueryType::Using, process);
    }
    currentSrc_ = nullptr;
    typeTrs_.set_current_src(nullptr);
}

void SymbolTableBuilder::reg_members()
{
    auto process = [this](const TSNode& current)
    {
        if (util::is_type_decl(current))
            return;

        const RegHandler handler = RegManager::get_reg_handler(current);
        handler(this, current);
    };
    for (auto& metadata : symbTable_.userTypeMetadata | std::views::values)
    {
        typeTrs_.set_current_namespace(metadata.scope);
        for (auto& [node, src] : metadata.defs)
        {
            const TSNode nClassBody = util::child_by_field_name(node, "body");
            currentSrc_             = src;
            typeTrs_.set_current_src(src);
            typeContext_.typeStack.push(metadata.userType);
            util::for_each_child_node(nClassBody, process);
            typeContext_.typeStack.pop();
        }
    }
    typeTrs_.set_current_namespace(nullptr);
    typeTrs_.set_current_src(nullptr);
    currentSrc_ = nullptr;
}

void SymbolTableBuilder::visit_class(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    self->register_type(node, util::TypeKind::Class);
}

void SymbolTableBuilder::visit_interface(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
    self->register_type(node, util::TypeKind::Interface);
}

void SymbolTableBuilder::visit_record(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
    self->register_type(node, util::TypeKind::Record);
}

void SymbolTableBuilder::visit_enum(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
    self->register_type(node, util::TypeKind::Enum);
}

void SymbolTableBuilder::visit_delegate(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
    self->register_type(node, util::TypeKind::Delegate);
}

void SymbolTableBuilder::visit_memb_var(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    const std::string_view src = self->src_str();

    TSNode nVarDecl;
    const CSModifiers modifs
        = CSModifiers::handle_var_modifs(node, src, &nVarDecl);
    const TSNode nType     = util::child_by_field_name(nVarDecl, "type");
    const TypeHandler th   = RegManager::get_type_handler(nType);
    Type* type             = th(&self->typeTrs_, nType);
    TypeMetadata& typeMeta = self->symbTable_.userTypeMetadata.at(
        self->typeContext_.typeStack.top()
    );

    std::vector<VarDefStmt*> varDefs;
    auto process = [&](const TSQueryMatch& match)
    {
        const TSNode nDecltor    = match.captures[0].node;
        const TSNode nName       = util::child_by_field_name(nDecltor, "name");
        std::string name         = util::extract_text(nName, src);
        MemberVarDefStmt* varDef = stmtFact_.mk_member_var_def(
            std::move(name),
            type,
            nullptr,
            modifs.get_access_mod().value_or(AccessModifier::Private)
        );

        varDefs.push_back(varDef);
        MemberVarMetadata membVarMeta{
            .varDef = varDef,
            .nVar   = nDecltor,
            .nInit  = ts_node_named_child(nDecltor, 1) // right side
        };
        typeMeta.memberVars.emplace(varDef->name_, membVarMeta);
    };
    util::for_each_match(nVarDecl, regs::QueryType::VarDecltor, process);
}

void SymbolTableBuilder::visit_property(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_method(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    const auto currentType = self->typeContext_.typeStack.top();
    if (! currentType)
        throw std::logic_error("Owner type not found");

    auto& userTypeMetadata = self->symbTable_.userTypeMetadata;
    const auto itTypeMeta  = userTypeMetadata.find(currentType);
    if (itTypeMeta == userTypeMetadata.end())
        throw std::logic_error("Type wasn't discovered yet");

    const CSModifiers modifs
        = CSModifiers::handle_memb_modifs(node, self->src_str());

    const TSNode nRetType       = util::child_by_field_name(node, "returns");
    const TSNode nFuncName      = util::child_by_field_name(node, "name");
    const TSNode nParams        = util::child_by_field_name(node, "parameters");

    const bool isVariadic       = util::has_variadic_param(nParams);
    const size_t cNamedChildren = ts_node_named_child_count(nParams);
    const size_t cParam = isVariadic ? cNamedChildren - 1 : cNamedChildren;
    std::string name    = util::extract_text(nFuncName, self->src_str());
    MethodId methodId{
        .name       = name,
        .paramCount = cParam,
        .isStatic   = modifs.has(CSModifier::Static),
    };

    auto [params, paramsMeta]
        = util::discover_params(nParams, self->src_str(), self->typeTrs_);
    MethodDefStmt* methodDef         = nullptr;
    auto& methods                    = itTypeMeta->second.methods;
    const auto& [itMethod, inserted] = methods.try_emplace(std::move(methodId));

    if (inserted)
    {
        const TypeHandler th = RegManager::get_type_handler(nRetType);
        Type* retType        = th(&self->typeTrs_, nRetType);
        methodDef            = stmtFact_.mk_method_def(
            currentType,
            stmtFact_.mk_function_def(
                std::move(name),
                std::move(params),
                retType,
                nullptr
            ),
            modifs.get_access_mod().value_or(AccessModifier::Internal),
            modifs.get_virtuality()
        );
    }

    MethodMetadata metadata{
        .params    = std::move(paramsMeta),
        .methodDef = methodDef,
        .nMethod   = node,
    };
    itMethod->second = std::move(metadata);
}

void SymbolTableBuilder::reg_using_directive(const TSNode& nUsingDirective)
{
    SourceFile* src               = this->src();
    const std::string_view srcStr = src->srcStr;
    static const TSSymbol sGlobal = util::symbol_for_name("global", false);
    static const TSSymbol sStatic = util::symbol_for_name("static", false);
    const TSNode nAliasName
        = util::child_by_field_name(nUsingDirective, "name");
    bool isGlobal = false;
    bool isStatic = false;

    auto process  = [&isGlobal, &isStatic](const TSNode& current)
    {
        const TSSymbol sCurrent = ts_node_symbol(current);
        if (sCurrent == sGlobal)
            isGlobal = true;
        else if (sCurrent == sStatic)
            isStatic = true;
    };

    util::for_each_child_node(nUsingDirective, process, false);

    const Scope directiveScope = util::mk_scope(nUsingDirective, *src);
    ScopeNode* searchStart     = symbTable_.symbTree.find_node(directiveScope);

    if (ts_node_is_null(nAliasName))
    {
        using enum util::SearchScope;
        util::SearchScope searchScope;
        if (isStatic)
            searchScope = isGlobal ? GlobStaticUsing : LocalStaticUsing;
        else
            searchScope = isGlobal ? GlobUsing : FileUsing;

        const TSNode nQualif = ts_node_named_child(nUsingDirective, 0);
        ScopeNode* node
            = typeTrs_.resolve_qualif_name(nQualif, searchScope, searchStart);

        if (! node)
            return;

        if (isStatic)
        {
            if (const auto* b = node->has_data<TypeBinding>())
            {
                if (isGlobal)
                    symbTable_.globStaticUsings.push_back(*b);
                else
                    searchStart->data<Nms>().add_static_using(src, *b);
            }
        }
        else
        {
            if (isGlobal)
                symbTable_.globUsings.push_back(node);
            else
                src->fileContext.usings.push_back(node);
        }
    }
    else
    {
        using enum util::SearchScope;
        const auto seachScope = isGlobal ? GlobAlias : LocalAlias;
        const TSNode nQualif  = ts_node_next_named_sibling(nAliasName);
        ScopeNode* node
            = typeTrs_.resolve_qualif_name(nQualif, seachScope, searchStart);
        if (node)
        {
            auto* ext   = node->has_data<ExternalMarker>();
            Alias alias = ext ? Alias{std::move(ext->fqn)} : Alias{node};
            std::string aliasName = util::extract_text(nAliasName, srcStr);

            if (isGlobal)
            {
                symbTable_.globAliases.emplace(
                    std::move(aliasName),
                    std::move(alias)
                );
            }
            else
            {
                Nms& nms = searchStart->data<Nms>();
                nms.add_alias(std::move(aliasName), src, std::move(alias));
            }
        }
    }
}

void SymbolTableBuilder::register_type(
    const TSNode& node,
    const util::TypeKind typeKind
)
{
    const TSNode nName   = util::child_by_field_name(node, "name");
    std::string name     = util::extract_text(nName, src_str());
    Scope scope          = util::mk_scope(node, *src());
    UserTypeDefStmt* def = nullptr;
    ScopedType* type     = nullptr;

    switch (typeKind)
    {
    case util::TypeKind::Class:
    {
        ClassDefStmt* classDef
            = stmtFact_.mk_class_def(std::move(name), std::move(scope));
        def  = classDef;
        type = classDef->type_;
        break;
    }
    case util::TypeKind::Interface:
    {
        InterfaceDefStmt* intfDef
            = stmtFact_.mk_interface_def(std::move(name), std::move(scope));
        def  = intfDef;
        type = intfDef->m_type;
        break;
    }
    case util::TypeKind::Record:
    case util::TypeKind::Enum:
    case util::TypeKind::Delegate:
        // todo add registration for other types
        break;
    }

    if (def && type)
    {
        TypeMetadata metadata{
            .userType = def,
            .scope    = symbTable_.symbTree.add_type(type->scope_, type, def)
        };
        auto [it, inserted]
            = symbTable_.userTypeMetadata.try_emplace(def, std::move(metadata));

        if (inserted)
            symbTable_.userTypeKeys.push_back(def);
        it->second.defs.emplace_back(node, this->src());
    }
}

SourceFile* SymbolTableBuilder::src() const
{
    return currentSrc_
             ? currentSrc_
             : throw std::logic_error("Current source code is not set");
}

std::string_view SymbolTableBuilder::src_str() const
{
    return src()->srcStr;
}

} // namespace astfri::csharp
