#include <cstring>
#include <cstdio>
#include <sstream>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <fstream>
#include <iostream>

// function for loading content of a file into string
char* load(const char* path) {
  std::ifstream file(path);
  if(!file.is_open()) {
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

std::string load_code(const std::string path) {
  std::ifstream file(path);
  if(!file.is_open()) {
    std::string error = "Could not open file: " + path;
    return error;
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

int main() {
  // Create a parser.
  TSParser *parser = ts_parser_new();

  // Set the parser's language (JSON in this case).
  ts_parser_set_language(parser, tree_sitter_java());

  // Build a syntax tree based on source code stored in a string.
  const char *source_code = load(std::string("/tmp/resources/java_dummy.txt").c_str());
  TSTree *tree = ts_parser_parse_string(
    parser,
    NULL,
    source_code,
    strlen(source_code)
  );

  // Get the root node of the syntax tree.
  TSNode root_node = ts_tree_root_node(tree);

  // Print the syntax tree as an S-expression.
  char *string = ts_node_string(root_node);
  printf("Syntax tree: %s\n", string);

  // Free all of the heap-allocated memory.
  free(string);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}
