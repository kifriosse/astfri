#ifndef CSHARP_REGISTRIES_HPP
#define CSHARP_REGISTRIES_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/regs/NodeType.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace astfri
{
// forward declarations
enum class BinOpType;
enum class UnaryOpType;

struct Type;
struct Stmt;
struct Expr;

class TypeFactory;

} // namespace astfri

namespace astfri::csharp
{
enum class CSModifier : MaskType;
class SrcCodeVisitor;

namespace regs
{

struct MappingRule
{
    std::string_view nodeName;
    NodeType nodeType;
    bool isNamed;
};

class NodeTypes
{
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

struct Handlers
{
    const RegistryMap<StmtHandler> stmts;
    const RegistryMap<ExprHandler> exprs;
    const RegistryMap<TypeHandler> types;
    const RegistryMap<RegHandler> symbolRegs;
    Handlers();

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

struct Operations
{
    const RegistryStrMap<UnaryOpType> prefixUnaryOps;
    const RegistryStrMap<BinOpType> binaryOps;
    Operations();
};

struct Types
{
    TypeFactory& typeFact;
    const RegistryStrMap<Type*> types;
    Types();
};

struct Modifiers
{
    const RegistryStrMap<CSModifier> modifiers;
    Modifiers();
};
} // namespace regs

class RegManager
{
private:
    static regs::Handlers handlers_;
    static regs::Operations operations_;
    static regs::Types types_;
    static regs::Modifiers modifiers_;
    static regs::NodeTypes nodeTypes_;

public:
    static StmtHandler get_stmt_handler(const TSNode& node);
    static ExprHandler get_expr_handler(const TSNode& node);
    static TypeHandler get_type_handler(const TSNode& node);
    static RegHandler get_reg_handler(const TSNode& node);
    static StmtHandler get_stmt_handler(NodeType nodeType);
    static ExprHandler get_expr_handler(NodeType nodeType);
    static TypeHandler get_type_handler(NodeType nodeType);
    static RegHandler get_reg_handler(NodeType nodeType);
    static std::optional<UnaryOpType> get_prefix_unary_op(std::string_view op);
    static std::optional<BinOpType> get_bin_op(std::string_view op);
    static std::optional<Type*> get_type(std::string_view nodeType);
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
    static Type get_or_default(
        const RegistryMap<Type>& map,
        NodeType nodeType,
        Type nDefVal
    );
    /**
     * Gets optional value from the registry map
     * @tparam Type type of value in the registry map
     * @param map map for lookup
     * @param name name to lookup
     * @return optional value from the map
     */
    template<class Type>
    static std::optional<Type> get_opt(
        const std::unordered_map<std::string_view, Type>& map,
        std::string_view name
    );
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/regs/Registries.inl>

#endif // CSHARP_REGISTRIES_HPP