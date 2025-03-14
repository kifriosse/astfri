#ifndef EXPRESSION_TRANSFORMER_CLASS_HPP
#define EXPRESSION_TRANSFORMER_CLASS_HPP

#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <variant>

#include "libastfri-java/src/NodeMapper.hpp"

class ExpressionTransformer
{
private:
    astfri::TypeFactory& typeFactory;
    astfri::ExprFactory& exprFactory;
    NodeMapper* nodeMapper;

    astfri::BinOpExpr* get_bin_op_expr(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::UnaryOpExpr* get_un_op_expr(
        TSNode tsNode,
        std::string const& sourceCode
    );

    std::variant<
        astfri::ParamVarRefExpr*,
        astfri::LocalVarRefExpr*,
        astfri::MemberVarRefExpr*,
        astfri::StringLiteralExpr*>
        get_ref_expr(TSNode tsNode, std::string const& sourceCode);

    astfri::MethodCallExpr* get_method_call(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::NewExpr* get_new_expr(
        TSNode paramsNode,
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