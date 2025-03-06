#ifndef NODE_GETTER_CLASS_HPP
#define NODE_GETTER_CLASS_HPP

#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include "NodeMapper.hpp"

#include <cstring>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <vector>
#include <variant>

using StatementReturnType = std::variant<std::vector<astfri::LocalVarDefStmt*>, astfri::ReturnStmt*, astfri::ExprStmt*>;

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
    astfri::Expr* get_expr(std::string const& nodeName, std::string const& nodeText, TSNode tsNode, const std::string& sourceCode);
    astfri::BinOpExpr* get_bin_op_expr (TSNode tsNode, const std::string& sourceCode);
    astfri::UnaryOpExpr* get_un_op_expr(TSNode tsNode, const std::string& sourceCode);
    // std::variant<astfri::AssignExpr*, astfri::CompoundAssignExpr*> get_assign_expr(TSNode tsNode, std::string const& sourceCode);
    astfri::ParamVarRefExpr* get_param_ref(TSNode tsNode, std::string const& sourceCode);
    std::variant<astfri::ParamVarRefExpr*, astfri::LocalVarRefExpr*, astfri::MemberVarRefExpr*> get_ref_expr(TSNode tsNode, std::string const& sourceCode);
    astfri::MethodCallExpr* get_method_call(TSNode tsNode, std::string const& sourceCode);
    astfri::NewExpr* get_new_expr(TSNode paramsNode, std::string const& sourceCode);
    std::vector<astfri::ParamVarDefStmt*> get_params (TSNode paramsNode, const std::string& sourceCode);
    StatementReturnType search_sub_tree (TSNode tsNode, std::string const& nodeName, const std::string& sourceCode);
    astfri::ConstructorDefStmt* get_constructor (TSNode classNode, const std::string& sourceCode);
    std::vector<astfri::MethodDefStmt*> get_methods (TSNode classNode, const std::string& sourceCode);
    std::vector<astfri::MemberVarDefStmt*> get_member_vars (TSNode classNode, const std::string& sourceCode);
    std::vector<astfri::ClassDefStmt*> get_classes (TSTree* tree, const std::string& sourceCode);
public:
    NodeGetter(TSTree* tree, const std::string& sourceCode);
    std::vector<astfri::ClassDefStmt*> get_classes ();
};

#endif // NODE_GETTER_CLASS_HPP