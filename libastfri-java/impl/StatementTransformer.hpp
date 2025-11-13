#ifndef STATEMENT_TRANSFORMER_CLASS_HPP
#define STATEMENT_TRANSFORMER_CLASS_HPP

#include <libastfri/inc/Astfri.hpp>
#include <libastfri-java/impl/ExpressionTransformer.hpp>
#include <libastfri-java/impl/NodeMapper.hpp>

#include <cstring>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <vector>
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/Type.hpp"

namespace astfri::java
{

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
    astfri::StmtFactory& stmtFactory;
    ExpressionTransformer* exprTransformer;
    NodeMapper* nodeMapper;
    std::vector<astfri::InterfaceDefStmt*> functionalInterfaces;
    std::vector<astfri::ClassDefStmt*> classes;
    std::vector<astfri::InterfaceDefStmt*> interfaces;

    astfri::Stmt* get_stmt(TSNode tsNode, std::string const& sourceCode);

    astfri::AccessModifier get_access_modifier(TSNode tsNode, std::string const& sourceCode);

    astfri::Type* get_return_type(TSNode tsNode, std::string const& sourceCode);

    astfri::ParamVarDefStmt* transform_param_node(TSNode tsNode, std::string const& sourceCode);

    astfri::LocalVarDefStmt* transform_local_var_node(TSNode tsNode, std::string const& sourceCode);

    astfri::ExprStmt* transform_expr_stmt_node(TSNode tsNode, std::string const& sourceCode);

    astfri::IfStmt* transform_if_stmt_node(TSNode tsNode, std::string const& sourceCode);

    astfri::SwitchStmt* transform_switch_stmt_node(TSNode tsNode, std::string const& sourceCode);

    astfri::ForStmt* transform_for_stmt_node(TSNode tsNode, std::string const& sourceCode);

    astfri::WhileStmt* transform_while_stmt_node(TSNode tsNode, std::string const& sourceCode);

    astfri::DoWhileStmt* transform_do_while_stmt_node(TSNode tsNode, std::string const& sourceCode);

    astfri::ReturnStmt* transform_return_stmt_node(TSNode tsNode, std::string const& sourceCode);

    astfri::CompoundStmt* transform_body_node(TSNode tsNode, std::string const& sourceCode);
    
    FunctionType transform_function(TSNode tsNode, std::string const& sourceCode);
    
    astfri::MethodDefStmt* transform_method_node(TSNode tsNode, std::string const& sourceCode);
    
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
        std::string const& sourceCode);

    astfri::Scope get_scope(
        TSNode tsNode,
        std::string const& sourceCode);

    astfri::ClassDefStmt* transform_class(
        TSNode tsNode,
        std::string const& sourceCode
    );

    std::vector<astfri::ClassDefStmt*> transform_classes(
        TSTree* tree,
        std::string const& sourceCode
    );

    astfri::InterfaceDefStmt* transform_interface(
        TSNode tsNode,
        std::string const& sourceCode
    );
    
    std::vector<astfri::InterfaceDefStmt*> transform_interfaces(
        TSTree* tree,
        std::string const& sourceCode
    );
    
    public:
    StatementTransformer();
    ~StatementTransformer();

    astfri::LambdaExpr* transform_lambda_expr_node(
    TSNode tsNode,
    std::string const& sourceCode
    );
    
    astfri::TranslationUnit* fill_translation_unit(TSTree* tree, std::string const& sourceCode);
};

} // namespace astfri::java
#endif // STATEMENT_TRANSFORMER_CLASS_HPP