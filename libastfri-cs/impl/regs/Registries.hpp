#ifndef CSHARP_REGISTRIES_HPP
#define CSHARP_REGISTRIES_HPP

#include <libastfri-cs/impl/CSAliases.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string>
#include <unordered_map>
// ReSharper disable once CppUnusedIncludeDirective
#include <stdexcept>

namespace astfri
{

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

struct Handlers
{
    const RegistryMap<StmtHandler> stmts;
    const RegistryMap<ExprHandler> exprs;
    const RegistryMap<TypeHandler> types;
    const RegistryMap<RegHandler> symbol_regs;
    Handlers();

private:
    /**
     * Default error visit function that throws runtime error
     * @tparam Self type of self used in handler
     * @tparam RetType return type of handler
     * @return always throws runtime error
     */
    template<class Self, class RetType>
    static RetType visit_error(Self*, const TSNode& node);
};

struct Operations
{
    const RegistryMap<UnaryOpType> prefix_unary_op;
    const RegistryMap<BinOpType> bin_operations;
    Operations();
};

struct Types
{
    TypeFactory& type_factory;
    const RegistryMap<Type*> types;
    Types();
};

struct Modifiers
{
    const RegistryMap<CSModifier> modifiers;
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

public:
    static StmtHandler get_stmt_handler(const TSNode& node);
    static ExprHandler get_expr_handler(const TSNode& node);
    static TypeHandler get_type_handler(const TSNode& node);
    static RegHandler get_reg_handler(const TSNode& node);
    static StmtHandler get_stmt_handler(std::string_view node_type);
    static ExprHandler get_expr_handler(std::string_view node_type);
    static TypeHandler get_type_handler(std::string_view node_type);
    static RegHandler get_reg_handler(std::string_view node_type);
    static std::optional<UnaryOpType> get_prefix_unary_op(std::string_view op);
    static std::optional<BinOpType> get_bin_op(std::string_view op);
    static std::optional<Type*> get_type(std::string_view type_name);
    static std::optional<CSModifier> get_modifier(std::string_view modifier);
    static bool is_expr(const TSNode& node);
    static bool is_stmt(const TSNode& node);

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
     * @param name name to lookup
     * @param def_value default value to return if name wasn't found
     * @return value from the map or default value
     */
    template<class Type>
    static Type get_or_default(
        const RegistryMap<Type>& map,
        std::string_view name,
        Type def_value
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
        const RegistryMap<Type>& map,
        std::string_view name
    );
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/regs/Registries.inl>

#endif // CSHARP_REGISTRIES_HPP