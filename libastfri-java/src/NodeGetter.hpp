#ifndef NODE_GETTER_CLASS_HPP
#define NODE_GETTER_CLASS_HPP

#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <vector>

typedef struct Member_variable_node
{
    std::vector<std::string> modifiers;
    std::string type;
    std::string name;
    std::string value;
} Member_variable_node;

typedef struct Local_variable_node
{
    std::string type;
    std::string name;
    std::string value;
} Variable_node;

typedef struct Parameter_node
{
    std::string type;
    std::string name;
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
    TSTree* get_syntax_tree (char const* sourceCode);
    TSQuery* make_query(char const* queryString);
    std::vector<Parameter_node> get_params (TSNode paramsNode);
    std::vector<Local_variable_node> get_local_vars (TSNode methodNode);
    std::vector<Method_node> get_methods (TSNode classNode);
    std::vector<Member_variable_node> get_member_vars (TSNode classNode);
    std::vector<Class_node> get_classes (TSTree* tree);
public:
    NodeGetter();
};

#endif // NODE_GETTER_CLASS_HPP