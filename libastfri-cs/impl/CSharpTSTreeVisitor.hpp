#ifndef CSHARP_TS_TREE_VISITOR_HPP
#define CSHARP_TS_TREE_VISITOR_HPP

#include <functional>
#include <string>
#include <tree_sitter/api.h>
#include <unordered_map>
#include <utility>

#include "libastfri/inc/ExprFactory.hpp"
#include "libastfri/inc/StmtFactory.hpp"

namespace astfri::csharp
{

class CSharpTSTreeVisitor
{
public:
    using ExprHandler = std::function<Expr*(CSharpTSTreeVisitor *self, TSNode const*)>;
    using StmtHandler = std::function<Stmt*(CSharpTSTreeVisitor *self, TSNode const*)>;
private:
    using ExprFactory = astfri::ExprFactory;
    using StmtFactory = astfri::StmtFactory;

    std::string source_code_;
    std::unordered_map<std::string, StmtHandler> stmt_handlers_;
    std::unordered_map<std::string, ExprHandler> expr_handlers_ = {
        {"integer_literal", handle_int_lit},
        {"real_literal", handle_float_lit},
        {"boolean_literal", handle_bool_lit},
        {"character_literal", handle_char_lit},
        {"string_literal", handle_str_lit},
        {"null_literal", handle_null_literal},
        {"verbatim_string_literal", handle_verbatim_str_lit},
        {"this_expression", handle_this_expr},
        {"conditional_expression", handle_ternary_expr},
        {"prefix_unary_expression", handle_prefix_unary_op_expr},
        {"postfix_unary_expression", handle_postfix_unary_op_expr},

        {"ERROR", [](CSharpTSTreeVisitor*, TSNode const *node) -> Expr*
            {
                auto const [row, column] = ts_node_start_point(*node);
                throw std::runtime_error(
                    "Invalid C# syntax in source code at: row" +
                    std::to_string(row) + "and column " +
                    std::to_string(column)
                );
            }
        }
    };
    std::unordered_map<std::string, UnaryOpType> prefix_unary_operation = {
        {"+", UnaryOpType::Plus},
        {"-", UnaryOpType::Minus},
        {"++", UnaryOpType::PreIncrement},
        {"--", UnaryOpType::PreDecrement},
        {"!", UnaryOpType::LogicalNot},
        {"~", UnaryOpType::BitFlip},
        {"*", UnaryOpType::Dereference},
        {"&", UnaryOpType::AddressOf}
    };
public:
    explicit CSharpTSTreeVisitor(std::string  source_code)
        : source_code_(std::move(source_code))
    { }

    // Expressions
    ExprHandler get_expr_handler(TSNode const& node);

    //Literals
    static Expr* handle_int_lit(CSharpTSTreeVisitor *self, const TSNode* node); //todo
    static Expr* handle_float_lit(CSharpTSTreeVisitor *self, const TSNode* node); //todo
    static Expr* handle_bool_lit(CSharpTSTreeVisitor *self, const TSNode* node);
    static Expr* handle_char_lit(CSharpTSTreeVisitor *self, const TSNode* node); //todo
    static Expr* handle_str_lit(CSharpTSTreeVisitor *self, const TSNode* node); //todo
    static Expr* handle_null_literal(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_this_expr(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_verbatim_str_lit(CSharpTSTreeVisitor* self, TSNode const* node); //todo
    static Expr* handle_interpolated_str_lit(CSharpTSTreeVisitor* self, TSNode const* node); //todo

    //Reference Expersions
    static Expr* handle_param_var_ref_expr(CSharpTSTreeVisitor* self, TSNode const* node); //todo
    static Expr* handle_local_var_ref_expr(CSharpTSTreeVisitor* self, TSNode const* node); //todo

    // Operations
    static Expr* handle_prefix_unary_op_expr(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_postfix_unary_op_expr(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_binary_op_expr(CSharpTSTreeVisitor* self, TSNode const* node); // todo
    static Expr* handle_ternary_expr(CSharpTSTreeVisitor* self, TSNode const* node); //todo

    //Statements
    Stmt* handler_stmt(const TSNode* node); //todo

    // Variable Definitions
    // static Stmt* handle_local_var_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node); //todo
    // static Stmt* handle_param_var_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node); //todo
    // static Stmt* handle_member_var_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node); //todo

};

} // namespace astfri

#endif // CSHARP_TS_TREE_VISITOR_HPP