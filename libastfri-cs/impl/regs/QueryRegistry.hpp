#ifndef CSHARP_QUERY_REGISTRY_HPP
#define CSHARP_QUERY_REGISTRY_HPP

#include <libastfri-cs/impl/CSAliases.hpp>

#include <tree_sitter/api.h>

#include <unordered_map>

namespace astfri::csharp::regs
{

enum class QueryType
{
    TopLevel,
    VarDecltor,
    VarDecl,
    ParamModif,
    MethodModif,
    FileNamespace,
    CommentError,
    Using,
};

class Query
{
private:
    TSQuery* query_;
    IdentifierMap<uint32_t> capture_ids_;

public:
    Query(const TSLanguage* lang, std::string_view query);
    ~Query();
    Query(Query&& other) noexcept;
    Query& operator=(Query&& other) noexcept;

    uint32_t id(std::string_view name) const;
    TSQuery* get() const;
};

class QueryReg
{
private:
    std::unordered_map<QueryType, const Query> queries_{};
    QueryReg();

public:
    static QueryReg& get();

    QueryReg(const QueryReg& other)           = delete;
    QueryReg(QueryReg&& other)                = delete;
    QueryReg operator=(const QueryReg& other) = delete;
    QueryReg operator=(QueryReg&& other)      = delete;

    const Query* get_query(QueryType type) const;
};
} // namespace astfri::csharp::regs

#endif // CSHARP_QUERY_REGISTRY_HPP
