#ifndef CSHARP_NODE_REGISTRY_HPP
#define CSHARP_NODE_REGISTRY_HPP

#include <optional>
#include <string>
#include <unordered_map>

#include "CSharpTSTreeVisitor.hpp"
#include "libastfri/inc/Expr.hpp"

namespace astfri::csharp
{
class NodeRegistry
{
public:
    static CSharpTSTreeVisitor::StmtHandler get_stmt_handler(TSNode const& node);
    static CSharpTSTreeVisitor::ExprHandler get_expr_handler(TSNode const& node);
    static std::optional<UnaryOpType> get_prefix_unary_op(std::string const& operation);
    static std::optional<BinOpType> get_bin_op(std::string const& operation);
    static std::optional<Type*> get_type(std::string const& type_name);
    static std::optional<AccessModifier> get_access_modifier(std::string const& modifier);

private:
    static std::unordered_map<std::string, CSharpTSTreeVisitor::StmtHandler> stmt_handlers_;
    static std::unordered_map<std::string, CSharpTSTreeVisitor::ExprHandler> expr_handlers_;
    static std::unordered_map<std::string, UnaryOpType> prefix_unary_op;
    static std::unordered_map<std::string, BinOpType> bin_operations;
    static std::unordered_map<std::string, Type*> types_;
    static std::unordered_map<std::string, AccessModifier> access_modifiers;

private:
    static Expr* default_expr_handler(CSharpTSTreeVisitor*, TSNode const*);
    static Stmt* default_stmt_handler(CSharpTSTreeVisitor*, TSNode const*);
};
} // namespace astfri::csharp

#endif // CSHARP_NODE_REGISTRY_HPP