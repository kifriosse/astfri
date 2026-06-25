#ifndef ASTFRI_JAVA_IMPL_EXPRESSION_TRANSFORMER_HPP
#define ASTFRI_JAVA_IMPL_EXPRESSION_TRANSFORMER_HPP

#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/StmtFwd.hpp>
#include <astfri/impl/TypeFwd.hpp>

#include <astfri-java/impl/NodeMapper.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>

#include <string>


namespace astfri::java {


class StatementTransformer;


class ExpressionTransformer {
public:
    ExpressionTransformer(
        astfri::ExprFactory *exprFactory,
        astfri::TypeFactory *typeFactory,
        StatementTransformer *stmtTr,
        NodeMapper *nodeMapper);

    std::string get_node_text(const TSNode& node, const std::string& sourceCode);

    astfri::Expr* get_expr(TSNode tsNode, const std::string& sourceCode);

private:
    astfri::BinOpExpr* transform_bin_op_expr_node(TSNode tsNode, const std::string& sourceCode);

    astfri::UnaryOpExpr* transform_un_op_expr_node(TSNode tsNode, const std::string& sourceCode);

    Expr* transform_ref_expr_node(TSNode tsNode, const std::string& sourceCode);

    astfri::MethodCallExpr* transform_method_call_node(
        TSNode tsNode,
        const std::string& sourceCode
    );

    astfri::NewExpr* transform_new_expr_node(TSNode tsNode, const std::string& sourceCode);

    astfri::IfExpr* transform_ternary_expr_node(TSNode tsNode, const std::string& sourceCode);

private:
    astfri::ExprFactory *m_exprFactory;
    astfri::TypeFactory *m_typeFactory;
    StatementTransformer *m_stmtTr;
    NodeMapper *m_nodeMapper;
};


} // namespace astfri::java

#endif // ASTFRI_JAVA_IMPL_EXPRESSION_TRANSFORMER_HPP
