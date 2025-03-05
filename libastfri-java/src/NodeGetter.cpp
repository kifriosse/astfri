#include "NodeGetter.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>

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

astfri::Expr* NodeGetter::get_expr(
    std::string const& nodeName,
    std::string const& nodeText,
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* expr;
    if (nodeName == "(decimal_integer_literal)")
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
    else if (nodeName == "(string_literal)")
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
    else if (nodeName == "(identifier)")
    {
        auto refExprVariant = this->get_ref_expr(tsNode, sourceCode);
        std::visit([&expr] (auto&& arg) { expr = arg; }, refExprVariant);
    }
    else if (nodeName.find("(binary_expression") != std::string::npos)
    {
        expr = this->get_bin_op_expr(tsNode, sourceCode);
    }
    else if (nodeName.find("(unary_expression") != std::string::npos)
    {
        expr = this->get_un_op_expr(tsNode, sourceCode);
    }
    else if (nodeName.find("(assignment_expression") != std::string::npos)
    {
        auto assignExprVariant = this->get_assign_expr(tsNode, sourceCode);
        std::visit([&expr] (auto&& arg) { expr = arg; }, assignExprVariant);
    }
    else if (nodeName == "(this)")
    {
        expr = exprFactory.mk_this();
    }
    else if (nodeName.find("(object_creation_expression") != std::string::npos)
    {
        expr = this->get_new_expr(tsNode, sourceCode);
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
    astfri::BinOpExpr* binOpExpr;

    char const* queryString = "(binary_expression left: (_) @left operator: _ "
                              "@operator right: (_) @right)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, tsNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        astfri::Expr* leftExpr;
        astfri::Expr* rightExpr;
        astfri::BinOpType binOpType;

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

            if (captureName == "left")
            {
                leftExpr = this->get_expr(
                    nodeName,
                    nodeText,
                    tsCapture.node,
                    sourceCode
                );
            }
            else if (captureName == "right")
            {
                rightExpr = this->get_expr(
                    nodeName,
                    nodeText,
                    tsCapture.node,
                    sourceCode
                );
            }
            else if (captureName == "operator")
            {
                binOpType
                    = this->nodeMapper->get_binOpMap().find(nodeText)->second;
            }
        }

        binOpExpr = exprFactory.mk_bin_on(leftExpr, binOpType, rightExpr);
    }

    return binOpExpr;
}

astfri::UnaryOpExpr* NodeGetter::get_un_op_expr(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::UnaryOpExpr* unOpExpr;

    char const* queryString
        = "(unary_expression operator: _ @operator operand: (_) @operand)";

    TSQuery* tsQuery        = make_query(queryString);
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
                operand = this->get_expr(
                    nodeName,
                    nodeText,
                    tsCapture.node,
                    sourceCode
                );
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

std::variant<astfri::AssignExpr*, astfri::CompoundAssignExpr*> NodeGetter::
    get_assign_expr(TSNode tsNode, std::string const& sourceCode)
{
    astfri::AssignExpr* assignExpr                 = nullptr;
    astfri::CompoundAssignExpr* compoundAssignExpr = nullptr;

    char const* queryString = "(assignment_expression left: (_) @left"
                              "operator: _ @operator right: (_) @right)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, tsNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        astfri::Expr* leftExpr  = nullptr;
        astfri::Expr* rightExpr = nullptr;
        astfri::BinOpType binOpType;

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

            if (captureName == "left")
            {
                leftExpr = this->get_expr(
                    nodeName,
                    nodeText,
                    tsCapture.node,
                    sourceCode
                );
            }
            else if (captureName == "right")
            {
                rightExpr = this->get_expr(
                    nodeName,
                    nodeText,
                    tsCapture.node,
                    sourceCode
                );
            }
            else if (captureName == "operator")
            {
                binOpType
                    = this->nodeMapper->get_binOpMap().find(nodeText)->second;
            }
        }

        if (astfri::BinOpType::Assign == binOpType)
        {
            assignExpr = exprFactory.mk_assign(leftExpr, rightExpr);
        }
        else
        {
            compoundAssignExpr = exprFactory.mk_compound_assign(
                leftExpr,
                binOpType,
                rightExpr
            );
        }
    }

    if (assignExpr != nullptr)
    {
        return assignExpr;
    }
    else
    {
        return compoundAssignExpr;
    }
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
    astfri::MethodCallExpr* methodCallExpr = nullptr;

    char const* queryString
        = "(method_invocation object: (_) @object name: (identifier) @name "
          "arguments: (argument_list) @args)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, tsNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        astfri::Expr* owner;
        std::string methodName;
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

            if (captureName == "object")
            {
                if (nodeName == "(identifier)")
                {
                    owner = exprFactory.mk_class_ref(nodeText);
                }
                else
                {
                    owner = this->get_expr(
                        nodeName,
                        nodeText,
                        tsCapture.node,
                        sourceCode
                    );
                }
            }
            else if (captureName == "name")
            {
                methodName = nodeText;
            }
            else if (captureName == "args")
            {
                for (uint32_t j = 0; j < ts_node_child_count(tsCapture.node);
                     ++j)
                {
                    arguments.push_back(get_expr(
                        nodeName,
                        nodeText,
                        ts_node_child(tsCapture.node, j),
                        sourceCode
                    ));
                }
            }
        }

        methodCallExpr
            = exprFactory.mk_method_call(owner, methodName, arguments);
    }

    return methodCallExpr;
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
                    arguments.push_back(get_expr(
                        nodeName,
                        nodeText,
                        ts_node_child(tsCapture.node, j),
                        sourceCode
                    ));
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
        std::cout << "param added" << std::endl;
    }

    ts_query_cursor_delete(tsCursor);
    ts_query_delete(tsQuery);
    return params;
}

StatementReturnType NodeGetter::search_sub_tree(
    TSNode tsNode,
    std::string const& nodeName,
    std::string const& sourceCode
)
{
    std::vector<astfri::LocalVarDefStmt*> localVars;

    uint32_t tsNodeChildeCount = ts_node_named_child_count(tsNode);
    astfri::Type* type = typeFactory.mk_unknown();
    std::string name;
    astfri::Expr* value = exprFactory.mk_unknown();
    astfri::Expr* expr;

    for (uint32_t i = 0; i < tsNodeChildeCount; i++)
    {
        TSNode child          = ts_node_named_child(tsNode, i);
        std::string childName = ts_node_string(child);
        if (childName.find(nodeName) != std::string::npos)
        {
            uint32_t grandChildCount = ts_node_named_child_count(child);
            for (uint32_t j = 0; j < grandChildCount; j++)
            {
                TSNode grandChild          = ts_node_named_child(child, j);
                std::string grandChildName = ts_node_string(grandChild);

                if (grandChildName.find("type") != std::string::npos)
                {
                    if (grandChildName == "(type_identifier)")
                    {
                        type = typeFactory.mk_user(
                            get_node_text(grandChild, sourceCode)
                        );
                    }
                    else
                    {
                        type = this->nodeMapper->get_typeMap()
                                   .find(get_node_text(grandChild, sourceCode))
                                   ->second;
                    }
                }
                else if (grandChildName.find("(variable_declarator")
                         != std::string::npos)
                {
                    uint32_t greatGrandChildCount
                        = ts_node_named_child_count(grandChild);
                    for (uint32_t k = 0; k < greatGrandChildCount; k++)
                    {
                        TSNode greatGrandChild
                            = ts_node_named_child(grandChild, k);
                        std::string greatGrandChildName
                            = ts_node_string(greatGrandChild);

                        if (greatGrandChildName == "(identifier)")
                        {
                            name = get_node_text(greatGrandChild, sourceCode);
                        }
                        else if (greatGrandChildName.find("expression")
                                     != std::string::npos
                                 || greatGrandChildName.find("method_invocation"
                                    ) != std::string::npos
                                 || greatGrandChildName.find("_literal")
                                        != std::string::npos)
                        {
                            value = this->get_expr(
                                ts_node_string(greatGrandChild),
                                get_node_text(greatGrandChild, sourceCode),
                                greatGrandChild,
                                sourceCode
                            );
                        }
                    }
                }
                else if (grandChildName.find("_expression")
                         != std::string::npos)
                {
                    uint32_t greatGrandChildrenCount
                        = ts_node_named_child_count(grandChild);
                    for (uint32_t k = 0; k < greatGrandChildrenCount; k++)
                    {
                        TSNode greatGrandChildNode
                            = ts_node_child(grandChild, k);
                        expr = this->get_expr(
                            ts_node_string(greatGrandChildNode),
                            get_node_text(greatGrandChildNode, sourceCode),
                            greatGrandChildNode,
                            sourceCode
                        );
                    }
                }
            }

            if (nodeName.find("(local_variable_declaration ")
                != std::string::npos)
            {
                localVars.push_back(
                    stmtFactory.mk_local_var_def(name, type, value)
                );
            }
        }
    }
    if (nodeName.find("(local_variable_declaration ") != std::string::npos)
    {
        return localVars;
    }
    else if (nodeName.find("(expression_statement "))
    {
        return stmtFactory.mk_expr(expr);
    }
    else
    {
        return stmtFactory.mk_return(expr);
    }
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

std::vector<astfri::MethodDefStmt*> NodeGetter::get_methods(
    TSNode classNode,
    std::string const& sourceCode
)
{
    std::vector<astfri::MethodDefStmt*> methods;

    char const* queryString
        = "(method_declaration (modifiers)? @method_modifiers type: (_) "
          "@method_return_type name: (identifier) @method_name "
          "(formal_parameters)? @params body: (block) @method_body)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, classNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        astfri::Type* methodReturnType = typeFactory.mk_unknown();
        std::string methodName;
        std::vector<astfri::ParamVarDefStmt*> methodParams;
        std::vector<astfri::LocalVarDefStmt*> methodVars;
        std::vector<std::string> methodModifiers;
        TSNode paramsNode;
        TSNode methodNode;
        astfri::FunctionDefStmt* func;
        astfri::AccessModifier access;

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

            if (captureName == "method_modifiers")
            {
                methodModifiers.push_back(nodeText);
            }
            else if (captureName == "method_return_type")
            {
                if (nodeName == "(type_identifier)")
                {
                    methodReturnType = typeFactory.mk_user(nodeText);
                }
                else
                {
                    methodReturnType = this->nodeMapper->get_typeMap()
                                           .find(nodeText)
                                           ->second;
                }
            }
            else if (captureName == "method_name")
            {
                methodName = nodeText;
            }
            else if (captureName == "params")
            {
                paramsNode = tsCapture.node;
            }
            else if (captureName == "method_body")
            {
                methodNode = tsCapture.node;
            }
        }

        if (! ts_node_is_null(paramsNode))
        {
            methodParams = this->get_params(paramsNode, sourceCode);
        }
        if (! ts_node_is_null(methodNode))
        {
            auto stmt = search_sub_tree(
                methodNode,
                "(local_variable_declaration ",
                sourceCode
            );

            std::visit(
                [&methodVars] (auto&& arg)
                {
                    if constexpr (std::is_same_v<
                                      decltype(arg),
                                      std::vector<astfri::LocalVarDefStmt*>>)
                    {
                        methodVars = arg;
                    }
                },
                stmt
            );
        }

        func = stmtFactory.mk_function_def(
            methodName,
            methodParams,
            methodReturnType,
            nullptr
        );

        access = astfri::AccessModifier::Internal;
        for (std::string mod : methodModifiers)
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

        astfri::MethodDefStmt* method
            = stmtFactory.mk_method_def(nullptr, func, access);
        methods.push_back(method);
        std::cout << "method added" << std::endl;
        methodModifiers.clear();
    }

    ts_query_cursor_delete(tsCursor);
    ts_query_delete(tsQuery);
    return methods;
}

std::vector<astfri::MemberVarDefStmt*> NodeGetter::get_member_vars(
    TSNode classNode,
    std::string const& sourceCode
)
{
    std::vector<astfri::MemberVarDefStmt*> memberVars;

    char const* queryString = "(field_declaration"
                              "  (modifiers)? @member_modifier"
                              "  type: (_) @member_type"
                              "  (variable_declarator"
                              "    name: (identifier) @member_name"
                              "    value: (_)? @member_value"
                              "  )"
                              ")";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, classNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        astfri::Type* memberVarType = typeFactory.mk_unknown();
        std::string memberVarName;
        astfri::Expr* memberVarValue;
        std::vector<std::string> memberVarModifiers;

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

            if (captureName == "memeber_modifier")
            {
                memberVarModifiers.push_back(nodeText);
            }
            else if (captureName == "member_type")
            {
                if (nodeName == "(type_identifier)")
                {
                    memberVarType = typeFactory.mk_user(nodeText);
                }
                else
                {
                    memberVarType = this->nodeMapper->get_typeMap()
                                        .find(nodeText)
                                        ->second;
                }
            }
            else if (captureName == "member_name")
            {
                memberVarName = nodeText;
            }
            else if (captureName == "member_value")
            {
                memberVarValue
                    = get_expr(nodeName, nodeText, tsCapture.node, sourceCode);
            }
        }

        astfri::AccessModifier access = astfri::AccessModifier::Internal;
        for (std::string mod : memberVarModifiers)
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

        memberVars.push_back(stmtFactory.mk_member_var_def(
            memberVarName,
            memberVarType,
            memberVarValue,
            access
        ));
        std::cout << "member added" << std::endl;
    }

    ts_query_cursor_delete(tsCursor);
    ts_query_delete(tsQuery);
    return memberVars;
}

std::vector<astfri::ClassDefStmt*> NodeGetter::get_classes(
    TSTree* tree,
    std::string const& sourceCode
)
{
    std::vector<astfri::ClassDefStmt*> classes;
    TSNode rootNode         = ts_tree_root_node(tree);

    char const* queryString = "(class_declaration"
                              "(modifiers)? @modifier"
                              "(identifier) @class_name"
                              "(class_body)? @class_body"
                              ")";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, rootNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        std::vector<std::string> classModifiers;
        std::string className;
        std::vector<astfri::MemberVarDefStmt*> classMemberVars;
        std::vector<astfri::MethodDefStmt*> classMethods;
        TSNode tsNode;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

            if (captureName == "class_modifier")
            {
                classModifiers.push_back(nodeText);
            }
            else if (captureName == "class_name")
            {
                className = nodeText;
            }
            else if (captureName == "class_body")
            {
                tsNode = tsCapture.node;
            }
        }

        if (! ts_node_is_null(tsNode))
        {
            classMemberVars = this->get_member_vars(tsNode, sourceCode);
            classMethods = this->get_methods(tsNode, sourceCode);
        }

        astfri::ClassDefStmt* classDef
            = stmtFactory
                  .mk_class_def(className, classMemberVars, classMethods, {});
        classes.push_back(classDef);
        std::cout << "class added" << std::endl;

        for (astfri::MethodDefStmt* method : classMethods)
        {
            method->owner_ = classDef;
            std::cout << "method owner set" << std::endl;
        }
    }

    ts_query_cursor_delete(tsCursor);
    ts_query_delete(tsQuery);
    return classes;
}

std::vector<astfri::ClassDefStmt*> NodeGetter::get_classes()
{
    return this->classes;
}