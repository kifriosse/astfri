#ifndef CSHARP_REGISTRIES_HPP
#define CSHARP_REGISTRIES_HPP

#include <astfri-cs/impl/CSFwd.hpp>
#include <astfri-cs/impl/regs/NodeType.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace astfri {
// forward declarations
enum class BinOpType;
enum class UnaryOpType;

struct Type;
struct Stmt;
struct Expr;

class TypeFactory;

} // namespace astfri

namespace astfri::csharp {

enum class CSModifier : MaskType;
class SrcCodeTransformer;

namespace maps {


/**
 * @brief Class holding mapping of tree-sitter symbols to NodeType enum values
 * and vice versa.
 */
class NodeTypes {
public:
    static constexpr TSSymbol INVALID_SYMBOL = UINT16_MAX;

private:
    std::vector<TSSymbol> symbolMap_;
    std::vector<NodeType> nodeTypeMap_;

public:
    NodeTypes();
    [[nodiscard]] TSSymbol get_symbol(NodeType type) const;
    [[nodiscard]] NodeType get_node_type(const TSNode& node) const;
};

/**
 * @brief Struct holding all mapping to visit functions for tree-sitter nodes.
 */
struct Mappers {
    const RegistryMap<StmtMapper> stmts;
    const RegistryMap<ExprMapper> exprs;
    const RegistryMap<TypeMapper> types;
    const RegistryMap<TypeCollector> typeCollectors;
    const RegistryMap<MemberCollector> symbCollectors;
    Mappers();

private:
    /**
     * Default error visit function that throws runtime error
     * @tparam Self type of self parameter used in handler
     * @tparam RetType return type of handler
     * @return always throws runtime error
     */
    template<class Self, class RetType>
    static RetType visit_error(Self*, const TSNode& node);
};

/**
 * @brief Struct holding mappings of C# operations to AST FRI Operation type
 * enum
 */
struct Operations {
    const RegistryMap<UnaryOpType> prefixUnaryOps;
    const RegistryMap<BinOpType> binaryOps;
    Operations();
};

/**
 * @brief Struct holding mapping of C# primitive types (in string form) to
 * AST FRI Type
 */
struct Types {
    TypeFactory& typeFact;
    const RegistryStrViewMap<Type*> types;
    Types();
};

/**
 * @brief Struct holding mapping of C# modifiers (in string form) to CSModifier
 * enum
 */
struct Modifiers {
    const RegistryStrViewMap<CSModifier> modifiers;
    Modifiers();
};

/**
 * @brief Ensures a class follows the Astfri Factory pattern.
 * * Requirements:
 * - Must have a static \c get_instance() returning \c Factory&.
 * - Must have \c mk_unknown() returning \c RetType.
 */
template<typename Factory, typename RetType>
concept is_valid_factory = requires(Factory f) {
    { Factory::get_instance() } -> std::same_as<Factory&>;
    { Factory::get_instance().mk_unknown() } -> std::convertible_to<RetType>;
};

/**
 * @brief Class responsible for managing all C# maps
 */
class MapManager {
private:
    const Mappers handlers_;
    const Modifiers modifiers_;
    const Operations operations_;
    const NodeTypes nodeTypes_;
    const Types types_;

    MapManager() = default;
public:

    MapManager(const MapManager&) = delete;
    MapManager(MapManager&&) = delete;
    MapManager& operator=(const MapManager&) = delete;
    MapManager& operator=(MapManager&&) = delete;

    static MapManager& get();

    StmtMapper get_stmt_mapper(const TSNode& node);
    StmtMapper get_stmt_mapper(NodeType nodeType);
    ExprMapper get_expr_mapper(const TSNode& node);
    ExprMapper get_expr_mapper(NodeType nodeType);
    TypeMapper get_type_mapper(const TSNode& node);
    TypeMapper get_type_mapper(NodeType nodeType);
    TypeCollector get_type_collector(const TSNode& node);
    TypeCollector get_type_collector(NodeType nodeType);
    MemberCollector get_symb_collector(const TSNode& node);
    MemberCollector get_symb_collector(NodeType nodeType);
    std::optional<UnaryOpType> get_prefix_unary_op(const TSNode& op) const;
    std::optional<BinOpType> get_bin_op(const TSNode& op) const;
    Type* get_primitive_type(std::string_view nodeType);
    CSModifier get_modifier(const TSNode& node, std::string_view src) const;
    CSModifier get_modifier(std::string_view modifs) const;
    bool is_expr(const TSNode& node) const;
    bool is_stmt(const TSNode& node);
    NodeType get_node_type(const TSNode& node) const;
    TSSymbol get_symbol(NodeType type) const;

private:
    static Stmt* default_stmt_visit(SrcCodeTransformer*, const TSNode&);
    /**
     * Default visit function that returns an instance of unknown
     * @tparam Factory Factory class with \c get_instance() and \c mk_unknown()
     * methods
     * @tparam Self type of self used in handler
     * @tparam RetType return type of handler
     * @return unknown instance of \code RetType\endcode
     */
    template<class Factory, class Self, class RetType>
    requires is_valid_factory<Factory, RetType>
    static RetType default_visit(Self*, const TSNode&);

    /**
     * Gets value from the registry map or returns default value
     * @tparam Type type of value in the registry map
     * @param map map for lookup
     * @param nodeType name to lookup
     * @param nDefVal default value to return if name wasn't found
     * @return value from the map or default value
     */
    template<class Type>
    static Type get_or_default(const RegistryMap<Type>& map, NodeType nodeType, Type nDefVal);
    /**
     * @brief Gets optional value from the registry map
     * @tparam Map map type, needs to implement method find
     * @param map map to lookup in
     * @param key key to lookup
     * @return optional value from the map
     */
    template <typename Map>
    static auto get_opt(const Map& map, const Map::key_type& key)
        -> std::optional<typename Map::mapped_type>;
};

} // namespace maps

} // namespace astfri::csharp

#include <astfri-cs/impl/regs/Maps.inl>

#endif // CSHARP_REGISTRIES_HPP
