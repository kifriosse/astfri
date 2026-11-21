#include <astfri/Astfri.hpp>

#include <astfri-cs/impl/data/CSModifiers.hpp>
#include <astfri-cs/impl/dispatchers/SymbTableBuilder.hpp>
#include <astfri-cs/impl/registries/Maps.hpp>
#include <astfri-cs/impl/symbols/SymbolTable.hpp>
#include <astfri-cs/impl/util/AstfriUtil.hpp>
#include <astfri-cs/impl/util/RapidJsonUtil.hpp>
#include <astfri-cs/impl/util/TSUtil.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <rapidjson/document.h>

#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace astfri::csharp {

namespace {

constexpr std::string_view netSdk[]{
    "System",
    "System.Collections.Generic",
    "System.IO",
    "System.Linq",
    "System.Net.Http",
    "System.Threading",
    "System.Threading.Tasks"
};

constexpr std::string_view webSdk[]{
    "System.Net.Http.Json",
    "Microsoft.AspNetCore.Builder",
    "Microsoft.AspNetCore.Hosting",
    "Microsoft.AspNetCore.Http",
    "Microsoft.AspNetCore.Routing",
    "Microsoft.Extensions.Configuration",
    "Microsoft.Extensions.DependencyInjection",
    "Microsoft.Extensions.Hosting",
    "Microsoft.Extensions.Logging"
};

constexpr std::string_view workerSdk[]{
    "Microsoft.Extensions.Configuration",
    "Microsoft.Extensions.DependencyInjection",
    "Microsoft.Extensions.Hosting",
    "Microsoft.Extensions.Logging"
};

constexpr std::string_view winFormsSdk[]{"System.Drawing", "System.Windows.Forms"};

constexpr std::string_view wpfSdkExclude[]{"System.IO", "System.Net.Http"};

} // namespace

struct SourceFile;

StmtFactory& SymbTableBuilder::stmtFact_        = StmtFactory::get_instance();
maps::MapManager& SymbTableBuilder::mapManager_ = maps::MapManager::get();
maps::QueryReg& SymbTableBuilder::queryReg_     = maps::QueryReg::get();

SymbTableBuilder::SymbTableBuilder(
    std::vector<std::unique_ptr<SourceFile>>& srcs,
    SymbolTable& symbTable
) :
    typeTrs_(symbTable),
    symbTable_(symbTable),
    srcs_(srcs) {
}

void SymbTableBuilder::reg_user_types() {
    for (auto& src : srcs_) {
        if (! src->tree)
            continue;

        currentSrc_ = src.get();
        typeTrs_.set_current_src(src.get());
        collect_types(src->tree);
    }
    currentSrc_ = nullptr;
    typeTrs_.set_current_src(nullptr);
}

void SymbTableBuilder::reg_using_directives() {
    auto process = [this](const TSQueryMatch& match) -> void {
        reg_using_directive(match.captures[0].node);
    };

    for (auto& src : srcs_) {
        typeTrs_.set_current_src(src.get());
        currentSrc_       = src.get();
        const TSNode root = ts_tree_root_node(src->tree);
        util::for_each_match(root, maps::QueryType::Using, process);
    }
    currentSrc_ = nullptr;
    typeTrs_.set_current_src(nullptr);
}

void SymbTableBuilder::reg_members() {
    auto process = [this](const TSNode& current) {
        if (util::is_type_decl(current))
            return;

        const MemberCollector collector = mapManager_.get_symb_collector(current);
        collector(this, current);
    };

    for (auto* metadata : symbTable_.get_type_metadata()) {
        typeTrs_.set_current_namespace(metadata->tb().treeNode);
        for (auto& [node, src] : metadata->defs()) {
            currentSrc_ = src;
            typeTrs_.set_current_src(src);
            typeContext_ = metadata->tb();
            // processing header - base list and generic param constraints
            util::for_each_child_node(node, process);
            const TSNode nClassBody = util::child_by_field_name(node, "body");
            util::for_each_child_node(nClassBody, process);
            typeContext_ = std::nullopt;
        }
    }
    typeTrs_.set_current_namespace(nullptr);
    typeTrs_.set_current_src(nullptr);
    currentSrc_ = nullptr;
}

ScopeNode* SymbTableBuilder::visit_class(SymbTableBuilder* self, const TSNode& node) {
    return self->visit_type_def(node, util::TypeKind::Class);
}

ScopeNode* SymbTableBuilder::visit_interface(SymbTableBuilder* self, const TSNode& node) {
    return self->visit_type_def(node, util::TypeKind::Interface);
}

ScopeNode* SymbTableBuilder::visit_record(SymbTableBuilder* self, const TSNode& node) {
    return self->visit_type_def(node, util::TypeKind::Record);
}

ScopeNode* SymbTableBuilder::visit_enum(SymbTableBuilder* self, const TSNode& node) {
    return self->visit_type_def(node, util::TypeKind::Enum);
}

ScopeNode* SymbTableBuilder::visit_delegate(SymbTableBuilder* self, const TSNode& node) {
    return self->visit_type_def(node, util::TypeKind::Delegate);
}

void SymbTableBuilder::visit_memb_var(SymbTableBuilder* self, const TSNode& node) {
    const std::string_view src = self->src_str();

    TSNode nVarDecl;
    const auto modifs      = CSModifiers::parse_var_modifs(node, src, &nVarDecl);
    const TSNode nType     = util::child_by_field_name(nVarDecl, "type");
    const TypeMapper tm    = mapManager_.get_type_mapper(nType);
    Type* type             = tm(&self->typeTrs_, nType);
    TypeMetadata* typeMeta = self->symbTable_.get_type_metadata(self->typeContext_->type);

    if (! typeMeta)
        return;

    std::vector<VarDefStmt*> varDefs;
    auto process = [&](const TSQueryMatch& match) {
        const TSNode nDecltor    = match.captures[0].node;
        const TSNode nName       = util::child_by_field_name(nDecltor, "name");
        std::string name         = util::extract_text(nName, src);
        MemberVarDefStmt* varDef = stmtFact_.mk_member_var_def(
            std::move(name),
            type,
            nullptr,
            modifs.get_access_mod().value_or(AccessModifier::Private),
            modifs.is_static()
        );

        varDefs.push_back(varDef);
        const MemberVarMetadata membVarMeta{
            .varDef = varDef,
            .nVar   = nDecltor,
            .nInit  = ts_node_named_child(nDecltor, 1) // right side
        };
        typeMeta->add_memb_var(varDef->name, membVarMeta);
    };
    util::for_each_match(nVarDecl, maps::QueryType::VarDecltor, process);
}

void SymbTableBuilder::visit_property(
    [[maybe_unused]] SymbTableBuilder* self,
    [[maybe_unused]] const TSNode& node
) {
}

void SymbTableBuilder::visit_method(SymbTableBuilder* self, const TSNode& node) {
    const auto typeMeta = self->symbTable_.get_type_metadata(self->typeContext_->type);
    if (! typeMeta)
        return;
    // throw std::logic_error("Type wasn't discovered yet");

    const std::string_view srcStr = self->src_str();
    const CSModifiers modifs      = CSModifiers::parser_method_modifs(node, srcStr);

    const TSNode nRetType         = util::child_by_field_name(node, "returns");
    const TSNode nFuncName        = util::child_by_field_name(node, "name");
    const TSNode nParams          = util::child_by_field_name(node, "parameters");

    const bool isVariadic         = util::has_variadic_param(nParams);
    const size_t cNamedChildren   = ts_node_named_child_count(nParams);
    const size_t cParam           = isVariadic ? cNamedChildren - 1 : cNamedChildren;
    std::string name              = util::extract_text(nFuncName, srcStr);
    MethodId methodId{
        .name       = name,
        .paramCount = cParam,
        .isStatic   = modifs.has(CSModifier::Static),
    };

    auto [params, paramsMeta] = util::discover_params(nParams, srcStr, self->typeTrs_);
    const TypeMapper tm       = mapManager_.get_type_mapper(nRetType);
    Stmt* def                 = nullptr;

    if (const auto* classType = as<ClassType>(self->typeContext_->type))
        def = classType->def;
    else if (const auto* intfType = as<InterfaceType>(self->typeContext_->type))
        def = intfType->def;

    if (! def)
        return;

    MethodDefStmt* methodDef = stmtFact_.mk_method_def(
        def,
        stmtFact_.mk_function_def(
            std::move(name),
            std::move(params),
            tm(&self->typeTrs_, nRetType),
            nullptr
        ),
        modifs.get_access_mod().value_or(AccessModifier::Internal),
        modifs.get_virtuality(),
        modifs.is_static()
    );

    MethodMetadata methodMetadata{
        .params    = std::move(paramsMeta),
        .methodDef = methodDef,
        .nMethod   = node
    };
    typeMeta->add_method(std::move(methodId), std::move(methodMetadata));
}

void SymbTableBuilder::visit_base_list(SymbTableBuilder* self, const TSNode& node) {
    ScopedType* type = self->typeContext_->type;
    if (const auto classType = as<ClassType>(type))
        self->visit_base_list_class(node, classType);
    else if (const auto intfType = as<InterfaceType>(type))
        self->visit_base_list_interface(node, intfType);
}

void SymbTableBuilder::visit_type_param_constraint(
    [[maybe_unused]] SymbTableBuilder* self,
    [[maybe_unused]] const TSNode& node
) {
    // todo implement generic constraints
}

void SymbTableBuilder::load_implicit_usings(const SDKProfile profile) {
    using enum SDKProfile;
    if (profile == None)
        return;

    auto addToScope = [this](const std::string_view qualif) {
        ScopeNode* entryPoint = typeTrs_.find_entry_point(
            qualif,
            util::SearchScope::GlobUsing,
            symbTable_.symb_tree().root(),
            nullptr
        );
        if (entryPoint)
            symbTable_.add_glob_using(entryPoint);
    };

    for (auto qualif : netSdk) {
        if (profile == WPF && std::ranges::find(wpfSdkExclude, qualif) != std::end(wpfSdkExclude))
            continue;

        addToScope(qualif);
    }

    if (profile == Core || profile == WPF)
        return;

    std::span<const std::string_view> usings{};
    switch (profile) {
    case Web:
        usings = webSdk;
        break;
    case Worker:
        usings = workerSdk;
        break;
    case WinForms:
        usings = winFormsSdk;
    default:
        break;
    }

    for (const auto& qualif : usings) {
        addToScope(qualif);
    }
}

void SymbTableBuilder::load_external_types(std::filesystem::path& jsonPath) {
    using namespace rapidjson;
    if (! exists(jsonPath)) {
        std::cerr << "File '" << jsonPath << "' wasn't found " << std::endl;
        return;
    }

    std::ifstream fileStream(jsonPath, std::ios::binary);
    std::string jsonContent(
        (std::istreambuf_iterator(fileStream)),
        std::istreambuf_iterator<char>()
    );
    Document d;
    d.Parse(jsonContent.data());
    if (! d.IsArray())
        return;

    for (SizeType i = 0; i < d.Size(); ++i) {
        const Value& item = d[i];
        if (! item.IsObject())
            continue;

        if (util::has_all(item, "Name", "Nms", "Arity", "Type")) {
            const Value& vName  = item["Name"];
            const Value& vNms   = item["Nms"];
            const Value& vArity = item["Arity"];
            const Value& vType  = item["Type"];
            if (! vName.IsString() || ! vNms.IsString() || ! vArity.IsInt() || ! vType.IsString()) {
                continue;
            }
            std::string name = vName.GetString();
            Scope scope      = util::mk_scope(vNms.GetString());
            // int arity        = vArity.GetInt();
            auto typeOpt = util::get_type_kind(vType.GetString());

            if (! typeOpt)
                continue;

            if (*typeOpt == util::TypeKind::Primitive) {
                Type* type = mapManager_.get_primitive_type(name);
                symbTable_.add_primitive(name, CSPrimitiveType{type});
                continue;
            }

            // todo generic parameters for external types
            TypeBinding tb = mk_type_binding(*typeOpt, scope, name, {});
            if (tb.type)
                symbTable_.add_type(tb);
        }
        else
            throw std::logic_error("Invalid JSON");
    }
}

void SymbTableBuilder::reg_using_directive(const TSNode& nUsingDirective) {
    SourceFile* src               = this->src();
    const std::string_view srcStr = src->srcStr;
    const TSNode nAliasName       = util::child_by_field_name(nUsingDirective, "name");
    bool isGlobal                 = false;
    bool isStatic                 = false;

    auto process                  = [&isGlobal, &isStatic](const TSNode& current) {
        switch (mapManager_.get_node_type(current)) {
        case NodeType::Static:
            isStatic = true;
            break;
        case NodeType::Global:
            isGlobal = true;
            break;
        default:
            break;
        }
    };

    util::for_each_child_node(nUsingDirective, process, false);

    const Scope directiveScope = util::mk_scope(nUsingDirective, *src);
    ScopeNode* searchStart     = symbTable_.symb_tree().find_node(directiveScope);

    using enum util::SearchScope;
    if (ts_node_is_null(nAliasName)) {
        util::SearchScope searchScope;
        if (isStatic)
            searchScope = isGlobal ? GlobStaticUsing : LocalStaticUsing;
        else
            searchScope = isGlobal ? GlobUsing : FileUsing;

        const TSNode nQualif = ts_node_named_child(nUsingDirective, 0);
        ScopeNode* node      = typeTrs_.resolve_qualif_name(nQualif, searchScope, searchStart);

        if (! node)
            return;

        if (isStatic) {
            if (const auto* b = node->is_a<TypeBinding>()) {
                if (isGlobal)
                    symbTable_.add_glob_static_using(*b);
                else
                    searchStart->data<Nms>().add_static_using(src, *b);
            }
        }
        else {
            if (isGlobal)
                symbTable_.add_glob_using(node);
            else
                src->usings.push_back(node);
        }
    }
    else {
        const auto seachScope = isGlobal ? GlobAlias : LocalAlias;
        const TSNode nQualif  = ts_node_next_named_sibling(nAliasName);
        if (ScopeNode* node = typeTrs_.resolve_qualif_name(nQualif, seachScope, searchStart)) {
            auto* ext             = node->is_a<ExternalMarker>();
            Alias alias           = ext ? Alias{ext->take_name()} : Alias{node};
            std::string aliasName = util::extract_text(nAliasName, srcStr);

            if (isGlobal) {
                symbTable_.add_glob_alias(std::move(aliasName), std::move(alias));
            }
            else {
                Nms& nms = searchStart->data<Nms>();
                nms.add_alias(std::move(aliasName), src, std::move(alias));
            }
        }
    }
}

void SymbTableBuilder::collect_types(const TSTree* tree) {
    struct State {
        TSNode nBody;
        ScopeNode* parentNode;
    };

    std::vector<State> stack;
    ScopeNode* const root = symbTable_.symb_tree().root();
    stack.emplace_back(ts_tree_root_node(tree), root);

    auto process = [this, &stack](const TSNode& node) {
        TSNode body = util::child_by_field_name(node, "body");
        if (ts_node_is_null(body))
            return;

        if (util::is_type_decl(node)) {
            const TypeCollector collector = mapManager_.get_type_collector(node);
            if (ScopeNode* scopeNode = collector(this, node))
                stack.emplace_back(body, scopeNode);
        }
        else if (mapManager_.is_a(node, NodeType::NamespaceDecl)) {
            const TSNode nName = util::child_by_field_name(node, "name");
            std::string name   = util::extract_text(nName, src_str());
            Nms nms(name);
            if (ScopeNode* scopeNode = currentParent_->add_child(std::move(name), std::move(nms)))
                stack.emplace_back(body, scopeNode);
        }
    };

    while (! stack.empty()) {
        const auto& [nBody, parent] = stack.back();
        currentParent_              = parent;
        stack.pop_back();
        util::for_each_child_node(nBody, process, true);
    }
}

ScopeNode* SymbTableBuilder::visit_type_def(const TSNode& node, const util::TypeKind type) {
    const TSNode nName     = util::child_by_field_name(node, "name");
    const std::string name = util::extract_text(nName, src_str());

    std::vector<GenericParam*> genParams;

    auto processClassHeader = [&](const TSNode& current) -> void {
        if (mapManager_.is_a(current, NodeType::TypeParamList)) {
            genParams = util::mk_generic_params(current, src_str());
        }
    };
    util::for_each_child_node(node, processClassHeader);

    const Scope scope    = util::mk_scope(currentParent_, *currentSrc_);
    const TypeBinding tb = mk_type_binding(type, scope, name, std::move(genParams));

    return tb.type ? symbTable_.add_type(tb, node, src()) : nullptr;
}

void SymbTableBuilder::visit_base_list_class(const TSNode& node, ClassType* classType) {
    bool first           = true;
    auto processBaseList = [&](const TSNode& current) -> void {
        const std::string name = util::extract_text(current, src_str());
        const TypeMapper tm    = mapManager_.get_type_mapper(current);
        Type* type             = tm(&typeTrs_, current);
        ClassDefStmt* def      = classType->def;

        if (first) {
            first = false;
            if (const auto tClass = as<ClassType>(type)) {
                def->baseTypes.push_back(tClass);
                return;
            }
        }

        if (const auto tInterface = as<InterfaceType>(type)) {
            def->interfaceTypes.push_back(tInterface);
        }
        else if (util::is_interface_name(name)) {
            def->interfaceTypes.push_back(stmtFact_.mk_interface_def(name, {})->type);
            // todo might be useless if external types are imported
        }
        else {
            TypeFactory& typeFact = TypeFactory::get_instance();
            def->incompleteBases.push_back(typeFact.mk_incomplete(name));
        }
    };

    util::for_each_child_node(node, processBaseList);
}

void SymbTableBuilder::visit_base_list_interface(const TSNode& node, InterfaceType* intfType) {
    auto processBaseList = [&](const TSNode& current) -> void {
        const TypeMapper tm = mapManager_.get_type_mapper(current);
        Type* type          = tm(&typeTrs_, current);

        if (const auto tInterface = as<InterfaceType>(type))
            intfType->def->baseTypes.push_back(tInterface);
        else {
            // todo incomplete type
        }
    };

    util::for_each_child_node(node, processBaseList);
}

std::string_view SymbTableBuilder::src_str() const {
    return src()->srcStr;
}

SourceFile* SymbTableBuilder::src() const {
    return currentSrc_ ? currentSrc_ : throw std::logic_error("Current source code is not set");
}

TypeBinding SymbTableBuilder::mk_type_binding(
    const util::TypeKind type,
    const Scope& scope,
    const std::string& name,
    std::vector<GenericParam*> genericParams
) {
    TypeBinding tb{.treeNode = nullptr, .type = nullptr};
    switch (type) {
    case util::TypeKind::Class: {
        ClassDefStmt* classDef = stmtFact_.mk_class_def(name, scope);
        classDef->tparams      = std::move(genericParams);
        tb.type                = classDef->type;
        break;
    }
    case util::TypeKind::Interface: {
        InterfaceDefStmt* intDef = stmtFact_.mk_interface_def(name, scope);
        intDef->tparams          = std::move(genericParams);
        tb.type                  = intDef->type;
        break;
    }
    case util::TypeKind::Record:
    case util::TypeKind::Delegate:
    case util::TypeKind::Enum:
    case util::TypeKind::Primitive:
        break;
    }

    return tb;
}

} // namespace astfri::csharp
