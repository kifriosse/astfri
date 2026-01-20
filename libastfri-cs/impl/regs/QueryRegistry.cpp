#include <libastfri-cs/impl/regs/Queries.h>
#include <libastfri-cs/impl/regs/QueryRegistry.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <iostream>
#include <ranges>
#include <string>

namespace astfri::csharp::regs
{

Query::Query(const TSLanguage* lang, const std::string_view query)
{
    TSQueryError err;
    uint32_t offset;
    query_ = ts_query_new(lang, query.data(), query.length(), &offset, &err);

    if (err != TSQueryErrorNone)
    {
        std::cerr << "Error while creating query at offset " << offset
                  << "\nError code " << err;

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

        if (offset < query.length())
        {
            const size_t start = offset >= 10 ? offset - 10 : 0;
            const size_t end   = std::min<size_t>(offset + 10, query.size());
            std::cerr << "Query snippet around error:\n"
                      << query.substr(start, end - start) << '\n';
        }
    }
    else
    {
        for (size_t i = 0; i < ts_query_capture_count(query_); ++i)
        {
            uint32_t len;
            const char* name = ts_query_capture_name_for_id(query_, i, &len);
            capture_ids_.try_emplace({name, len}, i);
        }
    }
}

Query::~Query()
{
    if (query_)
    {
        ts_query_delete(query_);
        query_ = nullptr;
    }
}

Query::Query(Query&& other) noexcept :
    query_(other.query_),
    capture_ids_(std::move(other.capture_ids_))
{
    other.query_ = nullptr;
}

Query& Query::operator=(Query&& other) noexcept
{
    if (this != &other)
    {
        if (query_)
            ts_query_delete(query_);
        query_       = other.query_;
        other.query_ = nullptr;
        capture_ids_ = std::move(other.capture_ids_);
    }
    return *this;
}

uint32_t Query::id(const std::string_view name) const
{
    const auto it = capture_ids_.find(name);
    return it != capture_ids_.end()
             ? it->second
             : throw std::out_of_range(
                   "Query: Capture id not found for name: \""
                   + std::string(name) + "\""
               );
}

TSQuery* Query::get() const
{
    return query_;
}

QueryReg& QueryReg::get()
{
    static QueryReg instance;
    return instance;
}

const Query* QueryReg::get_query(const QueryType type) const
{
    const auto it = queries_.find(type);
    return it != queries_.end()
             ? &it->second
             : throw std::out_of_range("QueryReg: Query not found");
}

QueryReg::QueryReg()
{
    using enum QueryType;
    const std::pair<QueryType, std::string_view> mapping[] = {
        {TopLevel,      queries::top_level_stmts_q},
        {VarDecltor,    queries::decltor_q        },
        {VarDecl,       queries::var_decl_q       },
        {ParamModif,    queries::param_modif_q    },
        {MethodModif,   queries::method_modif_q   },
        {FileNamespace, queries::file_namespace_q },
        {CommentError,  queries::comment_error_q  },
        {Using,         queries::using_dir_q      }
    };

    const TSLanguage* lang = tree_sitter_c_sharp();

    for (const auto& [type, query_str] : mapping)
    {
        queries_.try_emplace(type, lang, query_str);
    }
}

} // namespace astfri::csharp::regs
