#include "ExpressionTransformer.hpp"

#include <ranges>

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
    std::string nodeName = ts_node_string(tsNode);
    std::string nodeText = get_node_text(tsNode, sourceCode);
    astfri::Expr* expr;
    if (nodeName == ("(decimal_integer_literal)"))
    {
        expr = exprFactory.mk_int_literal(atoi(nodeText.c_str()));
    }
    else if (nodeName == "(decimal_floating_point_literal)")
    {
        expr = exprFactory.mk_float_literal(atof(nodeText.c_str()));
    }
    else if (nodeName == "(character_literal)")
    {
        expr = exprFactory.mk_char_literal(nodeText[0]);
    }
    else if (nodeName.find("(string_literal ") == 0
             || nodeName == "(string_literal)")
    {
        expr = exprFactory.mk_string_literal(nodeText);
    }
    else if (nodeName == "(true)")
    {
        expr = exprFactory.mk_bool_literal(true);
    }
    else if (nodeName == "(false)")
    {
        expr = exprFactory.mk_bool_literal(false);
    }
    else if (nodeName == "(null_literal)")
    {
        expr = exprFactory.mk_null_literal();
    }
    else if (nodeName == "(this)")
    {
        expr = exprFactory.mk_this();
    }
    else if (nodeName.find("(field_access ") == 0)
    {
        TSNode object;
        std::string objectName;
        if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
        {
            object     = ts_node_named_child(tsNode, 0);
            objectName = ts_node_string(object);
        }
        if (objectName == "(this)")
        {
            expr = exprFactory.mk_this();
        }
        else
        {
            expr = exprFactory.mk_class_ref(get_node_text(object, sourceCode));
        }
    }
    else if (nodeName == "(identifier)")
    {
        auto refExprVariant = this->transform_ref_expr_node(tsNode, sourceCode);
        std::visit([&expr](auto&& arg) { expr = arg; }, refExprVariant);
    }
    else if (nodeName.find("(binary_expression") == 0)
    {
        expr = this->transform_bin_op_expr_node(tsNode, sourceCode);
    }
    else if (nodeName.find("(assignment_expression") == 0)
    {
        expr = this->transform_bin_op_expr_node(tsNode, sourceCode);
    }
    else if (nodeName.find("(unary_expression") == 0)
    {
        expr = this->transform_un_op_expr_node(tsNode, sourceCode);
    }
    else if (nodeName.find("(update_expression") == 0)
    {
        expr = this->transform_un_op_expr_node(tsNode, sourceCode);
    }
    else if (nodeName.find("(object_creation_expression") == 0)
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
        expr = exprFactory.mk_new(
            exprFactory.mk_constructor_call(type, arguments)
        );
    }
    else if (nodeName.find("(method_invocation") == 0)
    {
        expr = this->transform_method_call_node(tsNode, sourceCode);
    }
    else if (nodeName.find("(parenthesized_expression") == 0)
    {
        expr = this->get_expr(ts_node_named_child(tsNode, 0), sourceCode);
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
    std::string referenceName = get_node_text(tsNode, sourceCode);
    TSNode currentNode        = tsNode;

    while (std::string_view(ts_node_string(currentNode)).find("program")
           == std::string::npos)
    {
        TSNode parentNode = ts_node_parent(currentNode);

        // variable just for debugging
        std::string debugParentNode = ts_node_string(parentNode);

        if (std::string_view(ts_node_string(parentNode))
                .find("formal_parameters")
            != std::string::npos)
        {
            uint32_t childCount = ts_node_child_count(parentNode);
            for (uint32_t i = 0; i < childCount; i++)
            {
                TSNode childNode = ts_node_child(parentNode, i);
                if (std::string_view(ts_node_string(childNode))
                        .find("formal_parameter")
                    != std::string::npos)
                {
                    uint32_t grandChildCount = ts_node_child_count(childNode);
                    for (uint32_t j = 0; j < grandChildCount; j++)
                    {
                        TSNode grandChildNode = ts_node_child(childNode, j);
                        if (std::string_view(ts_node_string(grandChildNode))
                                .find("identifier")
                            != std::string::npos)
                        {
                            std::string grandChildNodeName
                                = ts_node_string(grandChildNode);
                            std::string grandChildNodeText
                                = get_node_text(grandChildNode, sourceCode);
                            if (grandChildNodeText == referenceName)
                            {
                                return exprFactory.mk_param_var_ref(
                                    referenceName
                                );
                            }
                        }
                    }
                }
            }
        }

        if (std::string_view(ts_node_string(parentNode)).find("block")
            != std::string::npos)
        {
            uint32_t childCount = ts_node_child_count(parentNode);
            for (uint32_t i = 0; i < childCount; i++)
            {
                TSNode childNode = ts_node_child(parentNode, i);
                if (std::string_view(ts_node_string(childNode))
                        .find("local_variable_declaration")
                    != std::string::npos)
                {
                    uint32_t grandChildCount = ts_node_child_count(childNode);
                    for (uint32_t j = 0; j < grandChildCount; j++)
                    {
                        TSNode grandChildNode = ts_node_child(childNode, j);
                        if (std::string_view(ts_node_string(grandChildNode))
                                .find("variable_declarator")
                            != std::string::npos)
                        {
                            uint32_t greatGrandChildCount
                                = ts_node_child_count(grandChildNode);
                            for (uint32_t k = 0; k < greatGrandChildCount; k++)
                            {
                                TSNode greatGrandChildNode
                                    = ts_node_child(grandChildNode, k);
                                if (std::string_view(
                                        ts_node_string(greatGrandChildNode)
                                    )
                                        .find("identifier")
                                    != std::string::npos)
                                {
                                    if (get_node_text(
                                            greatGrandChildNode,
                                            sourceCode
                                        )
                                        == referenceName)
                                    {
                                        return exprFactory.mk_local_var_ref(
                                            referenceName
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (std::string_view(ts_node_string(parentNode)).find("class_body")
            != std::string::npos)
        {
            uint32_t childCount = ts_node_child_count(parentNode);
            for (uint32_t i = 0; i < childCount; i++)
            {
                TSNode childNode = ts_node_child(parentNode, i);
                if (std::string_view(ts_node_string(childNode))
                        .find("field_declaration")
                    != std::string::npos)
                {
                    uint32_t grandChildCount = ts_node_child_count(childNode);
                    for (uint32_t j = 0; j < grandChildCount; j++)
                    {
                        TSNode grandChildNode = ts_node_child(childNode, j);
                        if (std::string_view(ts_node_string(grandChildNode))
                                .find("variable_declarator")
                            != std::string::npos)
                        {
                            uint32_t greatGrandChildCount
                                = ts_node_child_count(grandChildNode);
                            for (uint32_t k = 0; k < greatGrandChildCount; k++)
                            {
                                TSNode greatGrandChildNode
                                    = ts_node_child(grandChildNode, k);
                                if (std::string_view(
                                        ts_node_string(greatGrandChildNode)
                                    )
                                        .find("identifier")
                                    != std::string::npos)
                                {
                                    if (get_node_text(
                                            greatGrandChildNode,
                                            sourceCode
                                        )
                                        == referenceName)
                                    {
                                        return exprFactory.mk_member_var_ref(
                                            nullptr,
                                            referenceName
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        currentNode = parentNode;
    }
    return exprFactory.mk_string_literal("?????");
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

    for (uint32_t i = 0; i < childCount; i++)
    {
        TSNode child          = ts_node_named_child(tsNode, i);
        std::string childName = ts_node_string(child);

        if (childName.find("(field_access ") == 0 || childName == "(this)")
        {
            owner = this->get_expr(child, sourceCode);
        }
        else if (childName == ("(identifier)"))
        {
            name = get_node_text(child, sourceCode);
        }
        else if (childName.find("(argument_list ") == 0)
        {
            uint32_t argCount = ts_node_named_child_count(child);
            for (uint32_t j = 0; j < argCount; j++)
            {
                TSNode argNode          = ts_node_named_child(child, j);
                std::string argNodeName = ts_node_string(argNode);
                arguments.push_back(get_expr(argNode, sourceCode));
            }
        }
    }

    return exprFactory.mk_method_call(owner, name, arguments);
}