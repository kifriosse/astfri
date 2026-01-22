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

Scope create_scope(const TSNode& node, const std::string_view src)
{
    enum NodeType
    {
        Class,
        Interface,
        Namespace,
        Root,
    };

    // todo add other type like struct and record
    static const std::unordered_map<std::string_view, NodeType> node_types = {
        {"class_declaration",     Class    },
        {"struct_declaration",    Class    },
        {"interface_declaration", Interface},
        {"namespace_declaration", Namespace},
        {"compilation_unit",      Root     },
    };

    std::stack<std::string> scopes;
    Scope scope     = {};
    TSNode nCurrent = node;
    TSNode nParent  = ts_node_parent(nCurrent);

    bool foundNms   = false;
    while (! ts_node_is_null(nParent))
    {
        const auto it = node_types.find(ts_node_type(nParent));
        nCurrent      = nParent;
        nParent       = ts_node_parent(nCurrent);

        if (it == node_types.end())
            continue;

        switch (it->second)
        {
        case Class:
        case Interface:
        {
            const TSNode n_name = child_by_field_name(nCurrent, "name");
            std::string name    = extract_text(n_name, src);
            scopes.push(std::move(name));
            break;
        }
        case Root:
        {
            if (foundNms)
                break;

            TSNode nNms{};
            auto process = [&nNms](const TSQueryMatch& match)
            { nNms = match.captures[0].node; };
            for_each_match(nCurrent, regs::QueryType::FileNamespace, process);

            if (ts_node_is_null(nNms))
                break;

            const TSNode n_name    = child_by_field_name(nNms, "name");
            const std::string name = extract_text(n_name, src);
            split_namespace(scopes, name);
            break;
        }
        case Namespace:
        {
            foundNms               = true;
            const TSNode n_name    = child_by_field_name(nCurrent, "name");
            const std::string name = extract_text(n_name, src);
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
    const CSModifiers paramMod = CSModifiers::handle_modifs_param(node, src);
    Type* tParam = paramMod.get_indection_type(th(&typeTrs, nType));

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
} // namespace astfri::csharp::util