#ifndef CSHARP_REGISTRIES_HPP
#define CSHARP_REGISTRIES_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/regs/NodeType.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <stdexcept>
#include <string>
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
class SrcCodeVisitor;

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
    const RegistryMap<SymbCollector> symbCollectors;
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
    const RegistryStrViewMap<UnaryOpType> prefixUnaryOps;
    const RegistryStrViewMap<BinOpType> binaryOps;
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
} // namespace maps

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
    static maps::Mappers handlers_;
    static maps::Modifiers modifiers_;
    static maps::Operations operations_;
    static maps::NodeTypes nodeTypes_;
    static maps::Types types_;

public:
    static StmtMapper get_stmt_mapper(const TSNode& node);
    static StmtMapper get_stmt_mapper(NodeType nodeType);
    static ExprMapper get_expr_mapper(const TSNode& node);
    static ExprMapper get_expr_mapper(NodeType nodeType);
    static TypeMapper get_type_mapper(const TSNode& node);
    static TypeMapper get_type_mapper(NodeType nodeType);
    static TypeCollector get_type_collector(const TSNode& node);
    static TypeCollector get_type_collector(NodeType nodeType);
    static SymbCollector get_symb_collector(const TSNode& node);
    static SymbCollector get_symb_collector(NodeType nodeType);
    static std::optional<UnaryOpType> get_prefix_unary_op(std::string_view op);
    static std::optional<BinOpType> get_bin_op(std::string_view op);
    static Type* get_primitive_type(std::string_view nodeType);
    static CSModifier get_modifier(const TSNode& node, std::string_view src);
    static CSModifier get_modifier(std::string_view modifs);
    static bool is_expr(const TSNode& node);
    static bool is_stmt(const TSNode& node);
    static NodeType get_node_type(const TSNode& node);
    static TSSymbol get_symbol(NodeType type);

private:
    static Stmt* default_stmt_visit(SrcCodeVisitor*, const TSNode&);
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
     * Gets optional value from the registry map
     * @tparam Type type of value in the registry map
     * @param map map for lookup
     * @param name name to lookup
     * @return optional value from the map
     */
    template<class Type>
    static std::optional<Type> get_opt(const RegistryStrViewMap<Type>& map, std::string_view name);
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/regs/Registries.inl>

#endif // CSHARP_REGISTRIES_HPP