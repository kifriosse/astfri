#ifndef CSHARP_QUERY_REGISTRY_HPP
#define CSHARP_QUERY_REGISTRY_HPP

#include <tree_sitter/api.h>

#include <string>
#include <unordered_map>

namespace astfri::csharp::regs
{

enum class QueryType
{
    TopLevel,
    VarDecltor,
    VarModifier,
    MethodModifier,
    FileNamespace,
    CommentError,
    Using
};

class QueryReg
{
private:
    std::unordered_map<QueryType, TSQuery*> queries_{};

    QueryReg();
    ~QueryReg();
    void init();

public:
    static QueryReg& get();
    QueryReg(const QueryReg& other)           = delete;
    QueryReg(QueryReg&& other)                = delete;
    QueryReg operator=(const QueryReg& other) = delete;
    QueryReg operator=(QueryReg&& other)      = delete;

    const TSQuery* get_query(QueryType type);

private:
    static const std::string top_level_stmts_q;
    static const std::string decltor_q;
    static const std::string var_modif_q;
    static const std::string method_modif_q;
    static const std::string file_namespace_q;
    static const std::string comment_error_q;
    static const std::string using_dir_q;
};
} // namespace astfri::csharp::regs

#endif // CSHARP_QUERY_REGISTRY_HPP

