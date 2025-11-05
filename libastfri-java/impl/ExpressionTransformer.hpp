#ifndef EXPRESSION_TRANSFORMER_CLASS_HPP
#define EXPRESSION_TRANSFORMER_CLASS_HPP

#include <libastfri/inc/Astfri.hpp>
#include <libastfri-java/impl/NodeMapper.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>

namespace astfri::java
{

class StatementTransformer;

class ExpressionTransformer
{
private:
    astfri::TypeFactory& typeFactory;
    astfri::ExprFactory& exprFactory;
    NodeMapper* nodeMapper;
    StatementTransformer* stmtTr;

    astfri::BinOpExpr* transform_bin_op_expr_node(TSNode tsNode, std::string const& sourceCode);

    astfri::UnaryOpExpr* transform_un_op_expr_node(TSNode tsNode, std::string const& sourceCode);

    Expr* transform_ref_expr_node(TSNode tsNode, std::string const& sourceCode);

    astfri::MethodCallExpr* transform_method_call_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::NewExpr* transform_new_expr_node(TSNode tsNode, std::string const& sourceCode);

    astfri::IfExpr* transform_ternary_expr_node(TSNode tsNode, std::string const& sourceCode);

public:
    ExpressionTransformer(StatementTransformer* stmtTr);
    ~ExpressionTransformer();

    std::string get_node_text(TSNode const& node, std::string const& sourceCode);

    astfri::Expr* get_expr(TSNode tsNode, std::string const& sourceCode);
};

} // namespace astfri::java
#endif // EXPRESSION_TRANSFORMER_CLASS_HPP