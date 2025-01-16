#include <cstddef>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

typedef struct Tree_Sitter_Node {
  TSNode node;
  std::string type;
  std::vector<TSNode> children;
} Tree_Sitter_Node;

// function for loading content of a file into string
char* load(const char* path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::string error = "Could not open file: " + std::string(path);
    char* errorMsg = new char[error.size() + 1];
    strcpy(errorMsg, error.c_str());
    return errorMsg;
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  std::string str = buffer.str();

  char* content = new char[str.size() + 1];
  strcpy(content, str.c_str());
  return content;
}

std::string get_directory(const std::string& file_path) {
    size_t last_slash_idx = file_path.find_last_of("/\\");
    if (std::string::npos != last_slash_idx) {
        return file_path.substr(0, last_slash_idx + 1);
    }
    return "./";
}

std::vector<TSNode> get_nodes(TSNode node) {
  std::vector<TSNode> nodes;
  nodes.push_back(node);

  uint32_t child_count = ts_node_child_count(node);

  for (uint32_t i = 0; i < child_count; i++) {
    TSNode child = ts_node_child(node, i);
    std::vector<TSNode> child_nodes = get_nodes(child);
    nodes.insert(nodes.begin(), child_nodes.begin(), child_nodes.end());
  }

  return nodes;
}

std::vector<TSNode> get_named_nodes(TSNode node) {
    std::vector<TSNode> nodes;

    if (ts_node_is_named(node)) {
        nodes.push_back(node);
    }

    uint32_t child_count = ts_node_child_count(node);

    for (uint32_t i = 0; i < child_count; i++) {
        TSNode child = ts_node_child(node, i);
        std::vector<TSNode> child_nodes = get_named_nodes(child);
        nodes.insert(nodes.end(), child_nodes.begin(), child_nodes.end());
    }

    return nodes;
}

void print_nodes(std::vector<TSNode> nodes, TSNode root) {
  nodes = get_nodes(root);

  for (int i = nodes.size() - 1; i >= 0; --i) {
    printf("%s\n", ts_node_type(nodes[i]));
  }
}

void print_named_nodes(std::vector<TSNode> nodes, TSNode root) {
  nodes = get_nodes(root);

  for (int i = nodes.size() - 1; i >= 0; --i) {
    if (ts_node_is_named(nodes[i])) {
      printf("%s\n", ts_node_type(nodes[i]));
    } else {}
  }

}

std::string get_node_type(TSNode node) {
  return ts_node_type(node);
}

int main() {
  // Create a parser.
  TSParser *parser = ts_parser_new();

  // Set the parser's language (JAVA in this case).
  ts_parser_set_language(parser, tree_sitter_java());

  // Build a syntax tree based on source code stored in a string.
  std::string current_dir = get_directory(__FILE__);
    
    // Append the text file name
    std::string path = current_dir + "Dummy.txt";
  const char *source_code = load(path.c_str());
  std::cout << load(path.c_str()) << std::endl;
  TSTree *tree = ts_parser_parse_string(
    parser,
    NULL,
    source_code,
    strlen(source_code)
  );

  // Get the root node of the syntax tree.
  TSNode root_node = ts_tree_root_node(tree);
  std::cout << "Root node: " << ts_node_type(root_node) << std::endl;
  TSNode next_node = ts_node_named_child(root_node, 0);
  std::cout << "Next node: " << ts_node_type(next_node) << std::endl;
  TSNode body_node = ts_node_named_child(next_node, 1);
  TSNode body_sibling = ts_node_next_sibling(body_node);
  std::cout << "Next sibling node: " << ts_node_type(body_sibling) << std::endl;
  std::cout << "Children count " << ts_node_named_child_count(next_node) << std::endl;

  // Print the syntax tree as an S-expression.
  char *string = ts_node_string(TSNode(ts_tree_root_node(tree)));
  printf("Syntax tree: %s\n", string);

  // Get all nodes in the tree
  std::vector<TSNode> nodes = get_nodes(root_node);
  print_named_nodes(nodes, root_node);

  std::vector<TSNode> named_nodes = get_named_nodes(root_node);

  //Wallking through tree
  TSTreeCursor cursor = ts_tree_cursor_new(root_node);
  bool result = ts_tree_cursor_goto_first_child(&cursor);
  std::cout << result << std::endl;
  std::cout << ts_node_type(ts_tree_cursor_current_node(&cursor)) << std::endl;

  auto& exprFactory = astfri::ExprFactory::get_instance();
  auto& stmtFactory = astfri::StmtFactory::get_instance();
  auto& typeFactory = astfri::TypeFactory::get_instance();

  std::vector<astfri::FunctionDefStmt*> functions;
  std::vector<astfri::ParamVarDefStmt*> params;

  for (ulong i = 0; i < named_nodes.size(); i++) {
    if (ts_node_child_count(nodes[i]) > 0) {
        Tree_Sitter_Node node;
        node.node = nodes[i];
        node.type = ts_node_type(nodes[i]);
        
        // Get child nodes and filter out "modifiers" if present
        std::vector<TSNode> children = get_nodes(nodes[i]);
        if (std::string_view(ts_node_type(children[0])) == "modifiers") { // string_view
            children.erase(children.begin());
        }

        astfri::Type* function_return_type = typeFactory.mk_void();
        
        // Check if the node is a method declaration
        if (node.type == "method_declaration") {
            if (std::string_view(ts_node_type(children[0])) == "integral_type") {
                function_return_type = typeFactory.mk_int();
            }

            // Extract function name
            TSNode identifier_node = ts_node_child_by_field_name(children[1], "identifier", strlen("identifier"));
            std::string function_name = ts_node_string(identifier_node);

            // Extract parameters
            Tree_Sitter_Node parameters;
            parameters.node = children[2];
            parameters.type = ts_node_type(children[2]);
            parameters.children = get_named_nodes(children[2]);

            for (auto& param : parameters.children) {
                Tree_Sitter_Node param_node;
                param_node.node = param;
                param_node.type = ts_node_type(param);
                param_node.children = get_named_nodes(param);

                astfri::Type* param_type = nullptr;
                if (std::string_view(ts_node_type(param_node.children[0])) == "integral_type") {
                    param_type = typeFactory.mk_int();
                }

                // Extract parameter name
                TSNode param_identifier_node = ts_node_child_by_field_name(param_node.children[1], "identifier", strlen("identifier"));
                std::string param_name = ts_node_string(param_identifier_node);
                auto parameter = stmtFactory.mk_param_var_def(param_name, param_type, nullptr);
                params.push_back(parameter);
            }

            // Extract function body and create the function statement
            Tree_Sitter_Node body_node;
            body_node.node = children[3];
            body_node.type = ts_node_type(children[3]);
            body_node.children = get_named_nodes(children[3]);

            astfri::ParamVarRefExpr* param1 = exprFactory.mk_param_var_ref("a");
            astfri::ParamVarRefExpr* param2 = exprFactory.mk_param_var_ref("b");

            auto body = stmtFactory.mk_compound(std::vector<astfri::Stmt*>{
              stmtFactory.mk_expr(exprFactory.mk_bin_on(param1, astfri::BinOpType::Add, param2))
            });

            auto function = stmtFactory.mk_function_def(function_name, params, function_return_type, body);
        }
    }
}


  // Free all of the heap-allocated memory.
  free(string);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}