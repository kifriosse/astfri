#include "Transformer.hpp"
#include <vector>
#include "libastfri-java/src/NodeGetter.hpp"
#include "libastfri-java/src/NodeMapper.hpp"
#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/ExprFactory.hpp"
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/StmtFactory.hpp"
#include "libastfri/inc/Type.hpp"
#include "libastfri/inc/TypeFactory.hpp"

Transformer::Transformer(std::string const& path) : stmtFactory(astfri::StmtFactory::get_instance()), typeFactory(astfri::TypeFactory::get_instance()), exprFactory(astfri::ExprFactory::get_instance())
{
    TreeMaker* treeMaker = new TreeMaker(path);
    NodeGetter* nodeGetter = new NodeGetter(treeMaker->get_syntax_tree(), treeMaker->get_source_code());
    NodeMapper* nodeMapper = new NodeMapper();
}

std::vector<astfri::Stmt*> Transformer::transform_member_vars(std::vector<Member_variable_node> memberVarNodes) 
{
    std::vector<astfri::Stmt*> memberVariables;
    astfri::AccessModifier access;
    std::map<std::string, astfri::Type*>::iterator typeIt;
    astfri::Type* type;
    astfri::Expr* initializer;
    for (Member_variable_node var : memberVarNodes) 
    {
        for (std::string modifier : var.modifiers) {
            if (modifier == "public") {
                access = astfri::AccessModifier::Public;
            } else if (modifier == "private") {
                access = astfri::AccessModifier::Private;
            } else if (modifier == "protected") {
                access = astfri::AccessModifier::Protected;
            } else {
                access = astfri::AccessModifier::Internal;
            }
        }
        typeIt = nodeMapper->get_typeMap().find(var.type);
        if (typeIt != nodeMapper->get_typeMap().end()) {
            type = typeIt->second;
        } else {
            type = typeFactory.mk_unknown();
        }
        
        auto exprIt = nodeMapper->get_exprMap().find(var.literal);
        if (exprIt != nodeMapper->get_exprMap().end()) {
            initializer = exprIt->second(var.value);
        } else {
            initializer = nullptr;
        }

        memberVariables.push_back(stmtFactory.mk_member_var_def(var.name, type, initializer, access));
    }

    return memberVariables;
}

std::vector<astfri::Stmt*> Transformer::transform_local_vars(std::vector<Local_variable_node> localVarNodes) 
{
    std::vector<astfri::Stmt*> localVariables;
    astfri::Type* type;
    astfri::Expr* initializer;
    std::map<std::string, astfri::Type*>::iterator typeIt;
    for (Local_variable_node var : localVarNodes) 
    {
        typeIt = nodeMapper->get_typeMap().find(var.type);
        if (typeIt != nodeMapper->get_typeMap().end()) {
            type = typeIt->second;
        } else {
            type = typeFactory.mk_unknown();
        }
        
        auto exprIt = nodeMapper->get_exprMap().find(var.literal);
        if (exprIt != nodeMapper->get_exprMap().end()) {
            initializer = exprIt->second(var.value);
        } else {
            initializer = nullptr;
        }

        localVariables.push_back(stmtFactory.mk_local_var_def(var.name, type, initializer));
    }

    return localVariables;
}

std::vector<astfri::Stmt*> Transformer::transform_formal_params(std::vector<Parameter_node> formalParamNodes) 
{
    std::vector<astfri::Stmt*> formalParams;
    astfri::Type* type;
    astfri::Expr* initializer;
    std::map<std::string, astfri::Type*>::iterator typeIt;
    for (Parameter_node param : formalParamNodes) 
    {
        typeIt = nodeMapper->get_typeMap().find(param.type);
        if (typeIt != nodeMapper->get_typeMap().end()) {
            type = typeIt->second;
        } else {
            type = typeFactory.mk_unknown();
        }
        
        auto exprIt = nodeMapper->get_exprMap().find(param.literal);
        if (exprIt != nodeMapper->get_exprMap().end()) {
            initializer = exprIt->second(param.value);
        } else {
            initializer = nullptr;
        }

        formalParams.push_back(stmtFactory.mk_param_var_def(param.name, type, initializer));
    }

    return formalParams;
}