#ifndef CSHARP_REGISTRIES_HPP
#define CSHARP_REGISTRIES_HPP

#include <libastfri-cs/impl/CSAliases.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string>
#include <unordered_map>

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
class SourceCodeVisitor;

namespace regs
{

struct Handlers
{
    const RegistryMap<StmtHandler> stmts;
    const RegistryMap<ExprHandler> exprs;
    const RegistryMap<RegHandler> symbol_reg_handlers;
    Handlers();
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
    static RegHandler get_reg_handler(const TSNode& node);
    static StmtHandler get_stmt_handler(std::string_view node_type);
    static ExprHandler get_expr_handler(std::string_view node_type);
    static RegHandler get_reg_handler(std::string_view node_type);
    static std::optional<UnaryOpType> get_prefix_unary_op(std::string_view op);
    static std::optional<BinOpType> get_bin_op(std::string_view operation);
    static std::optional<Type*> get_type(std::string_view type_name);
    static std::optional<CSModifier> get_modifier(std::string_view modifier);
    static bool is_expr(const TSNode& node);
    static bool is_stmt(const TSNode& node);

private:
    static Expr* default_expr_handler(SourceCodeVisitor*, const TSNode*);
    static Stmt* default_stmt_handler(SourceCodeVisitor*, const TSNode*);
};
} // namespace astfri::csharp
#endif // CSHARP_REGISTRIES_HPP