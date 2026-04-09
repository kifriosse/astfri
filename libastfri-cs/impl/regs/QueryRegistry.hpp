#ifndef CSHARP_QUERY_REGISTRY_HPP
#define CSHARP_QUERY_REGISTRY_HPP

#include <libastfri-cs/impl/CSFwd.hpp>

#include <tree_sitter/api.h>

#include <unordered_map>

namespace astfri::csharp::maps {

/**
 * @brief Enum for identifying different types of queries in the registry
 */
enum class QueryType {
    TopLevel,
    VarDecltor,
    VarDecl,
    ParamModif,
    MethodModif,
    FileNamespace,
    CommentError,
    Using,
};

/**
 * @brief Class representing a tree-sitter query with capture id mapping.
 * Contains precompiled query and mapping from capture names to their ids for
 * easy access.
 * @note Query is not copyable
 */
class Query {
private:
    TSQuery* query_;
    IdentifierMap<CaptureId> captureIds;

public:
    /**
     * @brief Constructor for Query class. Compiles the query and initializes
     * capture id mapping.
     * @param lang language for which the query is compiled.
     * @param query string representation of the query to compile.
     */
    Query(const TSLanguage* lang, std::string_view query);
    ~Query();
    Query(const Query& other)            = delete;
    Query& operator=(const Query& other) = delete;
    Query(Query&& other) noexcept;
    Query& operator=(Query&& other) noexcept;

    /**
     * @brief Returns capture id for the given capture name.
     * @param name name of the capture to get id for.
     * @return capture id for the given capture name.
     * @throws std::out_of_range if capture name is not found in the query.
     */
    CaptureId id(std::string_view name) const;
    /**
     * @brief Returns pointer to the compiled query.
     * @return pointer to the compiled query.
     */
    TSQuery* get() const;
};

/**
 * @brief Registry for tree-sitter queries.
 * @note Uses singleton pattern which makes it not copyable and not movable.
 */
class QueryReg {
private:
    std::unordered_map<QueryType, const Query> queries_{};
    QueryReg();

public:
    /**
     * @brief Returns reference to the singleton instance of QueryReg.
     * @return reference to the singleton instance of QueryReg.
     */
    static QueryReg& get();

    QueryReg(const QueryReg& other)           = delete;
    QueryReg(QueryReg&& other)                = delete;
    QueryReg operator=(const QueryReg& other) = delete;
    QueryReg operator=(QueryReg&& other)      = delete;

    /**
     * @brief Returns pointer to the query for the given query type.
     * @param type type of the query to get.
     * @return pointer to the query for the given query type. \c nullptr if
     * query type is not implemented.
     */
    const Query* get_query(QueryType type) const;
};
} // namespace astfri::csharp::maps

#endif // CSHARP_QUERY_REGISTRY_HPP
