#include "NodeGetter.hpp"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <tree_sitter/api.h>
#include <vector>

#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/Type.hpp"

NodeGetter::NodeGetter(TSTree* tree, std::string const& sourceCode) :
    typeFactory(astfri::TypeFactory::get_instance()),
    exprFactory(astfri::ExprFactory::get_instance()),
    stmtFactory(astfri::StmtFactory::get_instance()),
    nodeMapper(new NodeMapper())
{
    this->classes = get_classes(tree, sourceCode);
}

TSQuery* NodeGetter::make_query(char const* queryString)
{
    uint32_t errorOffset;
    TSQueryError errorType;

    TSQuery* tsQuery = ts_query_new(
        tree_sitter_java(),
        queryString,
        strlen(queryString),
        &errorOffset,
        &errorType
    );

    return tsQuery;
}

std::string NodeGetter::get_node_text(
    TSNode const& node,
    std::string const& sourceCode
)
{
    uint32_t start = ts_node_start_byte(node);
    uint32_t end   = ts_node_end_byte(node);
    return sourceCode.substr(start, end - start);
}

astfri::Expr* NodeGetter::get_expr(TSNode tsNode, std::string const& sourceCode)
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
        auto refExprVariant = this->get_ref_expr(tsNode, sourceCode);
        std::visit([&expr](auto&& arg) { expr = arg; }, refExprVariant);
    }
    else if (nodeName.find("(binary_expression") == 0)
    {
        expr = this->get_bin_op_expr(tsNode, sourceCode);
    }
    else if (nodeName.find("(assignment_expression") == 0)
    {
        expr = this->get_bin_op_expr(tsNode, sourceCode);
    }
    else if (nodeName.find("(unary_expression") == 0)
    {
        expr = this->get_un_op_expr("unExpr", tsNode, sourceCode);
    }
    else if (nodeName.find("(update_expression") == 0)
    {
        expr = this->get_un_op_expr("updateExpr", tsNode, sourceCode);
    }
    else if (nodeName.find("(object_creation_expression") == 0)
    {
        expr = this->get_new_expr(tsNode, sourceCode);
    }
    else if (nodeName.find("(method_invocation ") == 0)
    {
        expr = this->get_method_call(tsNode, sourceCode);
    }
    else if (nodeName.find("(parenthesized_expression ") == 0)
    {
        expr = this->get_expr(ts_node_named_child(tsNode, 0), sourceCode);
    }
    else
    {
        expr = exprFactory.mk_unknown();
    }
    return expr;
}

astfri::BinOpExpr* NodeGetter::get_bin_op_expr(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* leftExpr  = nullptr;
    astfri::BinOpType binOp = astfri::BinOpType::Add;
    astfri::Expr* rightExpr = nullptr;

    TSNode leftOperand;
    TSNode binOperator;
    TSNode rightOperand;

    if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        leftOperand                 = ts_node_named_child(tsNode, 0);
        std::string leftOperandName = ts_node_string(leftOperand);
        leftExpr                    = this->get_expr(leftOperand, sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        rightOperand                 = ts_node_named_child(tsNode, 1);
        std::string rightOperandName = ts_node_string(rightOperand);
        rightExpr                    = this->get_expr(rightOperand, sourceCode);
    }
    if (! ts_node_is_null(ts_node_child(tsNode, 1)))
    {
        binOperator                 = ts_node_child(tsNode, 1);
        std::string binOperatorName = get_node_text(binOperator, sourceCode);
        binOp = this->nodeMapper->get_binOpMap().find(binOperatorName)->second;
    }

    return exprFactory.mk_bin_on(leftExpr, binOp, rightExpr);
}

astfri::UnaryOpExpr* NodeGetter::get_un_op_expr(
    std::string qString,
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::UnaryOpExpr* unOpExpr;

    char const* queryStringUnExpr
        = "(unary_expression operator: _ @operator operand: (_) @operand)";

    char const* queryStringUpdateExpr
        = "(update_expression _? @operator (identifier) @operand _? @operator)";

    TSQuery* tsQuery;
    if (qString == "unExpr")
    {
        tsQuery = make_query(queryStringUnExpr);
    }
    else if (qString == "updateExpr")
    {
        tsQuery = make_query(queryStringUpdateExpr);
    }

    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, tsNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        astfri::Expr* operand;
        astfri::UnaryOpType unOpType;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeName(ts_node_string(tsCapture.node));
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

            if (captureName == "operand")
            {
                operand = this->get_expr(tsCapture.node, sourceCode);
            }
            else if (captureName == "operator")
            {
                unOpType
                    = this->nodeMapper->get_unaryOpMap().find(nodeText)->second;
            }
        }

        unOpExpr = exprFactory.mk_unary_op(unOpType, operand);
    }

    return unOpExpr;
}

std::variant<
    astfri::ParamVarRefExpr*,
    astfri::LocalVarRefExpr*,
    astfri::MemberVarRefExpr*>
    NodeGetter::get_ref_expr(TSNode tsNode, std::string const& sourceCode)
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
                            if (get_node_text(grandChildNode, sourceCode)
                                == referenceName)
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
    return exprFactory.mk_local_var_ref("??????");
}

astfri::MethodCallExpr* NodeGetter::get_method_call(
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

astfri::NewExpr* NodeGetter::get_new_expr(
    TSNode paramsNode,
    std::string const& sourceCode
)
{
    astfri::NewExpr* newExpr = nullptr;

    char const* queryString  = "(object_creation_expression type: (_) @type "
                               "arguments: (argument_list) @args)";

    TSQuery* tsQuery         = make_query(queryString);
    TSQueryCursor* tsCursor  = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, paramsNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        astfri::Type* type = typeFactory.mk_unknown();
        std::vector<astfri::Expr*> arguments;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeName(ts_node_string(tsCapture.node));
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

            if (captureName == "type")
            {
                if (nodeName == "(type_identifier)")
                {
                    type = typeFactory.mk_user(nodeText);
                }
                else
                {
                    type = this->nodeMapper->get_typeMap()
                               .find(nodeText)
                               ->second;
                }
            }
            else if (captureName == "args")
            {
                for (uint32_t j = 0; j < ts_node_child_count(tsCapture.node);
                     ++j)
                {
                    arguments.push_back(
                        get_expr(ts_node_child(tsCapture.node, j), sourceCode)
                    );
                }
            }
        }

        newExpr = exprFactory.mk_new(
            exprFactory.mk_constructor_call(type, arguments)
        );
    }

    return newExpr;
}

std::vector<astfri::ParamVarDefStmt*> NodeGetter::get_params(
    TSNode paramsNode,
    std::string const& sourceCode
)
{
    std::vector<astfri::ParamVarDefStmt*> params;

    char const* queryString = "(formal_parameter type: _ @param_type name: "
                              "(identifier) @param_name)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, paramsNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        astfri::Type* paramType = typeFactory.mk_unknown();
        std::string paramName;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeName(ts_node_string(tsCapture.node));
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

            if (captureName == "param_type")
            {
                if (nodeName == "(type_identifier)")
                {
                    paramType = typeFactory.mk_user(nodeText);
                }
                else
                {
                    paramType = this->nodeMapper->get_typeMap()
                                    .find(nodeText)
                                    ->second;
                }
            }
            else if (captureName == "param_name")
            {
                paramName = nodeText;
            }
        }

        params.push_back(
            stmtFactory.mk_param_var_def(paramName, paramType, nullptr)
        );
    }

    ts_query_cursor_delete(tsCursor);
    ts_query_delete(tsQuery);
    return params;
}

astfri::LocalVarDefStmt* NodeGetter::get_local_var(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Type* type = typeFactory.mk_unknown();
    std::string name;
    astfri::Expr* init;

    std::string tsNodeName = ts_node_string(tsNode);
    uint32_t childCount    = ts_node_named_child_count(tsNode);
    for (uint32_t j = 0; j < childCount; j++)
    {
        TSNode child          = ts_node_named_child(tsNode, j);
        std::string childName = ts_node_string(child);

        if (childName == "(identifier)"
            || childName.find("type") != std::string::npos)
        {
            if (childName == "(identifier)")
            {
                type = typeFactory.mk_user(get_node_text(child, sourceCode));
            }
            else
            {
                type = this->nodeMapper->get_typeMap()
                           .find(get_node_text(child, sourceCode))
                           ->second;
            }
        }
        else if (childName.find("(variable_declarator") == 0)
        {
            uint32_t greatGrandChildCount = ts_node_named_child_count(child);
            for (uint32_t k = 0; k < greatGrandChildCount; k++)
            {
                TSNode greatGrandChild = ts_node_named_child(child, k);
                std::string greatGrandChildName
                    = ts_node_string(greatGrandChild);

                if (greatGrandChildName == "(identifier)" && k == 0)
                {
                    name = get_node_text(greatGrandChild, sourceCode);
                }
                else
                {
                    init = this->get_expr(greatGrandChild, sourceCode);
                }
            }
        }
    }
    return stmtFactory.mk_local_var_def(name, type, init);
}

astfri::ExprStmt* NodeGetter::get_expr_stmt(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* expr;

    TSNode child          = ts_node_named_child(tsNode, 0);
    std::string childName = ts_node_string(child);
    expr                  = this->get_expr(child, sourceCode);

    return stmtFactory.mk_expr(expr);
}

astfri::IfStmt* NodeGetter::get_if_stmt(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* condition = nullptr;
    astfri::Stmt* iftrue    = nullptr;
    astfri::Stmt* iffalse   = nullptr;

    uint32_t childCount     = ts_node_named_child_count(tsNode);
    for (uint32_t j = 0; j < childCount; j++)
    {
        TSNode child          = ts_node_named_child(tsNode, j);
        std::string childName = ts_node_string(child);
        TSNode conditionNode  = ts_node_named_child(child, 0);
        TSNode iftrueNode;
        TSNode iffalseNode;
        if (childName.find("(parenthesized_expression ") == 0)
        {
            std::string conditionNodeName = ts_node_string(conditionNode);
            condition = this->get_expr(conditionNode, sourceCode);
        }
        else if ((childName.find("(block ") == 0 && j == 1))
        {
            iftrueNode                 = child;
            std::string iftrueNodeName = ts_node_string(iftrueNode);
            iftrue = this->search_sub_tree(iftrueNode, sourceCode);
        }
        else if (childName.find("(block ") == 0 && j == 2)
        {
            iffalseNode                 = child;
            std::string iffalseNodeName = ts_node_string(iffalseNode);
            iffalse = this->search_sub_tree(iffalseNode, sourceCode);
        }
        else if (childName.find("(if_statement ") == 0)
        {
            if (j == 1)
            {
                iftrueNode                 = child;
                std::string iftrueNodeName = ts_node_string(iftrueNode);
                iftrue = this->get_if_stmt(iftrueNode, sourceCode);
            }
            else
            {
                iffalseNode                 = child;
                std::string iffalseNodeName = ts_node_string(iffalseNode);
                iffalse = this->get_if_stmt(iffalseNode, sourceCode);
            }
        }
    }

    return stmtFactory.mk_if(condition, iftrue, iffalse);
}

astfri::ForStmt* NodeGetter::get_for_stmt(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Stmt* init      = nullptr;
    astfri::Expr* condition = nullptr;
    astfri::Stmt* step      = nullptr;
    astfri::Stmt* body      = nullptr;

    TSNode initNode;
    TSNode conditionNode;
    TSNode stepNode;
    TSNode bodyNode;

    if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        initNode                 = ts_node_named_child(tsNode, 0);
        std::string initNodeName = ts_node_string(initNode);
        init                     = this->get_local_var(initNode, sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        conditionNode                 = ts_node_named_child(tsNode, 1);
        std::string conditionNodeName = ts_node_string(conditionNode);
        condition = this->get_expr(conditionNode, sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 2)))
    {
        stepNode                 = ts_node_named_child(tsNode, 2);
        std::string stepNodeName = ts_node_string(stepNode);
        astfri::Expr* expr       = this->get_expr(stepNode, sourceCode);
        step                     = stmtFactory.mk_expr(expr);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 3)))
    {
        bodyNode                 = ts_node_named_child(tsNode, 3);
        std::string bodyNodeName = ts_node_string(bodyNode);
        body                     = this->search_sub_tree(bodyNode, sourceCode);
    }

    return stmtFactory.mk_for(init, condition, step, body);
}

astfri::WhileStmt* NodeGetter::get_while_stmt(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* condition    = nullptr;
    astfri::CompoundStmt* body = nullptr;

    TSNode helpNode;
    TSNode conditionNode;
    TSNode bodyNode;

    if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        helpNode = ts_node_named_child(tsNode, 0);
        if (! ts_node_is_null(ts_node_named_child(helpNode, 0)))
        {
            conditionNode                 = ts_node_named_child(helpNode, 0);
            std::string conditionNodeName = ts_node_string(conditionNode);
            condition = this->get_expr(conditionNode, sourceCode);
        }
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        bodyNode                 = ts_node_named_child(tsNode, 1);
        std::string bodyNodeName = ts_node_string(bodyNode);
        body                     = this->search_sub_tree(bodyNode, sourceCode);
    }

    return stmtFactory.mk_while(condition, body);
}

astfri::DoWhileStmt* NodeGetter::get_do_while_stmt(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* condition    = nullptr;
    astfri::CompoundStmt* body = nullptr;

    TSNode helpNode;
    TSNode conditionNode;
    TSNode bodyNode;

    if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        bodyNode                 = ts_node_named_child(tsNode, 0);
        std::string bodyNodeName = ts_node_string(bodyNode);
        body                     = this->search_sub_tree(bodyNode, sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        helpNode = ts_node_named_child(tsNode, 1);
        if (! ts_node_is_null(ts_node_named_child(helpNode, 0)))
        {
            conditionNode                 = ts_node_named_child(helpNode, 0);
            std::string conditionNodeName = ts_node_string(conditionNode);
            condition = this->get_expr(conditionNode, sourceCode);
        }
    }

    return stmtFactory.mk_do_while(condition, body);
}

astfri::ReturnStmt* NodeGetter::get_return_stmt(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* expr;

    TSNode child          = ts_node_named_child(tsNode, 0);
    std::string childName = ts_node_string(child);
    expr                  = this->get_expr(child, sourceCode);

    return stmtFactory.mk_return(expr);
}

astfri::CompoundStmt* NodeGetter::search_sub_tree(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    std::vector<astfri::Stmt*> statements = {};
    uint32_t tsNodeChildeCount            = ts_node_named_child_count(tsNode);

    for (uint32_t i = 0; i < tsNodeChildeCount; i++)
    {
        TSNode child          = ts_node_named_child(tsNode, i);
        std::string childName = ts_node_string(child);
        if (childName.find("(local_variable_declaration ") == 0)
        {
            statements.push_back(this->get_local_var(child, sourceCode));
        }
        else if (childName.find("(expression_statement ") == 0)
        {
            statements.push_back(this->get_expr_stmt(child, sourceCode));
        }
        else if (childName.find("(if_statement ") == 0)
        {
            statements.push_back(this->get_if_stmt(child, sourceCode));
        }
        //else if (childName.find("(switch_expression ") == 0)
        //{
        //    statements.push_back(this->get_switch_stmt(child, sourceCode));
        //}
        else if (childName.find("(for_statement ") == 0)
        {
            statements.push_back(this->get_for_stmt(child, sourceCode));
        }
        else if (childName.find("(while_statement ") == 0)
        {
            statements.push_back(this->get_while_stmt(child, sourceCode));
        }
        else if (childName.find("(do_statement ") == 0)
        {
            statements.push_back(this->get_do_while_stmt(child, sourceCode));
        }
        else if (childName.find("(return_statement ") == 0)
        {
            statements.push_back(this->get_return_stmt(child, sourceCode));
        }
    }
    return stmtFactory.mk_compound(statements);
}

astfri::ConstructorDefStmt* NodeGetter::get_constructor(
    TSNode classNode,
    std::string const& sourceCode
)
{
    astfri::ConstructorDefStmt* constructorDef;

    char const* queryString
        = "(constructor_declaration (modifiers)? @constructor_modifiers "
          "name: (identifier) @constructor_name (formal_parameter)? @params "
          "body: (_) @constructor_body)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, classNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        std::vector<std::string> constructorModifiers;
        std::string constructorName;
        TSNode paramsNode;
        TSNode constructorNode;
        std::vector<astfri::ParamVarDefStmt*> params;
        std::vector<astfri::LocalVarDefStmt*> vars;
        astfri::AccessModifier access = astfri::AccessModifier::Internal;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeName(ts_node_string(tsCapture.node));
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

            if (captureName == "constructor_modifiers")
            {
                constructorModifiers.push_back(nodeText);
            }
            else if (captureName == "constructor_name")
            {
                constructorName = nodeText;
            }
            else if (captureName == "params")
            {
                paramsNode = tsCapture.node;
            }
            else if (captureName == "constructor_body")
            {
                constructorNode = tsCapture.node;
            }
        }

        if (! ts_node_is_null(paramsNode))
        {
            params = get_params(paramsNode, sourceCode);
        }
        if (! ts_node_is_null(constructorNode))
        {
            // vars = get_local_vars(constructorNode, sourceCode);
        }

        for (std::string mod : constructorModifiers)
        {
            if (mod == "public")
            {
                access = astfri::AccessModifier::Public;
            }
            else if (mod == "private")
            {
                access = astfri::AccessModifier::Private;
            }
            else if (mod == "internal")
            {
                access = astfri::AccessModifier::Internal;
            }
            else if (mod == "protected")
            {
                access = astfri::AccessModifier::Protected;
            }
        }

        constructorDef
            = stmtFactory
                  .mk_constructor_def(nullptr, params, {}, nullptr, access);
    }

    ts_query_cursor_delete(tsCursor);
    ts_query_delete(tsQuery);
    return constructorDef;
}

astfri::MethodDefStmt* NodeGetter::get_method(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    TSNode modifierNode;
    TSNode typeNode;
    TSNode nameNode;
    TSNode parameterNode;
    TSNode bodyNode;

    astfri::AccessModifier access = astfri::AccessModifier::Internal;
    astfri::Type* type            = nullptr;
    std::string name;
    std::vector<astfri::ParamVarDefStmt*> params;
    astfri::CompoundStmt* body = nullptr;

    if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        modifierNode        = ts_node_named_child(tsNode, 0);
        uint32_t childCount = ts_node_child_count(modifierNode);
        for (uint32_t i = 0; i < childCount; i++)
        {
            TSNode child          = ts_node_child(modifierNode, i);
            std::string childName = get_node_text(child, sourceCode);
            if (childName == "private" || childName == "public"
                || childName == "internal" || childName == "protected")
            {
                access = this->nodeMapper->get_modMap().find(childName)->second;
            }
        }
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        typeNode                 = ts_node_named_child(tsNode, 1);
        std::string typeNodeName = ts_node_string(typeNode);
        std::string typeNodeText = get_node_text(typeNode, sourceCode);

        if (typeNodeName == "(identifier)")
        {
            type = typeFactory.mk_user(typeNodeText);
        }
        else
        {
            type = this->nodeMapper->get_typeMap().find(typeNodeText)->second;
        }
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 2)))
    {
        nameNode = ts_node_named_child(tsNode, 2);
        name     = get_node_text(nameNode, sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 3)))
    {
        parameterNode                 = ts_node_named_child(tsNode, 3);
        std::string parameterNodeName = ts_node_string(parameterNode);
        params = this->get_params(parameterNode, sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 4)))
    {
        bodyNode                 = ts_node_named_child(tsNode, 4);
        std::string bodyNodeName = ts_node_string(parameterNode);
        body                     = this->search_sub_tree(bodyNode, sourceCode);
    }

    astfri::FunctionDefStmt* func
        = stmtFactory.mk_function_def(name, params, type, body);

    return stmtFactory
        .mk_method_def(nullptr, func, access, astfri::Virtuality::NotVirtual);
}

astfri::MemberVarDefStmt* NodeGetter::get_attribute(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    TSNode modifierNode;
    TSNode typeNode;
    TSNode declaratorNode;

    astfri::AccessModifier access = astfri::AccessModifier::Internal;
    astfri::Type* type            = nullptr;
    std::string name;
    astfri::Expr* init = nullptr;

    if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        modifierNode        = ts_node_named_child(tsNode, 0);
        uint32_t childCount = ts_node_child_count(modifierNode);
        for (uint32_t i = 0; i < childCount; i++)
        {
            TSNode child          = ts_node_child(modifierNode, i);
            std::string childName = get_node_text(child, sourceCode);
            if (childName == "private" || childName == "public"
                || childName == "internal" || childName == "protected")
            {
                access = this->nodeMapper->get_modMap().find(childName)->second;
            }
        }
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        typeNode                 = ts_node_named_child(tsNode, 1);
        std::string typeNodeName = ts_node_string(typeNode);
        std::string typeNodeText = get_node_text(typeNode, sourceCode);

        if (typeNodeName == "(identifier)")
        {
            type = typeFactory.mk_user(typeNodeText);
        }
        else
        {
            type = this->nodeMapper->get_typeMap().find(typeNodeText)->second;
        }
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 2)))
    {
        declaratorNode = ts_node_named_child(tsNode, 2);
        TSNode attributeNameNode;
        TSNode attributeInitNode;

        if (! ts_node_is_null(ts_node_named_child(declaratorNode, 0)))
        {
            attributeNameNode = ts_node_named_child(declaratorNode, 0);
            name              = get_node_text(attributeNameNode, sourceCode);
        }
        if (! ts_node_is_null(ts_node_named_child(declaratorNode, 1)))
        {
            attributeInitNode = ts_node_named_child(declaratorNode, 1);
            init              = this->get_expr(attributeInitNode, sourceCode);
        }
    }

    return stmtFactory.mk_member_var_def(name, type, init, access);
}

std::vector<astfri::ClassDefStmt*> NodeGetter::get_classes(
    TSTree* tree,
    std::string const& sourceCode
)
{
    TSNode rootNode = ts_tree_root_node(tree);
    TSNode classNode;
    TSNode classNameNode;
    TSNode classBodyNode;
    std::string className;
    std::vector<astfri::MemberVarDefStmt*> attributes;
    std::vector<astfri::MethodDefStmt*> methods;
    std::vector<astfri::ClassDefStmt*> classes;

    uint32_t childCount = ts_node_named_child_count(rootNode);
    for (uint32_t i = 0; i < childCount; i++)
    {
        if (! ts_node_is_null(ts_node_named_child(rootNode, i)))
        {
            classNode = ts_node_named_child(rootNode, i);
        }

        if (! ts_node_is_null(ts_node_named_child(classNode, 1)))
        {
            classNameNode = ts_node_named_child(classNode, 1);
            className     = get_node_text(classNameNode, sourceCode);
        }

        if (! ts_node_is_null(ts_node_named_child(classNode, 2)))
        {
            classBodyNode = ts_node_named_child(classNode, 2);
        }

        uint32_t grandChildCount = ts_node_named_child_count(classBodyNode);
        for (uint32_t i = 0; i < grandChildCount; i++)
        {
            TSNode grandChild          = ts_node_named_child(classBodyNode, i);
            std::string grandChildName = ts_node_string(grandChild);

            if (grandChildName.find("(field_declaration ") == 0)
            {
                attributes.push_back(this->get_attribute(grandChild, sourceCode)
                );
            }
            else if (grandChildName.find("(method_declaration ") == 0)
            {
                methods.push_back(this->get_method(grandChild, sourceCode));
            }
        }

        astfri::ClassDefStmt* classDef = stmtFactory.mk_class_def(className);
        classDef->vars_                = attributes;
        classDef->methods_             = methods;
        classes.push_back(classDef);

        for (astfri::MethodDefStmt* method : methods)
        {
            method->owner_ = classDef;
        }
        attributes.clear();
        methods.clear();
    }

    return classes;
}

std::vector<astfri::ClassDefStmt*> NodeGetter::get_classes()
{
    return this->classes;
}