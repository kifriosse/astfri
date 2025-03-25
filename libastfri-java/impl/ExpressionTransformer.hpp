#ifndef EXPRESSION_TRANSFORMER_CLASS_HPP
#define EXPRESSION_TRANSFORMER_CLASS_HPP

#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri-java/impl/NodeMapper.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <variant>


class ExpressionTransformer
{
private:
    astfri::TypeFactory& typeFactory;
    astfri::ExprFactory& exprFactory;
    NodeMapper* nodeMapper;

    astfri::BinOpExpr* transform_bin_op_expr_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::UnaryOpExpr* transform_un_op_expr_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    std::variant<
        astfri::ParamVarRefExpr*,
        astfri::LocalVarRefExpr*,
        astfri::MemberVarRefExpr*,
        astfri::StringLiteralExpr*>
        transform_ref_expr_node(TSNode tsNode, std::string const& sourceCode);

    astfri::MethodCallExpr* transform_method_call_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::NewExpr* transform_new_expr_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::IfExpr* transform_ternary_expr_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

public:
    ExpressionTransformer();

    std::string get_node_text(
        TSNode const& node,
        std::string const& sourceCode
    );

    astfri::Expr* get_expr(TSNode tsNode, std::string const& sourceCode);
};

#endif // EXPRESSION_TRANSFORMER_CLASS_HPP