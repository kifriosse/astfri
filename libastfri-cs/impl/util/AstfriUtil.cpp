#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/util/Utils.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace astfri::csharp::util
{

Scope mk_scope(const TSNode& node, const SourceFile& src_file)
{
    std::vector<std::string> scopes;
    Scope scope     = {};
    TSNode nCurrent = node;
    TSNode nParent  = ts_node_parent(nCurrent);

    // todo rewrite this to use TreeCursor
    bool foundNms = false;
    while (! ts_node_is_null(nParent))
    {
        const NodeType type = RegManager::get_node_type(nParent);
        nCurrent            = nParent;
        nParent             = ts_node_parent(nCurrent);

        switch (type)
        {
        case NodeType::ClassDecl:
        case NodeType::InterfaceDecl:
        case NodeType::RecordDecl:
        {
            const TSNode nName = child_by_field_name(nCurrent, "name");
            std::string name   = extract_text(nName, src_file.srcStr);
            scopes.push_back(std::move(name));
            break;
        }
        case NodeType::CompilationUnit:
        {
            if (foundNms || ! src_file.fileContext.fileNms)
                break;

            split_namespace(scopes, *src_file.fileContext.fileNms);
            break;
        }
        case NodeType::NamespaceDecl:
        {
            foundNms               = true;
            const TSNode nName     = child_by_field_name(nCurrent, "name");
            const std::string name = extract_text(nName, src_file.srcStr);
            split_namespace(scopes, name);
            break;
        }
        default:
            break;
        }
    }

    while (! scopes.empty())
    {
        scope.names_.push_back(scopes.back());
        scopes.pop_back();
    }
    return scope;
}

Scope mk_scope(const std::string_view qualifier)
{
    Scope scope;
    auto current     = qualifier.begin();
    auto sliceBegin  = current;
    const auto itEnd = qualifier.end();

    while (current != itEnd)
    {
        if (*current == '.')
        {
            scope.names_.emplace_back(sliceBegin, current);
            sliceBegin = current + 1;
        }
        ++current;
    }

    if (sliceBegin != itEnd)
    {
        scope.names_.emplace_back(sliceBegin, current);
    }

    return scope;
}

ParamVarDefStmt* make_param_def(
    const TSNode& node,
    const std::string_view src,
    TypeTranslator& typeTrs
)
{
    const TSNode nType         = child_by_field_name(node, "type");
    const TSNode nName         = child_by_field_name(node, "name");
    const TypeHandler th       = RegManager::get_type_handler(nType);
    const CSModifiers paramMod = CSModifiers::handle_param_modifs(node, src);
    Type* tParam = paramMod.get_indirection_type(th(&typeTrs, nType));

    return StmtFactory::get_instance()
        .mk_param_var_def(extract_text(nName, src), tParam, nullptr);
}

ParamSignature discover_params(
    const TSNode& node,
    const std::string_view src,
    TypeTranslator& typeTrs
)
{
    std::vector<ParamVarDefStmt*> params;
    std::vector<ParamMetadata> paramsMeta;

    auto collector = [&](const TSNode& current) -> void
    {
        const TSNode nName        = child_by_field_name(current, "name");
        ParamVarDefStmt* paramDef = make_param_def(current, src, typeTrs);
        params.push_back(paramDef);
        paramsMeta
            .emplace_back(paramDef, current, ts_node_next_named_sibling(nName));
    };
    process_param_list(node, collector);
    return {std::move(params), std::move(paramsMeta)};
}

FuncMetadata make_func_metadata(
    const TSNode& node,
    const std::string_view src,
    TypeTranslator& typeTrs
)
{
    static auto& stmtF        = StmtFactory::get_instance();
    const TSNode nName        = child_by_field_name(node, "name");
    const TSNode nRet         = child_by_field_name(node, "type");
    const TSNode nParams      = child_by_field_name(node, "parameters");
    std::string name          = extract_text(nName, src);
    const TypeHandler th      = RegManager::get_type_handler(nRet);
    Type* retType             = th(&typeTrs, nRet);
    auto [params, paramsMeta] = discover_params(nParams, src, typeTrs);
    return FuncMetadata{
        .params  = std::move(paramsMeta),
        .funcDef = stmtF.mk_function_def(
            std::move(name),
            std::move(params),
            retType,
            nullptr
        ),
        .nFunc = node,
    };
}

std::vector<GenericParam*> make_generic_params(
    const TSNode& node,
    const std::string_view src
)
{
    static auto& stmtFact = StmtFactory::get_instance();
    std::vector<GenericParam*> tparams;
    auto processGenericParam = [&](const TSNode& current) -> void
    {
        // todo temporary solution
        const TSNode nName = child_by_field_name(current, "name");
        GenericParam* genParam
            = stmtFact.mk_generic_param({}, extract_text(nName, src));
        tparams.push_back(genParam);
    };
    for_each_child_node(node, processGenericParam);
    return tparams;
}
} // namespace astfri::csharp::util