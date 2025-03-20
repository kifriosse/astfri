#ifndef STATEMENT_TRANSFORMER_CLASS_HPP
#define STATEMENT_TRANSFORMER_CLASS_HPP

#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <cstring>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <vector>


#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/Type.hpp"
#include "NodeMapper.hpp"

class ExpressionTransformer;

using FunctionType = std::tuple<
    astfri::AccessModifier,
    astfri::Type*,
    std::string,
    std::vector<astfri::ParamVarDefStmt*>,
    std::vector<astfri::BaseInitializerStmt*>,
    astfri::CompoundStmt*>;

class StatementTransformer
{
private:
    astfri::TypeFactory& typeFactory;
    astfri::ExprFactory& exprFactory;
    astfri::StmtFactory& stmtFactory;
    ExpressionTransformer* exprTransformer;
    NodeMapper* nodeMapper;

    std::vector<astfri::ClassDefStmt*> classes;
    std::vector<astfri::InterfaceDefStmt*> interfaces;

    astfri::Stmt* get_stmt(TSNode tsNode, std::string const& sourceCode);

    astfri::AccessModifier get_access_modifier(
        TSNode tsNode,
        std::string const& sourceCode
    );
    
    astfri::Type* get_return_type(
        TSNode tsNode,
        std::string const& sourceCode
    );
    
    astfri::ParamVarDefStmt* transform_param_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::LocalVarDefStmt* transform_local_var_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::ExprStmt* transform_expr_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::IfStmt* transform_if_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::SwitchStmt* transform_switch_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::ForStmt* transform_for_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::WhileStmt* transform_while_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::DoWhileStmt* transform_do_while_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::ReturnStmt* transform_return_stmt_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::CompoundStmt* transform_body_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    FunctionType transform_function(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::MethodDefStmt* transform_method_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::ConstructorDefStmt* transform_constructor_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::MemberVarDefStmt* transform_attribute_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    astfri::GenericParam* transform_tparam_node(
        TSNode tsNode,
        std::string const& sourceCode
    );

    std::vector<astfri::ClassDefStmt*> transform_classes(
        TSTree* tree,
        std::string const& sourceCode
    );

    std::vector<astfri::InterfaceDefStmt*> transform_interfaces(
        TSTree* tree,
        std::string const& sourceCode
    );

public:
    StatementTransformer(TSTree* tree, std::string const& sourceCode);
    std::vector<astfri::ClassDefStmt*> get_classes();
    std::vector<astfri::InterfaceDefStmt*> get_interfaces();
};

#endif // STATEMENT_TRANSFORMER_CLASS_HPP