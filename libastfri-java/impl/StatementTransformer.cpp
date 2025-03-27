#include "StatementTransformer.hpp"
#include "ExpressionTransformer.hpp"

#include <cstdint>
#include <cstdlib>
#include <sys/types.h>

namespace astfri::java
{
    StatementTransformer::StatementTransformer() :
        typeFactory(astfri::TypeFactory::get_instance()),
        exprFactory(astfri::ExprFactory::get_instance()),
        stmtFactory(astfri::StmtFactory::get_instance()),
        exprTransformer(new ExpressionTransformer()),
        nodeMapper(new NodeMapper())
        {
        }

    astfri::Stmt* StatementTransformer::get_stmt(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        std::string nodeType = ts_node_type(tsNode);
        if (nodeType == "local_variable_declaration")
        {
            return this->transform_local_var_node(tsNode, sourceCode);
        }
        else if (nodeType == "expression_statement")
        {
            return this->transform_expr_stmt_node(tsNode, sourceCode);
        }
        else if (nodeType == "if_statement")
        {
            return this->transform_if_stmt_node(tsNode, sourceCode);
        }
        else if (nodeType == "switch_expression")
        {
            return this->transform_switch_stmt_node(tsNode, sourceCode);
        }
        else if (nodeType == "for_statement")
        {
            return this->transform_for_stmt_node(tsNode, sourceCode);
        }
        else if (nodeType == "while_statement")
        {
            return this->transform_while_stmt_node(tsNode, sourceCode);
        }
        else if (nodeType == "do_statement")
        {
            return this->transform_do_while_stmt_node(tsNode, sourceCode);
        }
        else if (nodeType == "return_statement")
        {
            return this->transform_return_stmt_node(tsNode, sourceCode);
        }
        else if (nodeType == "break_statement")
        {
            return stmtFactory.mk_break();
        }
        else if (nodeType == "continue_statement")
        {
            return stmtFactory.mk_continue();
        }
        else if (nodeType == "line_comment")
        {
            return stmtFactory.mk_expr(exprFactory.mk_string_literal(
                this->exprTransformer->get_node_text(tsNode, sourceCode)
            ));
        }
        else if (nodeType == "explicit_constructor_invocation")
        {
            std::vector<astfri::Expr*> args;
            TSNode argumentsListNode = ts_node_named_child(tsNode, 1);
            uint32_t argsCount       = ts_node_named_child_count(argumentsListNode);
            for (uint32_t i = 0; i < argsCount; i++)
            {
                TSNode argNode          = ts_node_named_child(argumentsListNode, i);
                std::string argNodeName = ts_node_string(argNode);
                args.push_back(exprTransformer->get_expr(argNode, sourceCode));
            }
            return stmtFactory.mak_base_initializer(
                exprTransformer
                    ->get_node_text(ts_node_named_child(tsNode, 0), sourceCode),
                args
            );
        }
        else
        {
            return stmtFactory.mk_uknown();
        }
    }

    astfri::AccessModifier StatementTransformer::get_access_modifier(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::AccessModifier access;
        uint32_t modifiersCount = ts_node_child_count(tsNode);
        for (uint32_t j = 0; j < modifiersCount; j++)
        {
            TSNode modifierNode = ts_node_child(tsNode, j);
            std::string modifierNodeName
                = this->exprTransformer->get_node_text(modifierNode, sourceCode);
            if (modifierNodeName == "private" || modifierNodeName == "public"
                || modifierNodeName == "internal"
                || modifierNodeName == "protected")
            {
                access
                    = this->nodeMapper->get_modMap().find(modifierNodeName)->second;
            }
        }
        return access;
    }

    astfri::Type* StatementTransformer::get_return_type(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::Type* type   = typeFactory.mk_unknown();
        std::string nodeName = ts_node_string(tsNode);
        if (nodeName.find("(identifier)") == 0)
        {
            type = typeFactory.mk_user(
                exprTransformer->get_node_text(tsNode, sourceCode)
            );
        }
        else if (nodeName.find("(generic_type") == 0)
        {
            type = typeFactory.mk_user(
                this->exprTransformer->get_node_text(tsNode, sourceCode)
            );
        }
        else if (nodeName.find("(array_type") == 0)
        {
            type = typeFactory.mk_user(
                this->exprTransformer->get_node_text(tsNode, sourceCode)
            );
        }
        else
        {
            std::string typeNodeText
                = this->exprTransformer->get_node_text(tsNode, sourceCode);
            type = this->nodeMapper->get_typeMap().find(typeNodeText)->second;
        }
        return type;
    }

    astfri::ParamVarDefStmt* StatementTransformer::transform_param_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::Type* type = typeFactory.mk_unknown();
        std::string name;

        std::string tsNodeName = ts_node_string(tsNode);
        uint32_t childCount    = ts_node_named_child_count(tsNode);
        bool typeSet           = false;
        for (uint32_t j = 0; j < childCount; j++)
        {
            TSNode child          = ts_node_named_child(tsNode, j);
            std::string childName = ts_node_string(child);

            if ((childName == "(identifier)"
                 || childName.find("type") != std::string::npos)
                && (! typeSet))
            {
                type    = this->get_return_type(child, sourceCode);
                typeSet = true;
            }
            else if (childName == "(identifier)")
            {
                name = exprTransformer->get_node_text(child, sourceCode);
            }
        }
        return stmtFactory.mk_param_var_def(name, type, nullptr);
    }

    astfri::LocalVarDefStmt* StatementTransformer::transform_local_var_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::Type* type = typeFactory.mk_unknown();
        std::string name;
        astfri::Expr* init;

        std::string tsNodeName = ts_node_string(tsNode);
        uint32_t childCount    = ts_node_named_child_count(tsNode);
        bool typeSet           = false;
        for (uint32_t j = 0; j < childCount; j++)
        {
            TSNode child          = ts_node_named_child(tsNode, j);
            std::string childName = ts_node_string(child);

            if ((childName == "(identifier)"
                 || childName.find("type") != std::string::npos)
                && (! typeSet))
            {
                type    = this->get_return_type(child, sourceCode);
                typeSet = true;
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
                        name = exprTransformer->get_node_text(
                            greatGrandChild,
                            sourceCode
                        );
                    }
                    else
                    {
                        init = exprTransformer->get_expr(
                            greatGrandChild,
                            sourceCode
                        );
                    }
                }
            }
        }
        return stmtFactory.mk_local_var_def(name, type, init);
    }

    astfri::ExprStmt* StatementTransformer::transform_expr_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::Expr* expr;

        TSNode child          = ts_node_named_child(tsNode, 0);
        std::string childName = ts_node_string(child);
        expr                  = exprTransformer->get_expr(child, sourceCode);

        return stmtFactory.mk_expr(expr);
    }

    astfri::IfStmt* StatementTransformer::transform_if_stmt_node(
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
                condition = exprTransformer->get_expr(conditionNode, sourceCode);
            }
            else if ((childName.find("(block ") == 0 && j == 1))
            {
                iftrueNode                 = child;
                std::string iftrueNodeName = ts_node_string(iftrueNode);
                iftrue = this->transform_body_node(iftrueNode, sourceCode);
            }
            else if (childName.find("(block ") == 0 && j == 2)
            {
                iffalseNode                 = child;
                std::string iffalseNodeName = ts_node_string(iffalseNode);
                iffalse = this->transform_body_node(iffalseNode, sourceCode);
            }
            else if (childName.find("(if_statement ") == 0)
            {
                iffalseNode                 = child;
                std::string iffalseNodeName = ts_node_string(iffalseNode);
                iffalse = this->transform_if_stmt_node(iffalseNode, sourceCode);
            }
        }

        return stmtFactory.mk_if(condition, iftrue, iffalse);
    }

    astfri::SwitchStmt* StatementTransformer::transform_switch_stmt_node(
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
            condition     = exprTransformer->get_expr(conditionNode, sourceCode);
        }
        if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
        {
            switchBodyNode     = ts_node_named_child(tsNode, 1);
            uint32_t caseCount = ts_node_named_child_count(switchBodyNode);

            for (uint32_t i = 0; i < caseCount; i++)
            {
                TSNode caseNode = ts_node_named_child(switchBodyNode, i);
                uint32_t switchCaseChildren = ts_node_named_child_count(caseNode);
                std::vector<astfri::Stmt*> stmts;
                std::vector<astfri::Expr*> caseExprs;
                bool isDefaultCase = false;

                for (uint32_t j = 0; j < switchCaseChildren; j++)
                {
                    TSNode child = ts_node_named_child(caseNode, j);
                    std::string childText
                        = exprTransformer->get_node_text(child, sourceCode);

                    if (childText.find("case") == 0)
                    {
                        TSNode caseChild          = ts_node_named_child(child, 0);
                        std::string caseChildName = ts_node_string(caseChild);
                        uint32_t exprCount = ts_node_named_child_count(child);
                        for (uint32_t k = 0; k < exprCount; k++)
                        {
                            TSNode exprNode    = ts_node_named_child(child, k);
                            astfri::Expr* expr = this->exprTransformer->get_expr(
                                exprNode,
                                sourceCode
                            );
                            caseExprs.push_back(expr);
                        }
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
                    cases.push_back(
                        stmtFactory.mk_default_case(stmtFactory.mk_compound(stmts))
                    );
                }
                else
                {
                    cases.push_back(stmtFactory.mk_case(
                        caseExprs,
                        stmtFactory.mk_compound(stmts)
                    ));
                }
            }
        }
        return stmtFactory.mk_switch(condition, cases);
    }

    astfri::ForStmt* StatementTransformer::transform_for_stmt_node(
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
            init = this->transform_local_var_node(initNode, sourceCode);
        }
        if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
        {
            conditionNode                 = ts_node_named_child(tsNode, 1);
            std::string conditionNodeName = ts_node_string(conditionNode);
            condition = exprTransformer->get_expr(conditionNode, sourceCode);
        }
        if (! ts_node_is_null(ts_node_named_child(tsNode, 2)))
        {
            stepNode                 = ts_node_named_child(tsNode, 2);
            std::string stepNodeName = ts_node_string(stepNode);
            astfri::Expr* expr = exprTransformer->get_expr(stepNode, sourceCode);
            step               = stmtFactory.mk_expr(expr);
        }
        if (! ts_node_is_null(ts_node_named_child(tsNode, 3)))
        {
            bodyNode                 = ts_node_named_child(tsNode, 3);
            std::string bodyNodeName = ts_node_string(bodyNode);
            body = this->transform_body_node(bodyNode, sourceCode);
        }

        return stmtFactory.mk_for(init, condition, step, body);
    }

    astfri::WhileStmt* StatementTransformer::transform_while_stmt_node(
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
                condition = exprTransformer->get_expr(conditionNode, sourceCode);
            }
        }
        if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
        {
            bodyNode                 = ts_node_named_child(tsNode, 1);
            std::string bodyNodeName = ts_node_string(bodyNode);
            body = this->transform_body_node(bodyNode, sourceCode);
        }

        return stmtFactory.mk_while(condition, body);
    }

    astfri::DoWhileStmt* StatementTransformer::transform_do_while_stmt_node(
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
            body = this->transform_body_node(bodyNode, sourceCode);
        }
        if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
        {
            helpNode = ts_node_named_child(tsNode, 1);
            if (! ts_node_is_null(ts_node_named_child(helpNode, 0)))
            {
                conditionNode                 = ts_node_named_child(helpNode, 0);
                std::string conditionNodeName = ts_node_string(conditionNode);
                condition = exprTransformer->get_expr(conditionNode, sourceCode);
            }
        }

        return stmtFactory.mk_do_while(condition, body);
    }

    astfri::ReturnStmt* StatementTransformer::transform_return_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::Expr* expr;

        TSNode child          = ts_node_named_child(tsNode, 0);
        std::string childName = ts_node_string(child);
        expr                  = exprTransformer->get_expr(child, sourceCode);

        return stmtFactory.mk_return(expr);
    }

    astfri::CompoundStmt* StatementTransformer::transform_body_node(
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

    FunctionType StatementTransformer::transform_function(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::Type* type = nullptr;
        std::string name;
        std::vector<astfri::ParamVarDefStmt*> params;
        std::vector<astfri::BaseInitializerStmt*> baseInit;
        astfri::CompoundStmt* body    = nullptr;
        astfri::AccessModifier access = astfri::AccessModifier::Public;
        bool typeSet                  = false;

        uint32_t methodChildCount     = ts_node_named_child_count(tsNode);
        for (uint32_t i = 0; i < methodChildCount; i++)
        {
            TSNode methodChild          = ts_node_named_child(tsNode, i);
            std::string methodChildName = ts_node_string(methodChild);

            if (methodChildName.find("(modifiers") == 0)
            {
                access = get_access_modifier(methodChild, sourceCode);
            }
            else if ((methodChildName == "(identifier)"
                      || methodChildName.find("type") != std::string::npos)
                     && ! typeSet)
            {
                type    = this->get_return_type(methodChild, sourceCode);
                typeSet = true;
            }
            else if (methodChildName == "(identifier)")
            {
                name = exprTransformer->get_node_text(methodChild, sourceCode);
            }
            else if (methodChildName.find("(formal_parameters ") == 0)
            {
                uint32_t paramtersCount = ts_node_named_child_count(methodChild);
                for (uint32_t j = 0; j < paramtersCount; j++)
                {
                    TSNode parameterNode = ts_node_named_child(methodChild, j);
                    std::string parameterNodeName = ts_node_string(parameterNode);
                    params.push_back(
                        this->transform_param_node(parameterNode, sourceCode)
                    );
                }
            }
            else if (methodChildName.find("(constructor_body") == 0
                     || methodChildName.find("(block") == 0)
            {
                uint32_t bodyChildCount = ts_node_named_child_count(methodChild);
                std::vector<astfri::Stmt*> bodyStatements;

                for (uint32_t j = 0; j < bodyChildCount; j++)
                {
                    TSNode bodyChild   = ts_node_named_child(methodChild, j);
                    astfri::Stmt* stmt = this->get_stmt(bodyChild, sourceCode);

                    if (auto baseInitStmt
                        = dynamic_cast<astfri::BaseInitializerStmt*>(stmt))
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

        return std::make_tuple(access, type, name, params, baseInit, body);
    }

    astfri::MethodDefStmt* StatementTransformer::transform_method_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::Type* type = nullptr;
        std::string name;
        std::vector<astfri::ParamVarDefStmt*> params;
        astfri::CompoundStmt* body    = nullptr;
        astfri::AccessModifier access = astfri::AccessModifier::Public;

        std::tuple methodBody = this->transform_function(tsNode, sourceCode);
        access                = std::get<astfri::AccessModifier>(methodBody);
        type                  = std::get<astfri::Type*>(methodBody);
        name                  = std::get<std::string>(methodBody);
        params = std::get<std::vector<astfri::ParamVarDefStmt*>>(methodBody);
        body   = std::get<astfri::CompoundStmt*>(methodBody);

        astfri::FunctionDefStmt* func
            = stmtFactory.mk_function_def(name, params, type, body);

        return stmtFactory
            .mk_method_def(nullptr, func, access, astfri::Virtuality::NotVirtual);
    }

    astfri::ConstructorDefStmt* StatementTransformer::transform_constructor_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::AccessModifier access = astfri::AccessModifier::Public;
        std::string name;
        std::vector<astfri::ParamVarDefStmt*> params;
        std::vector<astfri::BaseInitializerStmt*> baseInit;
        astfri::CompoundStmt* body = nullptr;

        std::tuple methodBody      = this->transform_function(tsNode, sourceCode);
        access                     = std::get<astfri::AccessModifier>(methodBody);
        name                       = std::get<std::string>(methodBody);
        baseInit = std::get<std::vector<astfri::BaseInitializerStmt*>>(methodBody);
        params   = std::get<std::vector<astfri::ParamVarDefStmt*>>(methodBody);
        body     = std::get<astfri::CompoundStmt*>(methodBody);

        return stmtFactory
            .mk_constructor_def(nullptr, params, baseInit, body, access);
    }

    astfri::MemberVarDefStmt* StatementTransformer::transform_attribute_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        astfri::AccessModifier access = astfri::AccessModifier::Private;
        astfri::Type* type            = nullptr;
        astfri::Expr* init            = nullptr;
        bool typeSet                  = false;
        std::string name;

        std::string tsNodeName = ts_node_string(tsNode);
        uint32_t childCount    = ts_node_named_child_count(tsNode);
        for (uint32_t j = 0; j < childCount; j++)
        {
            TSNode child          = ts_node_named_child(tsNode, j);
            std::string childName = ts_node_string(child);

            if (childName == "(modifiers)")
            {
                access = get_access_modifier(child, sourceCode);
            }
            else if ((childName == "(identifier)"
                      || childName.find("type") != std::string::npos)
                     && (! typeSet))
            {
                type    = this->get_return_type(child, sourceCode);
                typeSet = true;
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
                        name = exprTransformer->get_node_text(
                            greatGrandChild,
                            sourceCode
                        );
                    }
                    else
                    {
                        init = exprTransformer->get_expr(
                            greatGrandChild,
                            sourceCode
                        );
                    }
                }
            }
        }

        return stmtFactory.mk_member_var_def(name, type, init, access);
    }

    astfri::GenericParam* StatementTransformer::transform_tparam_node(
        TSNode tsNode,
        std::string const& sourceCode
    )
    {
        std::string name;
        std::string constraint;
        if (! ts_node_is_null(ts_node_named_child(tsNode, 0)))
        {
            name = exprTransformer->get_node_text(
                ts_node_named_child(tsNode, 0),
                sourceCode
            );
        }
        if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
        {
            constraint = exprTransformer->get_node_text(
                ts_node_named_child(tsNode, 1),
                sourceCode
            );
        }

        return stmtFactory.mk_generic_param(constraint, name);
    }

    std::vector<astfri::ClassDefStmt*> StatementTransformer::transform_classes(
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
        std::vector<astfri::GenericParam*> tparams;
        std::vector<astfri::ClassDefStmt*> bases;
        std::vector<astfri::InterfaceDefStmt*> interfaces;
        std::vector<astfri::ClassDefStmt*> classes;

        uint32_t childCount = ts_node_named_child_count(rootNode);
        for (uint32_t i = 0; i < childCount; i++)
        {
            if (! ts_node_is_null(ts_node_named_child(rootNode, i)))
            {
                TSNode child          = ts_node_named_child(rootNode, i);
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
                TSNode classChild          = ts_node_named_child(classNode, j);
                std::string classChildName = ts_node_string(classChild);

                if (classChildName.find("(modifiers)") == 0)
                {
                    continue;
                }
                else if (classChildName.find("(identifier)") == 0)
                {
                    className
                        = exprTransformer->get_node_text(classChild, sourceCode);
                }
                else if (classChildName.find("(type_parameters ") == 0)
                {
                    uint32_t parametersCount
                        = ts_node_named_child_count(classChild);
                    for (uint32_t k = 0; k < parametersCount; k++)
                    {
                        TSNode parameterNode = ts_node_named_child(classChild, k);
                        tparams.push_back(
                            this->transform_tparam_node(parameterNode, sourceCode)
                        );
                    }
                }
                else if (classChildName.find("(superclass ") == 0)
                {
                    bases.push_back(
                        stmtFactory.mk_class_def(exprTransformer->get_node_text(
                            ts_node_named_child(classChild, 0),
                            sourceCode
                        ))
                    );
                }
                else if (classChildName.find("(super_interfaces ") == 0)
                {
                    uint32_t typeListChildCount
                        = ts_node_named_child_count(classChild);
                    for (uint32_t k = 0; k < typeListChildCount; k++)
                    {
                        TSNode typeListChild = ts_node_named_child(classChild, k);
                        interfaces.push_back(stmtFactory.mk_interface_def(
                            exprTransformer
                                ->get_node_text(typeListChild, sourceCode)
                        ));
                    }
                }
                else if (classChildName.find("(class_body ") == 0)
                {
                    uint32_t classBodyChildCount
                        = ts_node_named_child_count(classChild);
                    for (uint32_t k = 0; k < classBodyChildCount; k++)
                    {
                        TSNode classBodyChild = ts_node_named_child(classChild, k);
                        std::string classBodyChildName
                            = ts_node_string(classBodyChild);

                        if (classBodyChildName.find("(field_declaration ") == 0)
                        {
                            attributes.push_back(this->transform_attribute_node(
                                classBodyChild,
                                sourceCode
                            ));
                        }
                        else if (classBodyChildName.find("(method_declaration ")
                                 == 0)
                        {
                            methods.push_back(this->transform_method_node(
                                classBodyChild,
                                sourceCode
                            ));
                        }
                        else if (classBodyChildName.find("(constructor_declaration "
                                 )
                                 == 0)
                        {
                            constructors.push_back(this->transform_constructor_node(
                                classBodyChild,
                                sourceCode
                            ));
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
            interfaces.clear();
        }

        return classes;
    }

    std::vector<astfri::InterfaceDefStmt*> StatementTransformer::
        transform_interfaces(TSTree* tree, std::string const& sourceCode)
    {
        TSNode rootNode = ts_tree_root_node(tree);
        TSNode interfaceNode;
        std::string interfaceName;
        std::vector<astfri::MethodDefStmt*> methods;
        std::vector<astfri::GenericParam*> tparams;
        std::vector<astfri::InterfaceDefStmt*> bases;
        std::vector<astfri::InterfaceDefStmt*> interfaces;

        uint32_t childCount = ts_node_named_child_count(rootNode);
        for (uint32_t i = 0; i < childCount; i++)
        {
            if (! ts_node_is_null(ts_node_named_child(rootNode, i)))
            {
                TSNode child          = ts_node_named_child(rootNode, i);
                std::string childName = ts_node_string(child);
                if (childName.find("(interface_declaration ") == 0)
                {
                    interfaceNode = child;
                }
                else
                {
                    continue;
                }
            }

            uint32_t interfaceChildCount = ts_node_named_child_count(interfaceNode);
            for (uint32_t j = 0; j < interfaceChildCount; j++)
            {
                TSNode interfaceChild = ts_node_named_child(interfaceNode, j);
                std::string interfaceChildName = ts_node_string(interfaceChild);

                if (interfaceChildName.find("(identifier)") == 0)
                {
                    interfaceName = exprTransformer->get_node_text(
                        interfaceChild,
                        sourceCode
                    );
                }
                else if (interfaceChildName.find("(type_parameters ") == 0)
                {
                    uint32_t parametersCount
                        = ts_node_named_child_count(interfaceChild);
                    for (uint32_t k = 0; k < parametersCount; k++)
                    {
                        TSNode parameterNode
                            = ts_node_named_child(interfaceChild, k);
                        tparams.push_back(
                            this->transform_tparam_node(parameterNode, sourceCode)
                        );
                    }
                }
                else if (interfaceChildName.find("(extends_interfaces ") == 0)
                {
                    uint32_t typeListChildCount
                        = ts_node_named_child_count(interfaceChild);
                    for (uint32_t k = 0; k < typeListChildCount; k++)
                    {
                        TSNode typeListChild
                            = ts_node_named_child(interfaceChild, k);
                        bases.push_back(stmtFactory.mk_interface_def(
                            exprTransformer
                                ->get_node_text(typeListChild, sourceCode)
                        ));
                    }
                }
                else if (interfaceChildName.find("(interface_body ") == 0)
                {
                    uint32_t interfaceBodyChildCount
                        = ts_node_named_child_count(interfaceChild);
                    for (uint32_t k = 0; k < interfaceBodyChildCount; k++)
                    {
                        TSNode interfaceBodyChild
                            = ts_node_named_child(interfaceChild, k);
                        std::string interfaceBodyChildName
                            = ts_node_string(interfaceBodyChild);

                        if (interfaceBodyChildName.find("(method_declaration ")
                            == 0)
                        {
                            methods.push_back(this->transform_method_node(
                                interfaceBodyChild,
                                sourceCode
                            ));
                        }
                    }
                }
            }

            astfri::InterfaceDefStmt* interfaceDef
                = stmtFactory.mk_interface_def(interfaceName);
            interfaceDef->methods_ = methods;
            interfaceDef->tparams_ = tparams;
            interfaceDef->bases_   = bases;
            interfaces.push_back(interfaceDef);

            for (astfri::MethodDefStmt* method : methods)
            {
                method->owner_ = interfaceDef;
            }
            methods.clear();
            tparams.clear();
            bases.clear();
        }

        return interfaces;
    }

    astfri::TranslationUnit* StatementTransformer::fill_translation_unit(TSTree* tree, std::string const& sourceCode)
    {
        astfri::TranslationUnit* tu = this->stmtFactory.mk_translation_unit();
        tu->classes_ = this->transform_classes(tree, sourceCode);
        tu->interfaces_ = this->transform_interfaces(tree, sourceCode);
        return tu;
    }
} // namespace astfri::java