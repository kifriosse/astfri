#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/SymbolTableBuilder.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <ranges>
#include <string>
#include <vector>

namespace astfri::csharp
{
struct SourceCode;

StmtFactory& SymbolTableBuilder::stmtFact_       = StmtFactory::get_instance();
regs::QueryReg& SymbolTableBuilder::queryReg_ = regs::QueryReg::get();

SymbolTableBuilder::SymbolTableBuilder(
    std::vector<SourceCode>& srcs,
    SymbolTable& symbTable
) :
    typeTrs_(symbTable),
    srcs_(srcs),
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
        auto& [context, file, tree] = src;
        if (! tree)
            continue;

        currentSrc_ = &src;
        typeTrs_.set_current_src(&src);
        util::for_each_match(
            ts_tree_root_node(tree),
            regs::QueryType::TopLevel,
            process
        );
    }
    currentSrc_ = nullptr;
}

void SymbolTableBuilder::reg_using_directives()
{
    auto process = [this](const TSQueryMatch match) -> void
    { add_using_directive(match.captures[0].node); };
    for (auto& src : srcs_)
    {
        typeTrs_.set_current_src(&src);
        currentSrc_       = &src;
        const TSNode root = ts_tree_root_node(src.tree);
        util::for_each_match(root, regs::QueryType::Using, process);
    }
}

void SymbolTableBuilder::reg_members()
{
    for (auto& metadata : symbTable_.userTypeMetadata | std::views::values)
    {
        for (auto& [node, src] : metadata.defs)
        {
            currentSrc_ = src;
            typeTrs_.set_current_src(src);
            const TSNode nClassBody = util::child_by_field_name(node, "body");
            TSTreeCursor cursor     = ts_tree_cursor_new(nClassBody);
            typeContext_.typeStack.push(metadata.userType);

            if (! ts_tree_cursor_goto_first_child(&cursor))
                continue;

            do
            {
                TSNode nCurrent = ts_tree_cursor_current_node(&cursor);
                if (! ts_node_is_named(nCurrent))
                    continue;

                RegHandler handler = RegManager::get_reg_handler(nCurrent);
                handler(this, nCurrent);
            } while (ts_tree_cursor_goto_next_sibling(&cursor));

            typeContext_.typeStack.pop();
            ts_tree_cursor_delete(&cursor);
        }
    }
}

void SymbolTableBuilder::visit_class(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    const std::string_view src = self->src_str();
    const TSNode nName         = util::child_by_field_name(node, "name");
    // todo handle generic parameters
    Scope scope = util::create_scope(node, src);
    ClassDefStmt* classDef
        = stmtFact_.mk_class_def(util::extract_text(nName, src), std::move(scope));

    self->symbTable_.symbTree.add_type(scope, classDef->type_, classDef);

    auto [it, inserted] = self->symbTable_.userTypeMetadata.try_emplace(
        classDef,
        TypeMetadata{.userType = classDef}
    );
    if (inserted)
    {
        self->symbTable_.userTypeKeys.push_back(classDef);
    }
    it->second.defs.emplace_back(node, self->currentSrc_);
}

void SymbolTableBuilder::visit_interface(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_record(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_enum(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_delegate(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_memb_var(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    const std::string_view src = self->src_str();

    TSNode nVarDecl;
    const CSModifiers modifs
        = CSModifiers::handle_modifs_var(node, src, &nVarDecl);
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
        MemberVarDefStmt* varDef = stmtFact_.mk_member_var_def(
            util::extract_text(nName, src),
            type,
            nullptr,
            modifs.get_access_mod().value_or(AccessModifier::Private)
        );

        varDefs.push_back(varDef);
        MemberVarMetadata membVarMeta{
            .varDef = varDef,
            .nVar   = nDecltor,
            .nInit   = ts_node_named_child(nDecltor, 1) // right side
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
        = CSModifiers::handle_modifs_memb(node, self->src_str());

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

void SymbolTableBuilder::add_using_directive(const TSNode& node)
{
    // const static TSSymbol qualified_name_symb
    //     = util::symbol_for_name(lang, "qualified_name", true);
    // const static TSSymbol generic_name_symb
    //     = util::symbol_for_name(lang, "generic_name", true);
    static const TSSymbol sGlobal = util::symbol_for_name("global", false);
    static const TSSymbol sStatic = util::symbol_for_name("static", false);

    const TSNode nName            = util::child_by_field_name(node, "name");
    // util::print_child_nodes_types(node, src_code, true);

    if (ts_node_is_null(nName))
    {
        const TSNode nQualif = ts_node_named_child(node, 0);
        // std::cout << ts_node_type(qualifier_node) << std::endl;
        // util::print_child_nodes_types(qualifier_node, src_code);
        const std::string nmsStr = util::extract_text(nQualif, src_str());
        Scope scope              = util::create_scope(nmsStr);
        bool isGlobal            = false;
        bool isStatic            = false;
        for (CaptureId i = 0; i < ts_node_child_count(node); ++i)
        {
            const TSSymbol sCurrent = ts_node_symbol(ts_node_child(node, i));
            if (sCurrent == sGlobal)
                isGlobal = true;
            else if (sCurrent == sStatic)
                isStatic = true;
        }

        if (isGlobal && isStatic)
        {
            const std::string typeName = std::move(scope.names_.back());
            scope.names_.pop_back();
            if (const auto type
                = symbTable_.symbTree.find_type(scope, typeName))
            {
                symbTable_.globStaticUsings.push_back(type->def);
            }
        }
        else if (isStatic)
        {
            const std::string typeName = std::move(scope.names_.back());
            scope.names_.pop_back();
            if (const auto type
                = symbTable_.symbTree.find_type(scope, typeName))
            {
                src()->fileContext.staticUsings.push_back(type->def);
            }
        }
        else if (isGlobal)
        {
            symbTable_.globUsings.push_back(std::move(scope));
        }
        else
        {
            src()->fileContext.usings.push_back(std::move(scope));
        }
    }
    else
    {
        // todo aliases not implemented
        // const TSNode qualif_name_node = ts_node_named_child(node, 1);
        // std::cout << ts_node_type(qualif_name_node) << std::endl;
        // const std::string alias_name = util::extract_node_text(name_node,
        // src_code); if (ts_node_symbol(qualif_name_node) ==
        // qualified_name_symb)
        // {
        //     Scope scope =
        //     mk_scope(util::extract_node_text(qualif_name_node));
        // } else
        // {
        //
        // }

        // std::stack<std::string> scope_stack;
        // util::print_child_nodes_types(qualif_name_node, src_code);
        // util::split_namespace(scope_stack, qualifier);

        // if (ClassDefStmt* class_def = stmt_factory_.get_class_def(, ))
    }
}

SourceCode* SymbolTableBuilder::src() const
{
    return currentSrc_
             ? currentSrc_
             : throw std::logic_error("Current source code is not set");
}

std::string_view SymbolTableBuilder::src_str() const
{
    return src()->file.content;
}

} // namespace astfri::csharp
