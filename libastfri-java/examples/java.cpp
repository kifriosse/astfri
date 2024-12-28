#include <cstring>
#include <cstdio>
#include <sstream>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <fstream>
#include <iostream>
#include <vector>

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
  TSNode class_node = ts_node_named_child(root_node, 0);
  std::cout << "Class node: " << ts_node_type(class_node) << std::endl;
  TSNode class_body_node = ts_node_named_child(class_node, 1);
  TSNode class_body_sibling = ts_node_next_sibling(class_body_node);
  std::cout << "Next sibling node: " << ts_node_type(class_body_sibling) << std::endl;
  std::cout << "Children count " << ts_node_named_child_count(class_node) << std::endl;

  // Print the syntax tree as an S-expression.
  char *string = ts_node_string(TSNode(ts_tree_root_node(tree)));
  printf("Syntax tree: %s\n", string);

  // Get all nodes in the tree
  std::vector<TSNode> nodes = get_nodes(root_node);
  print_nodes(nodes, root_node);

  //Wallking through tree
  TSTreeCursor cursor = ts_tree_cursor_new(root_node);
  bool result = ts_tree_cursor_goto_first_child(&cursor);
  std::cout << result << std::endl;
  std::cout << ts_node_type(ts_tree_cursor_current_node(&cursor)) << std::endl;
  

  // Free all of the heap-allocated memory.
  free(string);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}