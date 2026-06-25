#ifndef ASTFRI_IMPL_STATEMENT_TRANSFORMER_HPP
#define ASTFRI_IMPL_STATEMENT_TRANSFORMER_HPP

#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/Scope.hpp>
#include <astfri/impl/StmtFwd.hpp>
#include <astfri/impl/TypeFwd.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>


namespace astfri::java {


class ExpressionTransformer;


class NodeMapper;


using FunctionType = std::tuple<
    astfri::AccessModifier,
    astfri::Type*,
    std::string,
    std::vector<astfri::ParamVarDefStmt*>,
    std::vector<astfri::BaseInitializerStmt*>,
    astfri::CompoundStmt*>;


class StatementTransformer {
public:
    StatementTransformer(
        astfri::ExprFactory *exprFactory,
        astfri::StmtFactory *stmtFactory,
        astfri::TypeFactory *typeFactory,
        ExpressionTransformer *exprTransformer,
        NodeMapper *nodeMapper);

    astfri::TranslationUnit fill_translation_unit(TSTree *tree, const std::string &sourceCode);

private:
    astfri::LambdaExpr* transform_lambda_expr_node(TSNode tsNode, const std::string& sourceCode);

    astfri::Stmt* get_stmt(TSNode tsNode, const std::string& sourceCode);

    astfri::AccessModifier get_access_modifier(TSNode tsNode, const std::string& sourceCode);

    astfri::Type* get_return_type(TSNode tsNode, const std::string& sourceCode);

    astfri::ParamVarDefStmt* transform_param_node(TSNode tsNode, const std::string& sourceCode);

    astfri::LocalVarDefStmt* transform_local_var_node(TSNode tsNode, const std::string& sourceCode);

    astfri::ExprStmt* transform_expr_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::IfStmt* transform_if_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::TryStmt* transform_try_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::CatchStmt* transform_catch_clause_node(TSNode tsNode, const std::string& sourceCode);

    astfri::SwitchStmt* transform_switch_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::ForStmt* transform_for_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::WhileStmt* transform_while_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::DoWhileStmt* transform_do_while_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::ForEachStmt* transform_foreach_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::ReturnStmt* transform_return_stmt_node(TSNode tsNode, const std::string& sourceCode);

    astfri::BaseInitializerStmt* transform_explicit_constructor_invocation(TSNode tsNode, const std::string& sourceCode);

    astfri::CompoundStmt* transform_body_node(TSNode tsNode, const std::string& sourceCode);

    FunctionType transform_function(TSNode tsNode, const std::string& sourceCode);

    astfri::MethodDefStmt* transform_method_node(TSNode tsNode, const std::string& sourceCode);

    astfri::ConstructorDefStmt* transform_constructor_node(
        TSNode tsNode,
        const std::string& sourceCode
    );

    astfri::MemberVarDefStmt* transform_attribute_node(
        TSNode tsNode,
        const std::string& sourceCode
    );

    astfri::GenericParam* transform_tparam_node(TSNode tsNode, const std::string& sourceCode);

    astfri::Scope get_scope(TSNode tsNode, const std::string& sourceCode);

    void fill_class(
        astfri::ClassDefStmt* classDef,
        TSNode classNode,
        const std::string& sourceCode);

    void fill_interface(
    astfri::InterfaceDefStmt* classDef,
    TSNode classNode,
    const std::string& sourceCode);

    astfri::ClassDefStmt* transform_class(TSNode tsNode, const std::string& sourceCode);

    std::vector<astfri::ClassDefStmt*> transform_classes(
        TSTree* tree,
        const std::string& sourceCode
    );

    astfri::InterfaceDefStmt* transform_interface(TSNode tsNode, const std::string& sourceCode);

    std::vector<astfri::InterfaceDefStmt*> transform_interfaces(
        TSTree* tree,
        const std::string& sourceCode
    );

private:
    long long lambdaID{0};

    astfri::ExprFactory *m_exprFactory;
    astfri::StmtFactory *m_stmtFactory;
    astfri::TypeFactory *m_typeFactory;
    ExpressionTransformer *m_exprTransformer;
    NodeMapper *m_nodeMapper;

    std::vector<astfri::ClassDefStmt*> m_classes;
    std::vector<astfri::InterfaceDefStmt*> m_interfaces;
    std::vector<astfri::InterfaceDefStmt*> m_functionalInterfaces;

    std::unordered_map<astfri::ClassDefStmt*, TSNode> m_clsNodes;
    std::unordered_map<astfri::InterfaceDefStmt*, TSNode> m_ifaceNodes;

    std::unordered_map<std::string, std::vector<astfri::ClassDefStmt*>> m_classesByName;
    std::unordered_map<std::string, std::vector<astfri::InterfaceDefStmt*>> m_interfacesByName;
    std::unordered_map<astfri::ClassDefStmt*, astfri::Scope> m_classScope;
    std::unordered_map<astfri::InterfaceDefStmt*, astfri::Scope> m_interfaceScope;

    std::unordered_map<std::string, std::vector<astfri::MethodDefStmt*>> m_methodsByName;
};


} // namespace astfri::java

#endif // ASTFRI_IMPL_STATEMENT_TRANSFORMER_HPP
