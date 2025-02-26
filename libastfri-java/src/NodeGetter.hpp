#ifndef NODE_GETTER_CLASS_HPP
#define NODE_GETTER_CLASS_HPP

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
    std::vector<Class_node> classes;
    TSQuery* make_query(char const* queryString);
    std::string get_node_text(const TSNode& node, const std::string& sourceCode);
    std::vector<Parameter_node> get_params (TSNode paramsNode, const std::string& sourceCode);
    std::vector<Local_variable_node> get_local_vars (TSNode methodNode, const std::string& sourceCode);
    std::vector<Method_node> get_methods (TSNode classNode, const std::string& sourceCode);
    std::vector<Member_variable_node> get_member_vars (TSNode classNode, const std::string& sourceCode);
    std::vector<Class_node> get_classes (TSTree* tree, const std::string& sourceCode);
public:
    NodeGetter(TSTree* tree, const std::string& sourceCode);
    std::vector<Class_node> get_classes();
};

#endif // NODE_GETTER_CLASS_HPP