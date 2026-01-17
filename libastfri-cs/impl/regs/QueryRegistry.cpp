#include <libastfri-cs/impl/regs/QueryRegistry.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace astfri::csharp::regs
{

QueryReg& QueryReg::get()
{
    static QueryReg instance;
    return instance;
}

const TSQuery* QueryReg::get_query(const QueryType type)
{
    const auto it = queries_.find(type);
    return it != queries_.end() ? it->second : nullptr;
}

QueryReg::QueryReg()
{
    init();
}

QueryReg::~QueryReg()
{
    for (auto query : queries_ | std::ranges::views::values)
    {
        if (query)
        {
            ts_query_delete(query);
            query = nullptr;
        }
    }
}

void QueryReg::init()
{
    using enum QueryType;
    const std::vector<std::pair<QueryType, const std::string&>> mapping = {
        {TopLevel,       top_level_stmts_q},
        {VarDecltor,     decltor_q        },
        {VarModifier,    var_modif_q      },
        {MethodModifier, method_modif_q   },
        {FileNamespace,  file_namespace_q },
        {CommentError,   comment_error_q  },
        {Using,          using_dir_q      }
    };

    const TSLanguage* lang = tree_sitter_c_sharp();
    for (auto& [type, query_str] : mapping)
    {
        TSQueryError err;
        uint32_t offset;
        TSQuery* query = ts_query_new(
            lang,
            query_str.c_str(),
            query_str.length(),
            &offset,
            &err
        );

        if (err != TSQueryErrorNone)
        {
            std::cerr << "Error while creating query at offset " << offset
                      << "\nError code " << err << std::endl;

            switch (err)
            {
            case TSQueryErrorSyntax:
                std::cerr << " : Syntax error in query.\n";
                break;
            case TSQueryErrorNodeType:
                std::cerr << " : Invalid node type in query.\n";
                break;
            case TSQueryErrorField:
                std::cerr << " : Invalid field in query.\n";
                break;
            case TSQueryErrorCapture:
                std::cerr << " : Invalid capture in query.\n";
                break;
            case TSQueryErrorStructure:
                std::cerr << " : Malformed query structure.\n";
                break;
            default:
                std::cerr << " : Unknown error.\n";
            }

            if (offset < query_str.length())
            {
                std::cerr << "Query snippet around error:\n";
                const size_t start = offset >= 10 ? offset - 10 : 0;
                const size_t end
                    = std::min<size_t>(offset + 10, query_str.size());
                std::cerr << query_str.substr(start, end - start) << '\n';
            }
        }

        queries_.emplace(type, query);
    }
}

} // namespace astfri::csharp::regs
