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
                            std::string grandChildNodeName = ts_node_string(grandChildNode);
                            std::string grandChildNodeText = get_node_text(grandChildNode, sourceCode);
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
                if (nodeName == "(identifier)")
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

astfri::SwitchStmt* NodeGetter::get_switch_stmt(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    TSNode conditionNode;
    TSNode switchBodyNode;

    astfri::Expr* condition = nullptr;
    std::vector<astfri::CaseBaseStmt*> cases;

    if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        conditionNode = ts_node_named_child(tsNode, 0);
        condition = this->get_expr(conditionNode, sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        switchBodyNode = ts_node_named_child(tsNode, 1);
        uint32_t caseCount = ts_node_named_child_count(switchBodyNode);
        
        for (uint32_t i = 0; i < caseCount; i++)
        {
            TSNode caseNode = ts_node_named_child(switchBodyNode, i);
            uint32_t switchCaseChildren = ts_node_named_child_count(caseNode);
            astfri::Expr* expr = nullptr;
            std::vector<astfri::Stmt*> stmts;
            bool isDefaultCase = false;

            for (uint32_t j = 0; j < switchCaseChildren; j++)
            {
                TSNode child = ts_node_named_child(caseNode, j);
                std::string childText = get_node_text(child, sourceCode);

                if (childText.find("case") == 0)
                {
                    TSNode caseChild = ts_node_named_child(child, 0);
                    expr = this->get_expr(caseChild, sourceCode);
                }
                else if (childText.find("default") == 0)
                {
                    isDefaultCase = true;
                }
                else
                {
                    stmts.push_back(this->get_stmt(child, sourceCode));
                }
            }

            if (isDefaultCase)
            {
                cases.push_back(stmtFactory.mk_default_case(stmtFactory.mk_compound(stmts)));
            }
            else
            {
                cases.push_back(stmtFactory.mk_case(expr, stmtFactory.mk_compound(stmts)));
            }
        }
    }
    return stmtFactory.mk_switch(condition, cases);
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

astfri::Stmt* NodeGetter::get_stmt(TSNode tsNode, std::string const& sourceCode)
{
    std::string nodeName = ts_node_string(tsNode);
    if (nodeName.find("(local_variable_declaration ") == 0)
    {
        return this->get_local_var(tsNode, sourceCode);
    }
    else if (nodeName.find("(expression_statement ") == 0)
    {
        return this->get_expr_stmt(tsNode, sourceCode);
    }
    else if (nodeName.find("(if_statement ") == 0)
    {
        return this->get_if_stmt(tsNode, sourceCode);
    }
    else if (nodeName.find("(switch_expression ") == 0)
    {
        return this->get_switch_stmt(tsNode, sourceCode);
    }
    else if (nodeName.find("(for_statement ") == 0)
    {
        return this->get_for_stmt(tsNode, sourceCode);
    }
    else if (nodeName.find("(while_statement ") == 0)
    {
        return this->get_while_stmt(tsNode, sourceCode);
    }
    else if (nodeName.find("(do_statement ") == 0)
    {
        return this->get_do_while_stmt(tsNode, sourceCode);
    }
    else if (nodeName.find("(return_statement ") == 0)
    {
        return this->get_return_stmt(tsNode, sourceCode);
    }
    else if (nodeName.find("(break_statement ") == 0)
    {
        return stmtFactory.mk_break();
    }
    else if (nodeName.find("(continue_statement ") == 0)
    {
        return stmtFactory.mk_continue();
    }
    else if (nodeName.find("(explicit_constructor_invocation ") == 0)
    {
        std::vector<astfri::Expr*> args;
        TSNode argumentsListNode = ts_node_named_child(tsNode, 1);
        uint32_t argsCount = ts_node_named_child_count(argumentsListNode);
        for (uint32_t i = 0; i < argsCount; i++)
        {
            TSNode argNode = ts_node_named_child(argumentsListNode, i);
            std::string argNodeName = ts_node_string(argNode);
            args.push_back(this->get_expr(argNode, sourceCode));
        }
        return stmtFactory.mak_base_initializer(get_node_text(ts_node_named_child(tsNode, 0), sourceCode), args);
    }
    else
    {
        return stmtFactory.mk_uknown();
    }
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
        statements.push_back(this->get_stmt(child, sourceCode));
    }
    return stmtFactory.mk_compound(statements);
}

astfri::ConstructorDefStmt* NodeGetter::get_constructor(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::AccessModifier access = astfri::AccessModifier::Internal;
    std::string name;
    std::vector<astfri::ParamVarDefStmt*> params;
    std::vector<astfri::BaseInitializerStmt*> baseInit;
    astfri::CompoundStmt* body = nullptr;

    uint32_t methodChildCount = ts_node_named_child_count(tsNode);
    for (uint32_t i = 0; i < methodChildCount; i++)
    {
        TSNode methodChild = ts_node_named_child(tsNode, i);
        std::string methodChildName = ts_node_string(methodChild);

        if (methodChildName == "(modifiers)")
        {
            uint32_t modifiersCount = ts_node_child_count(methodChild);
            for (uint32_t j = 0; j < modifiersCount; j++)
            {
                TSNode modifierNode = ts_node_child(methodChild, j);
                std::string modifierNodeName = get_node_text(modifierNode, sourceCode);
                if (modifierNodeName == "private" || modifierNodeName == "public"
                    || modifierNodeName == "internal" || modifierNodeName == "protected")
                {
                    access = this->nodeMapper->get_modMap().find(modifierNodeName)->second;
                }
            }
        }
        else if (methodChildName == "(identifier)")
        {
            name = get_node_text(methodChild, sourceCode);
        }
        else if (methodChildName.find("(formal_parameters ") == 0)
        {
            params = this->get_params(methodChild, sourceCode);
        }
        else if (methodChildName.find("(constructor_body ") == 0)
        {
            uint32_t bodyChildCount = ts_node_named_child_count(methodChild);
            std::vector<astfri::Stmt*> bodyStatements;

            for (uint32_t j = 0; j < bodyChildCount; j++)
            {
                TSNode bodyChild = ts_node_named_child(methodChild, j);
                astfri::Stmt* stmt = this->get_stmt(bodyChild, sourceCode);

                if (auto baseInitStmt = dynamic_cast<astfri::BaseInitializerStmt*>(stmt))
                {
                    baseInit.push_back(baseInitStmt);
                }
                else
                {
                    bodyStatements.push_back(stmt);
                }
            }
            body = stmtFactory.mk_compound(bodyStatements);
        }
    }

    return stmtFactory
        .mk_constructor_def(nullptr, params, baseInit, body, access);
}

astfri::MethodDefStmt* NodeGetter::get_method(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Type* type = nullptr;
    std::string name;
    std::vector<astfri::ParamVarDefStmt*> params;
    astfri::CompoundStmt* body = nullptr;
    astfri::AccessModifier access = astfri::AccessModifier::Internal;

    uint32_t methodChildCount = ts_node_named_child_count(tsNode);
    for (uint32_t i = 0; i < methodChildCount; i++)
    {
        TSNode methodChild = ts_node_named_child(tsNode, i);
        std::string methodChildName = ts_node_string(methodChild);

        if (methodChildName == ("(modifiers)"))
        {
            uint32_t modifiersCount = ts_node_child_count(methodChild);
            for (uint32_t j = 0; j < modifiersCount; j++)
            {
                TSNode modifierNode = ts_node_child(methodChild, j);
                std::string modifierNodeName = get_node_text(modifierNode, sourceCode);
                if (modifierNodeName == "private" || modifierNodeName == "public"
                    || modifierNodeName == "internal" || modifierNodeName == "protected")
                {
                    access = this->nodeMapper->get_modMap().find(modifierNodeName)->second;
                }
            }
        }
        else if ((methodChildName.find("type") != std::string::npos || methodChildName.find("identifier") != std::string::npos) && (i == 1))
        {
            if (methodChildName.find("(identifier)") == 0)
            {
                type = typeFactory.mk_user(get_node_text(methodChild, sourceCode));
            }
            else if (methodChildName.find("(generic_type") == 0)
            {
                continue;
            }
            else 
            {
                type = this->nodeMapper->get_typeMap().find(get_node_text(methodChild, sourceCode))->second;
            }
        }
        else if (methodChildName == "(identifier)")
        {
            name = get_node_text(methodChild, sourceCode);
        }
        else if (methodChildName.find("(formal_parameters ") == 0)
        {
            params = this->get_params(methodChild, sourceCode);
        }
        else if (methodChildName.find("(block ") == 0)
        {
            body = this->search_sub_tree(methodChild, sourceCode);
        }
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

astfri::GenericParam* NodeGetter::get_tparam(TSNode tsNode, std::string const& sourceCode)
{
    std::string name;
    std::string constraint;
    if (!ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        name = get_node_text(ts_node_named_child(tsNode, 0), sourceCode);
    }
    if (!ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        constraint = get_node_text(ts_node_named_child(tsNode, 1), sourceCode);
    }

    return stmtFactory.mk_generic_param(constraint, name);
}

std::vector<astfri::ClassDefStmt*> NodeGetter::get_classes(
    TSTree* tree,
    std::string const& sourceCode
)
{
    TSNode rootNode = ts_tree_root_node(tree);
    TSNode classNode;
    std::string className;
    std::vector<astfri::MemberVarDefStmt*> attributes;
    std::vector<astfri::MethodDefStmt*> methods;
    std::vector<astfri::ConstructorDefStmt*> constructors;
    std::vector<astfri::ClassDefStmt*> classes;
    std::vector<astfri::GenericParam*> tparams;
    std::vector<astfri::ClassDefStmt*> bases;
    std::vector<astfri::InterfaceDefStmt*> interfaces;

    uint32_t childCount = ts_node_named_child_count(rootNode);
    for (uint32_t i = 0; i < childCount; i++)
    {
        if (! ts_node_is_null(ts_node_named_child(rootNode, i)))
        {
            TSNode child = ts_node_named_child(rootNode, i);
            std::string childName = ts_node_string(child);
            if (childName.find("(class_declaration ") == 0)
            {
                classNode = child;
            }
            else
            {
                continue;
            }
        }

        uint32_t classChildCount = ts_node_named_child_count(classNode);
        for (uint32_t j = 0; j < classChildCount; j++)
        {
            TSNode classChild = ts_node_named_child(classNode, j);
            std::string classChildName = ts_node_string(classChild);

            if (classChildName.find("(modifiers)") == 0)
            {
                continue;
            }
            else if (classChildName.find("(identifier)") == 0)
            {
                className = get_node_text(classChild, sourceCode);
            }
            else if (classChildName.find("(type_parameters ") == 0)
            {
                uint32_t parametersCount = ts_node_named_child_count(classChild);
                for (uint32_t k = 0; k < parametersCount; k++)
                {
                    TSNode parameterNode = ts_node_named_child(classChild, k);
                    tparams.push_back(this->get_tparam(parameterNode, sourceCode));
                }
            }
            else if (classChildName.find("(superclass ") == 0)
            {
                bases.push_back(stmtFactory.mk_class_def(get_node_text(ts_node_named_child(classChild, 0), sourceCode)));
            }
            else if (classChildName.find("(super_interfaces ") == 0)
            {
                uint32_t typeListChildCount = ts_node_named_child_count(classChild);
                for (uint32_t k = 0; k < typeListChildCount; k++)
                {
                    TSNode typeListChild = ts_node_named_child(classChild, k);
                    interfaces.push_back(stmtFactory.mk_interface_def(get_node_text(typeListChild, sourceCode)));
                }
            }
            else if (classChildName.find("(class_body ") == 0)
            {
                uint32_t classBodyChildCount = ts_node_named_child_count(classChild);
                for (uint32_t k = 0; k < classBodyChildCount; k++)
                {
                    TSNode classBodyChild          = ts_node_named_child(classChild, k);
                    std::string classBodyChildName = ts_node_string(classBodyChild);
        
                    if (classBodyChildName.find("(field_declaration ") == 0)
                    {
                        attributes.push_back(this->get_attribute(classBodyChild, sourceCode)
                        );
                    }
                    else if (classBodyChildName.find("(method_declaration ") == 0)
                    {
                        methods.push_back(this->get_method(classBodyChild, sourceCode));
                    }
                    else if (classBodyChildName.find("(constructor_declaration ") == 0)
                    {
                        constructors.push_back(this->get_constructor(classBodyChild, sourceCode));
                    }
                }
            }
        }

        astfri::ClassDefStmt* classDef = stmtFactory.mk_class_def(className);
        classDef->vars_                = attributes;
        classDef->methods_             = methods;
        classDef->constructors_        = constructors;
        classDef->tparams_             = tparams;
        classDef->bases_               = bases;
        classDef->interfaces_          = interfaces;
        classes.push_back(classDef);

        for (astfri::MethodDefStmt* method : methods)
        {
            method->owner_ = classDef;
        }
        for (astfri::ConstructorDefStmt* constructor : constructors)
        {
            constructor->owner_ = classDef;
        }
        attributes.clear();
        methods.clear();
        constructors.clear();
        tparams.clear();
        bases.clear();
    }

    return classes;
}

std::vector<astfri::ClassDefStmt*> NodeGetter::get_classes()
{
    return this->classes;
}