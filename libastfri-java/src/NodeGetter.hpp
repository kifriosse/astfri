#ifndef NODE_GETTER_CLASS_HPP
#define NODE_GETTER_CLASS_HPP

#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include "NodeMapper.hpp"
#include "libastfri/inc/Stmt.hpp"

#include <cstring>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <vector>
#include <variant>

using StatementReturnType = std::variant<astfri::LocalVarDefStmt*, astfri::ReturnStmt*, astfri::ExprStmt*, astfri::UnknownStmt*>;

class NodeGetter
{
private:
    astfri::TypeFactory& typeFactory;
    astfri::ExprFactory& exprFactory;
    astfri::StmtFactory& stmtFactory;
    NodeMapper* nodeMapper;
    std::vector<astfri::ClassDefStmt*> classes;
    TSQuery* make_query(char const* queryString);
    std::string get_node_text(const TSNode& node, const std::string& sourceCode);
    astfri::Expr* get_expr(TSNode tsNode, const std::string& sourceCode);
    astfri::BinOpExpr* get_bin_op_expr (TSNode tsNode, const std::string& sourceCode);
    astfri::UnaryOpExpr* get_un_op_expr(std::string qString, TSNode tsNode, const std::string& sourceCode);
    std::variant<astfri::ParamVarRefExpr*, astfri::LocalVarRefExpr*, astfri::MemberVarRefExpr*> get_ref_expr(TSNode tsNode, std::string const& sourceCode);
    astfri::MethodCallExpr* get_method_call(TSNode tsNode, std::string const& sourceCode);
    astfri::NewExpr* get_new_expr(TSNode paramsNode, std::string const& sourceCode);
    std::vector<astfri::ParamVarDefStmt*> get_params (TSNode paramsNode, const std::string& sourceCode);
    astfri::LocalVarDefStmt* get_local_var(TSNode tsNode, std::string const& sourceCode);
    astfri::ExprStmt* get_expr_stmt(TSNode tsNode, std::string const& sourceCode);
    astfri::IfStmt* get_if_stmt(TSNode tsNode, std::string const& sourceCode);
    //astfri::SwitchStmt* get_switch_stmt(TSNode tsNode, std::string const& sourceCode);
    astfri::ForStmt* get_for_stmt(TSNode tsNode, std::string const& sourceCode);
    astfri::WhileStmt* get_while_stmt(TSNode tsNode, std::string const& sourceCode);
    astfri::DoWhileStmt* get_do_while_stmt(TSNode tsNode, std::string const& sourceCode);
    astfri::ReturnStmt* get_return_stmt(TSNode tsNode, std::string const& sourceCode);
    astfri::CompoundStmt* search_sub_tree (TSNode tsNode, const std::string& sourceCode);
    astfri::ConstructorDefStmt* get_constructor (TSNode classNode, const std::string& sourceCode);
    astfri::MethodDefStmt* get_method (TSNode tsNode, const std::string& sourceCode);
    astfri::MemberVarDefStmt* get_attribute (TSNode tsNode, const std::string& sourceCode);
    std::vector<astfri::ClassDefStmt*> get_classes (TSTree* tree, const std::string& sourceCode);
public:
    NodeGetter(TSTree* tree, const std::string& sourceCode);
    std::vector<astfri::ClassDefStmt*> get_classes ();
};

#endif // NODE_GETTER_CLASS_HPP