#ifndef NODE_MAPPER_CLASS_HPP
#define NODE_MAPPER_CLASS_HPP

#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <functional>
#include <map>
#include <variant>
#include <vector>

using StmtArguments = std::variant<
    std::tuple<std::string, astfri::Type*, astfri::Expr*>,
    std::tuple<
        std::string,
        std::vector<astfri::MemberVarDefStmt*>,
        std::vector<astfri::MethodDefStmt*>,
        std::vector<astfri::GenericParam*>>,
    std::tuple<
        astfri::ClassDefStmt*,
        astfri::FunctionDefStmt*,
        astfri::AccessModifier>,
    std::tuple<
        std::string,
        std::vector<astfri::ParamVarDefStmt*>,
        astfri::Type*,
        astfri::CompoundStmt*>,
    std::tuple<std::vector<astfri::Stmt*>>,
    std::tuple<astfri::Expr*>,
    std::tuple<astfri::Expr*, astfri::Stmt*, astfri::Stmt*>,
    std::tuple<astfri::Expr*, astfri::Stmt*>,
    std::tuple<astfri::Expr*, std::vector<astfri::CaseStmt*>>,
    std::tuple<astfri::Expr*, astfri::CompoundStmt*>,
    std::tuple<
        astfri::Stmt*,
        astfri::Expr*,
        astfri::Stmt*,
        astfri::CompoundStmt*>,
    std::tuple<
        std::string,
        astfri::Type*,
        astfri::Expr*,
        astfri::AccessModifier>>;

using ExprArguments = std::variant<
    std::tuple<int>,
    std::tuple<float>,
    std::tuple<char>,
    std::tuple<std::string>,
    std::tuple<bool>,
    std::tuple<astfri::Expr*, astfri::Expr*, astfri::Expr*>,
    std::tuple<astfri::Expr*, astfri::BinOpType, astfri::Expr*>,
    std::tuple<astfri::UnaryOpType, astfri::Expr*>,
    std::tuple<astfri::Expr*, astfri::Expr*>>;

class NodeMapper
{
private:
    astfri::TypeFactory& typeFactory;
    const std::map<std::string, astfri::Type*> typeMap;

    astfri::StmtFactory& stmtFactory;
    const std::map<std::string, std::function<astfri::Stmt*(StmtArguments)>> stmtMap;
    std::function<astfri::Stmt*(StmtArguments)> map_local_variable ();
    std::function<astfri::Stmt*(StmtArguments)> map_member_variable ();
    std::function<astfri::Stmt*(StmtArguments)> map_formal_parameter ();
    std::function<astfri::Stmt*(StmtArguments)> map_class_node ();
    std::function<astfri::Stmt*(StmtArguments)> map_method_node ();
    std::function<astfri::Stmt*(StmtArguments)> map_body_node ();
    std::function<astfri::Stmt*(StmtArguments)> map_return_node ();
    std::function<astfri::Stmt*(StmtArguments)> map_expression_statement ();
    std::function<astfri::Stmt*(StmtArguments)> map_if_statement ();
    std::function<astfri::Stmt*(StmtArguments)> map_case_node ();
    std::function<astfri::Stmt*(StmtArguments)> map_switch_node ();
    std::function<astfri::Stmt*(StmtArguments)> map_while_node ();
    std::function<astfri::Stmt*(StmtArguments)> map_do_while_node ();
    std::function<astfri::Stmt*(StmtArguments)> map_for_node ();

    astfri::ExprFactory& exprFactory;
    const std::map<std::string, std::function<astfri::Expr*(ExprArguments)>> exprMap;
    std::function<astfri::Expr*(ExprArguments)> map_integer_literal ();
    std::function<astfri::Expr*(ExprArguments)> map_float_literal ();
    std::function<astfri::Expr*(ExprArguments)> map_char_literal ();
    std::function<astfri::Expr*(ExprArguments)> map_string_literal ();
    std::function<astfri::Expr*(ExprArguments)> map_boolean_literal ();
    std::function<astfri::Expr*(ExprArguments)> map_binary_operation ();
    std::function<astfri::Expr*(ExprArguments)> map_unary_operation ();
    std::function<astfri::Expr*(ExprArguments)> map_assignment ();
    std::function<astfri::Expr*(ExprArguments)> map_compound_assignment ();
    std::function<astfri::Expr*(ExprArguments)> map_parameter_variable_reference ();
    std::function<astfri::Expr*(ExprArguments)> map_local_variable_reference ();
    std::function<astfri::Expr*(ExprArguments)> map_member_variable_reference (
    );

    const std::map<std::string, astfri::BinOpType> binOpMap;
    const std::map<std::string, astfri::UnaryOpType> unaryOpMap;

public:
    NodeMapper();
    std::map<std::string, astfri::Type*> get_typeMap ();
    std::map<std::string, std::function<astfri::Expr*(ExprArguments)>> get_exprMap ();
    std::map<std::string, std::function<astfri::Stmt*(StmtArguments)>> get_stmtMap ();
    std::map<std::string, astfri::BinOpType> get_binOpMap ();
    std::map<std::string, astfri::UnaryOpType> get_unaryOpMap ();
};
#endif // NODE_MAPPER_CLASS_HPP