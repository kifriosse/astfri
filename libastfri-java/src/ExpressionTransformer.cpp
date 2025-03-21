#include "ExpressionTransformer.hpp"

#include <cstdint>
#include <cstring>
#include <ranges>
#include <string>
#include <tree_sitter/api.h>

#include "libastfri/inc/Expr.hpp"

ExpressionTransformer::ExpressionTransformer() :
    typeFactory(astfri::TypeFactory::get_instance()),
    exprFactory(astfri::ExprFactory::get_instance()),
    nodeMapper(new NodeMapper())
{
}

std::string ExpressionTransformer::get_node_text(
    TSNode const& node,
    std::string const& sourceCode
)
{
    uint32_t start = ts_node_start_byte(node);
    uint32_t end   = ts_node_end_byte(node);
    return sourceCode.substr(start, end - start);
}

astfri::Expr* ExpressionTransformer::get_expr(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    std::string nodeType = ts_node_type(tsNode);
    std::string nodeText = get_node_text(tsNode, sourceCode);
    astfri::Expr* expr;
    if (nodeType == "decimal_integer_literal")
    {
        expr = exprFactory.mk_int_literal(atoi(nodeText.c_str()));
    }
    else if (nodeType == "decimal_floating_point_literal")
    {
        expr = exprFactory.mk_float_literal(atof(nodeText.c_str()));
    }
    else if (nodeType == "character_literal")
    {
        expr = exprFactory.mk_char_literal(nodeText[0]);
    }
    else if (nodeType == "string_literal")
    {
        expr = exprFactory.mk_string_literal(nodeText);
    }
    else if (nodeType == "true")
    {
        expr = exprFactory.mk_bool_literal(true);
    }
    else if (nodeType == "false")
    {
        expr = exprFactory.mk_bool_literal(false);
    }
    else if (nodeType == "null_literal")
    {
        expr = exprFactory.mk_null_literal();
    }
    else if (nodeType == "this")
    {
        expr = exprFactory.mk_this();
    }
    else if (nodeType == "field_access")
    {
        TSNode identifierNode = ts_node_named_child(tsNode, 1);
        auto refExprVariant
            = this->transform_ref_expr_node(identifierNode, sourceCode);
        std::visit([&expr](auto&& arg) { expr = arg; }, refExprVariant);
    }
    else if (nodeType == "identifier")
    {
        auto refExprVariant = this->transform_ref_expr_node(tsNode, sourceCode);
        std::visit([&expr](auto&& arg) { expr = arg; }, refExprVariant);
    }
    else if (nodeType == "binary_expression")
    {
        expr = this->transform_bin_op_expr_node(tsNode, sourceCode);
    }
    else if (nodeType == "assignment_expression")
    {
        expr = this->transform_bin_op_expr_node(tsNode, sourceCode);
    }
    else if (nodeType == "unary_expression")
    {
        expr = this->transform_un_op_expr_node(tsNode, sourceCode);
    }
    else if (nodeType == "update_expression")
    {
        expr = this->transform_un_op_expr_node(tsNode, sourceCode);
    }
    else if (nodeType == "object_creation_expression")
    {
        expr = this->transform_new_expr_node(tsNode, sourceCode);
    }
    else if (nodeType == "method_invocation")
    {
        expr = this->transform_method_call_node(tsNode, sourceCode);
    }
    else if (nodeType == "parenthesized_expression")
    {
        expr = this->get_expr(ts_node_named_child(tsNode, 0), sourceCode);
    }
    else if (nodeType == "ternary_expression")
    {
        expr = this->transform_ternary_expr_node(tsNode, sourceCode);
    }
    else
    {
        expr = exprFactory.mk_unknown();
    }
    return expr;
}

astfri::BinOpExpr* ExpressionTransformer::transform_bin_op_expr_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* leftExpr        = nullptr;
    astfri::BinOpType binOperator = astfri::BinOpType::Add;
    astfri::Expr* rightExpr       = nullptr;

    TSNode leftOperandNode;
    TSNode binOperatorNode;
    TSNode rightOperandNode;

    if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        leftOperandNode             = ts_node_named_child(tsNode, 0);
        std::string leftOperandName = ts_node_string(leftOperandNode);
        leftExpr = this->get_expr(leftOperandNode, sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        rightOperandNode             = ts_node_named_child(tsNode, 1);
        std::string rightOperandName = ts_node_string(rightOperandNode);
        rightExpr = this->get_expr(rightOperandNode, sourceCode);
    }
    if (! ts_node_is_null(ts_node_child(tsNode, 1)))
    {
        binOperatorNode = ts_node_child(tsNode, 1);
        std::string binOperatorName
            = get_node_text(binOperatorNode, sourceCode);
        binOperator
            = this->nodeMapper->get_binOpMap().find(binOperatorName)->second;
    }

    return exprFactory.mk_bin_on(leftExpr, binOperator, rightExpr);
}

astfri::UnaryOpExpr* ExpressionTransformer::transform_un_op_expr_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* expr             = nullptr;
    astfri::UnaryOpType unOperator = astfri::UnaryOpType::Plus;

    TSNode firstNode;
    TSNode secondNode;

    if (! ts_node_is_null(ts_node_child(tsNode, 0)))
    {
        firstNode                 = ts_node_child(tsNode, 0);
        std::string firstNodeName = ts_node_string(firstNode);
        std::string firstNodeText = get_node_text(firstNode, sourceCode);
        if (! ts_node_is_null(ts_node_child(tsNode, 1)))
        {
            secondNode                 = ts_node_child(tsNode, 1);
            std::string secondNodeName = ts_node_string(secondNode);
            std::string secondNodeText = get_node_text(secondNode, sourceCode);
            for (std::string op :
                 std::views::keys(this->nodeMapper->get_unaryOpMap()))
            {
                if (op == firstNodeText)
                {
                    unOperator = this->nodeMapper->get_unaryOpMap()
                                     .find(firstNodeText)
                                     ->second;
                    expr = this->get_expr(secondNode, sourceCode);
                    break;
                }
                else
                {
                    expr       = this->get_expr(firstNode, sourceCode);
                    unOperator = this->nodeMapper->get_unaryOpMap()
                                     .find(secondNodeText)
                                     ->second;
                }
            }
        }
    }
    return exprFactory.mk_unary_op(unOperator, expr);
}

std::variant<
    astfri::ParamVarRefExpr*,
    astfri::LocalVarRefExpr*,
    astfri::MemberVarRefExpr*,
    astfri::StringLiteralExpr*>
    ExpressionTransformer::transform_ref_expr_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
{
    std::string referanceName = get_node_text(tsNode, sourceCode);
    TSNode rootNode           = tsNode;
    while (! ts_node_is_null(ts_node_parent(rootNode)))
    {
        std::string nodeName = ts_node_string(rootNode);
        rootNode             = ts_node_parent(rootNode);
    }
    char const* queryString
        = "(local_variable_declaration type: (_) declarator: "
          "(variable_declarator name: (identifier) @local_var_name value: "
          "(_)?)) (formal_parameter type: (_) name: (identifier) @param_name) "
          "(field_declaration (modifiers) type: (_) declarator: "
          "(variable_declarator name: (identifier) @attr_name value: (_)?))";

    uint32_t errorOffset;
    TSQueryError errorType;

    TSQuery* tsQuery = ts_query_new(
        tree_sitter_java(),
        queryString,
        strlen(queryString),
        &errorOffset,
        &errorType
    );

    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, rootNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeText = get_node_text(tsCapture.node, sourceCode);

            if (captureName == "local_var_name" && referanceName == nodeText)
            {
                return exprFactory.mk_local_var_ref(referanceName);
            }
            if (captureName == "param_name" && referanceName == nodeText)
            {
                return exprFactory.mk_param_var_ref(referanceName);
            }
            if (captureName == "attr_name" && referanceName == nodeText)
            {
                astfri::Expr* owner    = nullptr;

                TSNode fieldAccessNode = ts_node_parent(tsNode);
                std::string fieldAccessNodeName
                    = ts_node_string(fieldAccessNode);

                if (fieldAccessNodeName.find("(field_access") == 0)
                {
                    TSNode objectNode = ts_node_child(fieldAccessNode, 0);
                    std::string objectNodeText
                        = get_node_text(objectNode, sourceCode);

                    if (objectNodeText == "this")
                    {
                        owner = exprFactory.mk_this();
                    }
                    else
                    {
                        owner = exprFactory.mk_class_ref(objectNodeText);
                    }
                }
                return exprFactory.mk_member_var_ref(owner, referanceName);
            }
        }
    }

    ts_query_cursor_delete(tsCursor);
    ts_query_delete(tsQuery);
    return exprFactory.mk_string_literal("??????");
}

astfri::MethodCallExpr* ExpressionTransformer::transform_method_call_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* owner = nullptr;
    std::string name;
    std::vector<astfri::Expr*> arguments;
    uint32_t childCount = ts_node_named_child_count(tsNode);
    uint32_t childIndex = 0;

    for (uint32_t i = 0; i < childCount; i++)
    {
        TSNode child          = ts_node_named_child(tsNode, i);
        std::string childType = ts_node_type(child);
        char const* fieldName
            = ts_node_field_name_for_child(tsNode, childIndex);

        while (! fieldName)
        {
            fieldName = ts_node_field_name_for_child(tsNode, ++childIndex);
        }

        if (childType == "field_access")
        {
            TSNode objectNode          = ts_node_named_child(child, 0);
            std::string objectNodeName = ts_node_string(objectNode);
            if (objectNodeName == "(this)")
            {
                owner = exprFactory.mk_this();
            }
            else
            {
                owner
                    = exprFactory.mk_class_ref(get_node_text(child, sourceCode)
                    );
            }
        }
        else if (strcmp(fieldName, "object") == 0)
        {
            owner = exprFactory.mk_class_ref(get_node_text(child, sourceCode));
        }
        else if (strcmp(fieldName, "name") == 0)
        {
            name = get_node_text(child, sourceCode);
        }
        else if (strcmp(fieldName, "arguments") == 0)
        {
            uint32_t argCount = ts_node_named_child_count(child);
            for (uint32_t j = 0; j < argCount; j++)
            {
                TSNode argNode          = ts_node_named_child(child, j);
                std::string argNodeName = ts_node_string(argNode);
                arguments.push_back(get_expr(argNode, sourceCode));
            }
        }
        ++childIndex;
    }

    return exprFactory.mk_method_call(owner, name, arguments);
}

astfri::NewExpr* ExpressionTransformer::transform_new_expr_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    TSNode typeNode      = ts_node_named_child(tsNode, 0);
    std::string typeName = get_node_text(typeNode, sourceCode);
    astfri::Type* type   = typeFactory.mk_user(typeName);
    std::vector<astfri::Expr*> arguments;
    TSNode argumentsListNode = ts_node_named_child(tsNode, 1);
    uint32_t argsCount       = ts_node_named_child_count(argumentsListNode);
    for (uint32_t j = 0; j < argsCount; j++)
    {
        TSNode argNode = ts_node_named_child(argumentsListNode, j);
        arguments.push_back(get_expr(argNode, sourceCode));
    }
    return exprFactory.mk_new(exprFactory.mk_constructor_call(type, arguments));
}

astfri::IfExpr* ExpressionTransformer::transform_ternary_expr_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* cond
        = this->get_expr(ts_node_named_child(tsNode, 0), sourceCode);
    astfri::Expr* iftrue
        = this->get_expr(ts_node_named_child(tsNode, 1), sourceCode);
    astfri::Expr* iffalse
        = this->get_expr(ts_node_named_child(tsNode, 2), sourceCode);
    return exprFactory.mk_if(cond, iftrue, iffalse);
}