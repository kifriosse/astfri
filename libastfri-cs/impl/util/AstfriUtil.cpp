#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/util/Utils.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <functional>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>

namespace astfri::csharp::util
{

Scope create_scope(const TSNode& node, const SourceFile& src_file)
{
    enum class NodeType
    {
        Class,
        Interface,
        Record,
        Namespace,
        Root,
    };

    static const std::unordered_map<std::string_view, NodeType> node_types = {
        {"class_declaration",     NodeType::Class    },
        {"struct_declaration",    NodeType::Class    },
        {"record_declaration",    NodeType::Record   },
        {"interface_declaration", NodeType::Interface},
        {"namespace_declaration", NodeType::Namespace},
        {"compilation_unit",      NodeType::Root     },
    };

    std::stack<std::string> scopes;
    Scope scope     = {};
    TSNode nCurrent = node;
    TSNode nParent  = ts_node_parent(nCurrent);

    // todo rewrite this to use TreeCursor
    bool foundNms = false;
    while (! ts_node_is_null(nParent))
    {
        const auto it = node_types.find(ts_node_type(nParent));
        nCurrent      = nParent;
        nParent       = ts_node_parent(nCurrent);

        if (it == node_types.end())
            continue;

        switch (it->second)
        {
        case NodeType::Class:
        case NodeType::Interface:
        case NodeType::Record:
        {
            const TSNode nName = child_by_field_name(nCurrent, "name");
            std::string name   = extract_text(nName, src_file.srcStr);
            scopes.push(std::move(name));
            break;
        }
        case NodeType::Root:
        {
            if (foundNms || ! src_file.fileContext.fileNms)
                break;

            split_namespace(scopes, *src_file.fileContext.fileNms);
            break;
        }
        case NodeType::Namespace:
        {
            foundNms               = true;
            const TSNode nName     = child_by_field_name(nCurrent, "name");
            const std::string name = extract_text(nName, src_file.srcStr);
            split_namespace(scopes, name);
            break;
        }
        }
    }

    while (! scopes.empty())
    {
        scope.names_.push_back(scopes.top());
        scopes.pop();
    }
    return scope;
}

Scope create_scope(const std::string_view qualifier)
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