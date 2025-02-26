#ifndef NODE_MAPPER_CLASS_HPP
#define NODE_MAPPER_CLASS_HPP

#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>

#include <map>
#include <variant>
#include <vector>
#include <functional>

class NodeMapper {
    private:
        std::map<std::string, astfri::Type*> typeMap;
        using StmtArguments = std::variant<
        std::tuple<std::string, astfri::Type*, astfri::Expr*>,
        std::tuple<std::string, std::vector<astfri::MemberVarDefStmt*>, std::vector<astfri::MethodDefStmt*>, std::vector<astfri::GenericParam*>>,
        std::tuple<astfri::ClassDefStmt*, astfri::FunctionDefStmt*, astfri::AccessModifier>, 
        std::tuple<std::string, std::vector<astfri::ParamVarDefStmt*>, astfri::Type*, astfri::CompoundStmt*>,
        std::tuple<std::vector<astfri::Stmt*>>, 
        std::tuple<astfri::Expr*>,
        std::tuple<astfri::Expr*, astfri::Stmt*, astfri::Stmt*>,
        std::tuple<astfri::Expr*, astfri::Stmt*>,
        std::tuple<astfri::Expr*, std::vector<astfri::CaseStmt*>>,
        std::tuple<astfri::Expr*, astfri::CompoundStmt*>,
        std::tuple<astfri::Stmt*, astfri::Expr*, astfri::Stmt*, astfri::CompoundStmt*>,
        std::tuple<std::string, astfri::Type*, astfri::Expr*, astfri::AccessModifier>
        >;
        
        std::map<std::string, std::function<astfri::Stmt*(NodeMapper::StmtArguments)>> stmtMap;
        astfri::StmtFactory& stmtFactory;
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_local_variable();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_member_variable();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_formal_parameter();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_class_node();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_method_node();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_body_node();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_return_node();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_expression_statement();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_if_statement();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_case_node();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_switch_node();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_while_node();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_do_while_node();
        std::function<astfri::Stmt*(NodeMapper::StmtArguments)> map_for_node();
        
        using ExprArguments = std::variant<
        std::tuple<int>,
        std::tuple<float>,
        std::tuple<char>,
        std::tuple<std::string>,
        std::tuple<bool>,
        std::tuple<astfri::Expr*, astfri::Expr*, astfri::Expr*>,
        std::tuple<astfri::Expr*, astfri::BinOpType, astfri::Expr*>,
        std::tuple<astfri::UnaryOpType, astfri::Expr*>,
        std::tuple<astfri::Expr*, astfri::Expr*>
        >;
        
        std::map<std::string, std::function<astfri::Expr*(NodeMapper::ExprArguments)>> exprMap;
        astfri::ExprFactory& exprFactory;
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_integer_literal();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_float_literal();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_char_literal();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_string_literal();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_boolean_literal();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_binary_operation();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_unary_operation();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_assignment();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_compound_assignment();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_parameter_variable_reference();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_local_variable_reference();
        std::function<astfri::Expr*(NodeMapper::ExprArguments)> map_member_variable_reference();
    
    public:
        NodeMapper();
        std::map<std::string, astfri::Type*> get_typeMap();
        std::map<std::string, std::function<astfri::Expr*(NodeMapper::ExprArguments)>> get_exprMap();
        std::map<std::string, std::function<astfri::Stmt*(NodeMapper::StmtArguments)>> get_stmtMap();
};
#endif // NODE_MAPPER_CLASS_HPP