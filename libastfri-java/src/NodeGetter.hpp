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

typedef struct Member_variable_node
{
    std::vector<std::string> modifiers;
    std::string type;
    std::string name;
    std::string literal;
    std::string value;
} Member_variable_node;

typedef struct Local_variable_node
{
    std::string type;
    std::string name;
    std::string literal;
    std::string value;
} Variable_node;

typedef struct Parameter_node
{
    std::string type;
    std::string name;
    std::string literal;
    std::string value;
} Parameter_node;

typedef struct Method_node
{
    std::vector<std::string> modifiers;
    std::string returnType;
    std::string name;
    std::vector<Parameter_node> params;
    std::vector<Local_variable_node> vars;
} Function_node;

typedef struct Class_node
{
    std::vector<std::string> modifiers;
    std::string name;
    std::vector<Member_variable_node> vars;
    std::vector<Method_node> methods;
} Class_node;

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
    std::variant<astfri::AssignExpr*, astfri::CompoundAssignExpr*> get_assign_expr(TSNode tsNode, std::string const& sourceCode);
    astfri::ParamVarRefExpr* get_param_ref(TSNode tsNode, std::string const& sourceCode);
    std::variant<astfri::ParamVarRefExpr*, astfri::LocalVarRefExpr*, astfri::MemberVarRefExpr*> get_ref_expr(TSNode tsNode, std::string const& sourceCode);
    astfri::MethodCallExpr* get_method_call(TSNode tsNode, std::string const& sourceCode);
    std::vector<astfri::ParamVarDefStmt*> get_params (TSNode paramsNode, const std::string& sourceCode);
    std::vector<astfri::LocalVarDefStmt*> get_local_vars (TSNode methodNode, const std::string& sourceCode);
    astfri::ConstructorDefStmt* get_constructor (TSNode classNode, const std::string& sourceCode);
    std::vector<astfri::MethodDefStmt*> get_methods (TSNode classNode, const std::string& sourceCode);
    std::vector<astfri::MemberVarDefStmt*> get_member_vars (TSNode classNode, const std::string& sourceCode);
    std::vector<astfri::ClassDefStmt*> get_classes (TSTree* tree, const std::string& sourceCode);
public:
    NodeGetter(TSTree* tree, const std::string& sourceCode);
    std::vector<astfri::ClassDefStmt*> get_classes ();
};

#endif // NODE_GETTER_CLASS_HPP