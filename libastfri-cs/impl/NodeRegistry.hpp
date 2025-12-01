#ifndef CSHARP_NODE_REGISTRY_HPP
#define CSHARP_NODE_REGISTRY_HPP

#include <libastfri-cs/impl/CSharpTSTreeVisitor.hpp>
#include <libastfri-cs/impl/CSModifiers.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <optional>
#include <string>
#include <unordered_map>

namespace astfri::csharp
{
class NodeRegistry
{
private:
    static TypeFactory& type_factory_;

    static std::unordered_map<std::string, CSharpTSTreeVisitor::StmtHandler>
        stmt_handlers_;
    static std::unordered_map<std::string, CSharpTSTreeVisitor::ExprHandler>
        expr_handlers_;
    static std::unordered_map<std::string, UnaryOpType> prefix_unary_op;
    static std::unordered_map<std::string, BinOpType> bin_operations;
    static std::unordered_map<std::string, Type*> types_;
    static std::unordered_map<std::string, CSModifier> modifiers;

public:
    static CSharpTSTreeVisitor::StmtHandler get_stmt_handler(
        const TSNode& node
    );
    static CSharpTSTreeVisitor::ExprHandler get_expr_handler(
        const TSNode& node
    );
    static CSharpTSTreeVisitor::StmtHandler get_stmt_handler(
        const std::string& node_type
    );
    static CSharpTSTreeVisitor::ExprHandler get_expr_handler(
        const std::string& node_type
    );
    static std::optional<UnaryOpType> get_prefix_unary_op(
        const std::string& operation
    );
    static std::optional<BinOpType> get_bin_op(const std::string& operation);
    static std::optional<Type*> get_type(const std::string& type_name);
    static std::optional<CSModifier> get_modifier(const std::string& modifier);

private:
    static Expr* default_expr_handler(CSharpTSTreeVisitor*, const TSNode*);
    static Stmt* default_stmt_handler(CSharpTSTreeVisitor*, const TSNode*);
};
} // namespace astfri::csharp

#endif // CSHARP_NODE_REGISTRY_HPP