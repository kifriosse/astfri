#include "StatementTransformer.hpp"

#include <cstdint>
#include <cstdlib>
#include <sys/types.h>
#include <tree_sitter/api.h>

namespace astfri::java
{
StatementTransformer::StatementTransformer() :
    stmtFactory(astfri::StmtFactory::get_instance()),
    exprTransformer(new ExpressionTransformer(this)),
    nodeMapper(new NodeMapper()),
    functionalInterfaces()
{
}

StatementTransformer::~StatementTransformer()
{
    delete this->exprTransformer;
    delete this->nodeMapper;
}

astfri::Stmt* StatementTransformer::get_stmt(TSNode tsNode, std::string const& sourceCode)
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
        return stmtFactory.mk_expr(this->exprTransformer->get_expr(tsNode, sourceCode));
    }
    else if (nodeType == "explicit_constructor_invocation")
    {
        std::vector<astfri::Expr*> args;
        TSNode argumentsListNode = ts_node_named_child(tsNode, 1);
        uint32_t argsCount       = ts_node_named_child_count(argumentsListNode);
        for (uint32_t i = 0; i < argsCount; i++)
        {
            TSNode argNode          = ts_node_named_child(argumentsListNode, i);
            std::string argNodeType = ts_node_type(argNode);
            args.push_back(exprTransformer->get_expr(argNode, sourceCode));
        }
        return stmtFactory.mak_base_initializer(
            exprTransformer->get_node_text(ts_node_named_child(tsNode, 0), sourceCode),
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
    astfri::AccessModifier access = astfri::AccessModifier::Internal;
    uint32_t modifiersCount = ts_node_child_count(tsNode);
    for (uint32_t j = 0; j < modifiersCount; j++)
    {
        TSNode modifierNode = ts_node_child(tsNode, j);
        std::string modifierNodeName
            = this->exprTransformer->get_node_text(modifierNode, sourceCode);

        access = this->nodeMapper->get_modMap().contains(modifierNodeName)
           ? this->nodeMapper->get_modMap().at(modifierNodeName)
           : access;
    }
    return access;
}

astfri::Type* StatementTransformer::get_return_type(TSNode tsNode, std::string const& sourceCode)
{
    astfri::Type* type = astfri::TypeFactory::get_instance().mk_unknown();
    std::string typeNodeType = ts_node_type(tsNode);
    std::string typeNodeText = this->exprTransformer->get_node_text(tsNode, sourceCode);

    type = this->nodeMapper->get_typeMap().contains(typeNodeText)
        ? this->nodeMapper->get_typeMap().at(typeNodeText)
        : astfri::TypeFactory::get_instance().mk_class(typeNodeText, {});
    return type;
}

astfri::ParamVarDefStmt* StatementTransformer::transform_param_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Type* type     = nullptr;
    std::string name       = "";

    std::string tsNodeType = ts_node_type(tsNode);
    uint32_t childCount    = ts_node_named_child_count(tsNode);

    for (uint32_t j = 0; j < childCount; j++)
    {
        TSNode paramNode          = ts_node_named_child(tsNode, j);
        std::string paramNodeType = ts_node_type(paramNode);

        if (paramNodeType.find("type") != std::string::npos)
        {
            type = this->get_return_type(paramNode, sourceCode);
        }
        else if (paramNodeType == "identifier")
        {
            name = this->exprTransformer->get_node_text(paramNode, sourceCode);
        }
    }
    return stmtFactory.mk_param_var_def(name, type, nullptr);
}

astfri::LocalVarDefStmt* StatementTransformer::transform_local_var_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Type* type     = nullptr;
    std::string name       = "";
    astfri::Expr* init     = nullptr;

    std::string tsNodeType = ts_node_type(tsNode);
    uint32_t childCount    = ts_node_named_child_count(tsNode);

    for (uint32_t j = 0; j < childCount; j++)
    {
        TSNode varNode          = ts_node_named_child(tsNode, j);
        std::string varNodeType = ts_node_type(varNode);

        if (varNodeType.find("type") != std::string::npos)
        {
            type = this->get_return_type(varNode, sourceCode);
        }
        else if (varNodeType == "variable_declarator")
        {
            uint32_t declaratorChildCount = ts_node_named_child_count(varNode);
            for (uint32_t k = 0; k < declaratorChildCount; k++)
            {
                TSNode declaratorChild          = ts_node_named_child(varNode, k);
                std::string declaratorChildType = ts_node_type(declaratorChild);

                if (declaratorChildType == "identifier" && k == 0)
                {
                    name = exprTransformer->get_node_text(declaratorChild, sourceCode);
                }
                else
                {
                    init = exprTransformer->get_expr(declaratorChild, sourceCode);
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
    astfri::Expr* expr    = nullptr;

    TSNode child          = ts_node_named_child(tsNode, 0);
    std::string childType = ts_node_type(child);
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
        std::string childType = ts_node_type(child);
        TSNode conditionNode  = ts_node_named_child(child, 0);
        TSNode iftrueNode;
        TSNode iffalseNode;
        if (childType == "parenthesized_expression")
        {
            std::string conditionNodeType = ts_node_type(conditionNode);
            condition                     = exprTransformer->get_expr(conditionNode, sourceCode);
        }
        else if ((childType == "block" && j == 1))
        {
            iftrueNode                 = child;
            std::string iftrueNodeType = ts_node_type(iftrueNode);
            iftrue                     = this->transform_body_node(iftrueNode, sourceCode);
        }
        else if (childType == "block" && j == 2)
        {
            iffalseNode                 = child;
            std::string iffalseNodeType = ts_node_type(iffalseNode);
            iffalse                     = this->transform_body_node(iffalseNode, sourceCode);
        }
        else if (childType == "if_statement")
        {
            iffalseNode                 = child;
            std::string iffalseNodeType = ts_node_type(iffalseNode);
            iffalse                     = this->transform_if_stmt_node(iffalseNode, sourceCode);
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
            TSNode caseNode             = ts_node_named_child(switchBodyNode, i);
            uint32_t switchCaseChildren = ts_node_named_child_count(caseNode);
            std::vector<astfri::Stmt*> stmts;
            std::vector<astfri::Expr*> caseExprs;
            bool isDefaultCase = false;

            for (uint32_t j = 0; j < switchCaseChildren; j++)
            {
                TSNode child          = ts_node_named_child(caseNode, j);
                std::string childText = exprTransformer->get_node_text(child, sourceCode);

                if (childText.find("case") == 0)
                {
                    TSNode caseChild          = ts_node_named_child(child, 0);
                    std::string caseChildType = ts_node_type(caseChild);
                    uint32_t exprCount        = ts_node_named_child_count(child);
                    for (uint32_t k = 0; k < exprCount; k++)
                    {
                        TSNode exprNode    = ts_node_named_child(child, k);
                        astfri::Expr* expr = this->exprTransformer->get_expr(exprNode, sourceCode);
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
                cases.push_back(stmtFactory.mk_default_case(stmtFactory.mk_compound(stmts)));
            }
            else
            {
                cases.push_back(stmtFactory.mk_case(caseExprs, stmtFactory.mk_compound(stmts)));
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

    uint32_t childCount     = ts_node_named_child_count(tsNode);
    for (uint32_t i = 0; i < childCount; i++)
    {
        TSNode child          = ts_node_named_child(tsNode, i);
        std::string childType = ts_node_type(child);
        char const* fieldName = ts_node_field_name_for_named_child(tsNode, i);

        if (fieldName != nullptr && std::strcmp(fieldName, "init") == 0)
        {
            init = this->transform_local_var_node(child, sourceCode);
        }
        if (fieldName != nullptr && std::strcmp(fieldName, "condition") == 0)
        {
            condition = exprTransformer->get_expr(child, sourceCode);
        }
        if (fieldName != nullptr && std::strcmp(fieldName, "update") == 0)
        {
            step = stmtFactory.mk_expr(exprTransformer->get_expr(child, sourceCode));
        }
        if (fieldName != nullptr && std::strcmp(fieldName, "body") == 0)
        {
            body = this->transform_body_node(child, sourceCode);
        }
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

    uint32_t childCount     = ts_node_named_child_count(tsNode);
    for (uint32_t i = 0; i < childCount; i++)
    {
        TSNode child          = ts_node_named_child(tsNode, i);
        std::string childType = ts_node_type(child);
        char const* fieldName = ts_node_field_name_for_named_child(tsNode, i);

        if (fieldName != nullptr && std::strcmp(fieldName, "condition") == 0)
        {
            condition = exprTransformer->get_expr(child, sourceCode);
        }
        if (fieldName != nullptr && std::strcmp(fieldName, "body") == 0)
        {
            body = this->transform_body_node(child, sourceCode);
        }
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

    uint32_t childCount     = ts_node_named_child_count(tsNode);
    for (uint32_t i = 0; i < childCount; i++)
    {
        TSNode child          = ts_node_named_child(tsNode, i);
        std::string childType = ts_node_type(child);
        char const* fieldName = ts_node_field_name_for_named_child(tsNode, i);

        if (fieldName != nullptr && std::strcmp(fieldName, "body") == 0)
        {
            body = this->transform_body_node(child, sourceCode);
        }
        if (fieldName != nullptr && std::strcmp(fieldName, "condition") == 0)
        {
            condition = exprTransformer->get_expr(child, sourceCode);
        }
    }

    return stmtFactory.mk_do_while(condition, body);
}

astfri::ReturnStmt* StatementTransformer::transform_return_stmt_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::Expr* expr    = nullptr;

    if (!ts_node_is_null(ts_node_named_child(tsNode, 0)))
    {
        TSNode child          = ts_node_named_child(tsNode, 0);
        std::string childType = ts_node_type(child);
        expr                  = exprTransformer->get_expr(child, sourceCode);
    }

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
        std::string childType = ts_node_type(child);
        statements.push_back(this->get_stmt(child, sourceCode));
    }
    return stmtFactory.mk_compound(statements);
}

FunctionType StatementTransformer::transform_function(TSNode tsNode, std::string const& sourceCode)
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
        std::string methodChildType = ts_node_type(methodChild);

        if (methodChildType == "modifiers")
        {
            access = get_access_modifier(methodChild, sourceCode);
        }
        else if ((methodChildType == "identifier"
                  || methodChildType.find("type") != std::string::npos)
                 && ! typeSet)
        {
            type    = this->get_return_type(methodChild, sourceCode);
            typeSet = true;
        }
        else if (methodChildType == "identifier")
        {
            name = exprTransformer->get_node_text(methodChild, sourceCode);
        }
        else if (methodChildType == "formal_parameters")
        {
            uint32_t paramtersCount = ts_node_named_child_count(methodChild);
            for (uint32_t j = 0; j < paramtersCount; j++)
            {
                TSNode parameterNode          = ts_node_named_child(methodChild, j);
                std::string parameterNodeType = ts_node_type(parameterNode);
                params.push_back(this->transform_param_node(parameterNode, sourceCode));
            }
        }
        else if (methodChildType == "constructor_body" || methodChildType == "block")
        {
            uint32_t bodyChildCount = ts_node_named_child_count(methodChild);
            std::vector<astfri::Stmt*> bodyStatements;

            for (uint32_t j = 0; j < bodyChildCount; j++)
            {
                TSNode bodyChild   = ts_node_named_child(methodChild, j);
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

    std::tuple methodBody         = this->transform_function(tsNode, sourceCode);
    access                        = std::get<astfri::AccessModifier>(methodBody);
    type                          = std::get<astfri::Type*>(methodBody);
    name                          = std::get<std::string>(methodBody);
    params                        = std::get<std::vector<astfri::ParamVarDefStmt*>>(methodBody);
    body                          = std::get<astfri::CompoundStmt*>(methodBody);

    astfri::FunctionDefStmt* func = stmtFactory.mk_function_def(name, params, type, body);

    return stmtFactory.mk_method_def(nullptr, func, access, astfri::Virtuality::NotVirtual);
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
    baseInit                   = std::get<std::vector<astfri::BaseInitializerStmt*>>(methodBody);
    params                     = std::get<std::vector<astfri::ParamVarDefStmt*>>(methodBody);
    body                       = std::get<astfri::CompoundStmt*>(methodBody);

    return stmtFactory.mk_constructor_def(nullptr, params, baseInit, body, access);
}

astfri::MemberVarDefStmt* StatementTransformer::transform_attribute_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    astfri::AccessModifier access = astfri::AccessModifier::Private;
    astfri::Type* type            = nullptr;
    astfri::Expr* init            = nullptr;
    std::string name;

    std::string tsNodeType = ts_node_type(tsNode);
    uint32_t childCount    = ts_node_named_child_count(tsNode);
    for (uint32_t j = 0; j < childCount; j++)
    {
        TSNode attrNode          = ts_node_named_child(tsNode, j);
        std::string attrNodeType = ts_node_type(attrNode);

        if (attrNodeType == "modifiers")
        {
            access = get_access_modifier(attrNode, sourceCode);
        }
        else if (attrNodeType.find("type") != std::string::npos)
        {
            type = this->get_return_type(attrNode, sourceCode);
        }
        else if (attrNodeType == "variable_declarator")
        {
            uint32_t declaratorChildCount = ts_node_named_child_count(attrNode);
            for (uint32_t k = 0; k < declaratorChildCount; k++)
            {
                TSNode declaratorChild          = ts_node_named_child(attrNode, k);
                std::string declaratorChildType = ts_node_type(declaratorChild);

                if (declaratorChildType == "identifier" && k == 0)
                {
                    name = exprTransformer->get_node_text(declaratorChild, sourceCode);
                }
                else
                {
                    init = exprTransformer->get_expr(declaratorChild, sourceCode);
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
        name = exprTransformer->get_node_text(ts_node_named_child(tsNode, 0), sourceCode);
    }
    if (! ts_node_is_null(ts_node_named_child(tsNode, 1)))
    {
        constraint = exprTransformer->get_node_text(ts_node_named_child(tsNode, 1), sourceCode);
    }

    return stmtFactory.mk_generic_param(constraint, name);
}

astfri::LambdaExpr* StatementTransformer::transform_lambda_expr_node(
    TSNode tsNode,
    std::string const& sourceCode
)
{
    std::vector<ParamVarDefStmt*> lambdaParams;
    std::vector<ParamVarDefStmt*> funcInterfaceParams;
    Stmt* lambdaBody = nullptr;
    uint32_t lambdaChildCount = ts_node_named_child_count(tsNode);
    astfri::InterfaceDefStmt* funcInterface = nullptr;

    if (! ts_node_is_null(ts_node_parent(tsNode)))
    {
        TSNode lambdaParent = ts_node_parent(tsNode);
        TSNode lambdaParentPrevSibling = ts_node_prev_named_sibling(lambdaParent);
        std::string lambdaParentPrevSiblingType = ts_node_type(lambdaParentPrevSibling);

        for (auto i : this->functionalInterfaces)
        {
            std::string typeName = exprTransformer->get_node_text(lambdaParentPrevSibling, sourceCode);
            if (i->name_ == typeName)
            {
                funcInterface = i;
                break;
            }
        }

        if (funcInterface != nullptr)
                {
                    for (auto m : funcInterface->methods_)
                    {
                        if (m->func_->body_ == nullptr)
                        {
                            for (auto p : m->func_->params_)
                            {
                                funcInterfaceParams.push_back(p);
                            }
                        }
                    }
                }
    }

    for (uint32_t i = 0; i < lambdaChildCount; i++)
    {
        TSNode lambdaChild = ts_node_named_child(tsNode, i);
        std::string lambdaChildType = ts_node_type(lambdaChild);

        if (lambdaChildType == "inferred_parameters")
        {
            uint32_t parameterCount = ts_node_named_child_count(lambdaChild);
            for (uint32_t j = 0; j < parameterCount; j++)
            {
                TSNode parameterNode = ts_node_named_child(lambdaChild, j);
                lambdaParams.push_back(stmtFactory.mk_param_var_def(
                                        exprTransformer->get_node_text(parameterNode, sourceCode),
                                        funcInterfaceParams.empty() ? nullptr : funcInterfaceParams[j]->type_, 
                                        nullptr));
            }
            continue;
        }
        else if (lambdaChildType == "identifier")
        {
            lambdaParams.push_back(stmtFactory.mk_param_var_def(
                                    exprTransformer->get_node_text(lambdaChild, sourceCode), 
                                    funcInterfaceParams.empty() ? nullptr : funcInterfaceParams[0]->type_, 
                                    nullptr));
            continue;
        }
        else if (lambdaChildType == "block")
        {
            lambdaBody = this->transform_body_node(lambdaChild, sourceCode);
        }
        else if (lambdaChildType == "expression_statement") 
        {
            lambdaBody = this->transform_expr_stmt_node(lambdaChild, sourceCode);
        }
        else 
        {
            continue;
        }
    }

    return astfri::ExprFactory::get_instance().mk_lambda_expr(lambdaParams, lambdaBody);
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
            std::string childType = ts_node_type(child);
            if (childType == "class_declaration")
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
            std::string classChildType = ts_node_type(classChild);

            if (classChildType == "modifiers")
            {
                continue;
            }
            else if (classChildType == "identifier")
            {
                className = exprTransformer->get_node_text(classChild, sourceCode);
            }
            else if (classChildType == "type_parameters")
            {
                uint32_t parametersCount = ts_node_named_child_count(classChild);
                for (uint32_t k = 0; k < parametersCount; k++)
                {
                    TSNode parameterNode = ts_node_named_child(classChild, k);
                    tparams.push_back(this->transform_tparam_node(parameterNode, sourceCode));
                }
            }
            else if (classChildType == "superclass")
            {
                bases.push_back(stmtFactory.mk_class_def(
                    exprTransformer->get_node_text(ts_node_named_child(classChild, 0), sourceCode),
                    mk_scope()));
            }
            else if (classChildType == "super_interfaces")
            {
                uint32_t typeListChildCount = ts_node_named_child_count(classChild);
                for (uint32_t k = 0; k < typeListChildCount; k++)
                {
                    TSNode typeListChild = ts_node_named_child(classChild, k);
                    interfaces.push_back(stmtFactory.mk_interface_def(
                        exprTransformer->get_node_text(typeListChild, sourceCode)
                    ));
                }
            }
            else if (classChildType == "class_body")
            {
                uint32_t classBodyChildCount = ts_node_named_child_count(classChild);
                for (uint32_t k = 0; k < classBodyChildCount; k++)
                {
                    TSNode classBodyChild          = ts_node_named_child(classChild, k);
                    std::string classBodyChildType = ts_node_type(classBodyChild);

                    if (classBodyChildType == "field_declaration")
                    {
                        attributes.push_back(
                            this->transform_attribute_node(classBodyChild, sourceCode)
                        );
                    }
                    else if (classBodyChildType == "method_declaration")
                    {
                        methods.push_back(this->transform_method_node(classBodyChild, sourceCode));
                    }
                    else if (classBodyChildType == "constructor_declaration")
                    {
                        constructors.push_back(
                            this->transform_constructor_node(classBodyChild, sourceCode)
                        );
                    }
                }
            }
        }

        astfri::ClassDefStmt* classDef = stmtFactory.mk_class_def(className, mk_scope());
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

std::vector<astfri::InterfaceDefStmt*> StatementTransformer::transform_interfaces(
    TSTree* tree,
    std::string const& sourceCode
)
{
    TSNode rootNode = ts_tree_root_node(tree);
    TSNode interfaceNode;
    std::string interfaceName;
    std::vector<astfri::MethodDefStmt*> methods;
    std::vector<astfri::GenericParam*> tparams;
    std::vector<astfri::InterfaceDefStmt*> bases;
    std::vector<astfri::InterfaceDefStmt*> interfaces;
    bool funcInterface = false;

    uint32_t childCount = ts_node_named_child_count(rootNode);
    for (uint32_t i = 0; i < childCount; i++)
    {
        if (! ts_node_is_null(ts_node_named_child(rootNode, i)))
        {
            TSNode child          = ts_node_named_child(rootNode, i);
            std::string childType = ts_node_type(child);
            if (childType == "interface_declaration")
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
            TSNode interfaceChild          = ts_node_named_child(interfaceNode, j);
            std::string interfaceChildType = ts_node_type(interfaceChild);
            
            if (interfaceChildType == "modifiers")
            {
                uint32_t modifiersChildrenCount = ts_node_child_count(interfaceChild);
                if (modifiersChildrenCount > 0)
                {
                    for (uint32_t k = 0; k < modifiersChildrenCount; ++k)
                    {
                        TSNode modChild = ts_node_child(interfaceChild, k);
                        std::string modChildType = ts_node_type(modChild);
                        std::string annotation = exprTransformer->get_node_text(modChild, sourceCode);
                        if (modChildType == "marker_annotation" && annotation == "@FunctionalInterface")
                        {
                            funcInterface = true;
                        }
                    }
                }
            }
            else if (interfaceChildType == "identifier")
            {
                interfaceName = exprTransformer->get_node_text(interfaceChild, sourceCode);
            }
            else if (interfaceChildType == "type_parameters")
            {
                uint32_t parametersCount = ts_node_named_child_count(interfaceChild);
                for (uint32_t k = 0; k < parametersCount; k++)
                {
                    TSNode parameterNode = ts_node_named_child(interfaceChild, k);
                    tparams.push_back(this->transform_tparam_node(parameterNode, sourceCode));
                }
            }
            else if (interfaceChildType == "extends_interfaces")
            {
                uint32_t typeListChildCount = ts_node_named_child_count(interfaceChild);
                for (uint32_t k = 0; k < typeListChildCount; k++)
                {
                    TSNode typeListChild = ts_node_named_child(interfaceChild, k);
                    bases.push_back(stmtFactory.mk_interface_def(
                        exprTransformer->get_node_text(typeListChild, sourceCode)
                    ));
                }
            }
            else if (interfaceChildType == "interface_body")
            {
                uint32_t interfaceBodyChildCount = ts_node_named_child_count(interfaceChild);
                for (uint32_t k = 0; k < interfaceBodyChildCount; k++)
                {
                    TSNode interfaceBodyChild          = ts_node_named_child(interfaceChild, k);
                    std::string interfaceBodyChildType = ts_node_type(interfaceBodyChild);

                    if (interfaceBodyChildType == "method_declaration")
                    {
                        methods.push_back(
                            this->transform_method_node(interfaceBodyChild, sourceCode)
                        );
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }

        astfri::InterfaceDefStmt* interfaceDef = stmtFactory.mk_interface_def(interfaceName);
        interfaceDef->methods_                 = methods;
        interfaceDef->tparams_                 = tparams;
        interfaceDef->bases_                   = bases;
        interfaces.push_back(interfaceDef);

        if (funcInterface)
        {
            this->functionalInterfaces.push_back(interfaceDef);
            funcInterface = false;
        }

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

astfri::TranslationUnit* StatementTransformer::fill_translation_unit(
    TSTree* tree,
    std::string const& sourceCode
)
{
    astfri::TranslationUnit* tu = this->stmtFactory.mk_translation_unit();
    tu->interfaces_             = this->transform_interfaces(tree, sourceCode);
    tu->classes_                = this->transform_classes(tree, sourceCode);
    return tu;
}
} // namespace astfri::java