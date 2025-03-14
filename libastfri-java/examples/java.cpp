#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri-text/inc/ASTVisitor.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <vector>

#include "../src/NodeGetter.hpp"
#include "libastfri/inc/Stmt.hpp"

typedef struct Tree_Sitter_Node
{
    TSNode node;
    std::string type;
    std::vector<TSNode> children;
} Tree_Sitter_Node;

std::string load (std::string const& path)
{
    std::ifstream file(path);
    if (! file.is_open())
    {
        return "ERROR: Could not open file: " + path;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string get_directory (std::string const& file_path)
{
    size_t last_slash_idx = file_path.find_last_of("/\\");
    if (last_slash_idx != std::string::npos)
    {
        return file_path.substr(0, last_slash_idx + 1);
    }
    return "./";
}

std::vector<TSNode> get_nodes (TSNode node)
{
    std::vector<TSNode> nodes;
    nodes.push_back(node);

    uint32_t child_count = ts_node_child_count(node);

    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode child                    = ts_node_child(node, i);
        std::vector<TSNode> child_nodes = get_nodes(child);
        nodes.insert(nodes.begin(), child_nodes.begin(), child_nodes.end());
    }

    return nodes;
}

std::vector<TSNode> get_named_nodes (TSNode node)
{
    std::vector<TSNode> nodes;

    if (ts_node_is_named(node))
    {
        nodes.push_back(node);
    }

    uint32_t child_count = ts_node_child_count(node);

    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode child                    = ts_node_child(node, i);
        std::vector<TSNode> child_nodes = get_named_nodes(child);
        nodes.insert(nodes.end(), child_nodes.begin(), child_nodes.end());
    }

    return nodes;
}

void print_nodes (std::vector<TSNode> nodes, TSNode root)
{
    nodes = get_nodes(root);

    for (int i = nodes.size() - 1; i >= 0; --i)
    {
        printf("%s\n", ts_node_type(nodes[i]));
    }
}

void print_named_nodes (std::vector<TSNode> nodes, TSNode root)
{
    nodes = get_nodes(root);

    for (int i = nodes.size() - 1; i >= 0; --i)
    {
        if (ts_node_is_named(nodes[i]))
        {
            printf("%s\n", ts_node_type(nodes[i]));
        }
        else
        {
        }
    }
}

std::string get_node_type (TSNode node)
{
    return ts_node_type(node);
}

TSQuery* make_query (char const* queryString)
{
    uint32_t errorOffset;
    TSQueryError errorType;

    TSQuery* tsQuery = ts_query_new(
        tree_sitter_java(),
        queryString,
        strlen(queryString),
        &errorOffset,
        &errorType
    );

    return tsQuery;
}

std::string get_node_text(const TSNode& node, const std::string& source_code) {
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);
    return source_code.substr(start, end - start);
}

int main ()
{
    // Create a parser.
    TSParser* parser = ts_parser_new();

    // Set the parser's language (JAVA in this case).
    ts_parser_set_language(parser, tree_sitter_java());

    // Get current directory and append the file name
    std::string current_dir = get_directory(__FILE__);
    std::string path        = current_dir + "Dummy.txt";

    // Load file content
    std::string source_code = load(path);

    // Check if the file was successfully loaded
    if (source_code.rfind("ERROR:", 0) == 0)
    {
        std::cerr << source_code << std::endl; // Print error message
        return 1;                              // Exit with error
    }

    std::cout << source_code << std::endl;

    // Parse the source code
    TSTree* tree = ts_parser_parse_string(
        parser,
        nullptr,
        source_code.c_str(),
        source_code.size()
    );

    // Check if parsing was successful
    if (! tree)
    {
        std::cerr << "ERROR: Failed to parse source code." << std::endl;
        ts_parser_delete(parser);
        return 1;
    }

    // Get the root node of the syntax tree.
    TSNode root_node = ts_tree_root_node(tree);
    std::cout << "Root node: " << ts_node_type(root_node) << std::endl;
    TSNode next_node = ts_node_named_child(root_node, 0);
    std::cout << "Next node: " << ts_node_type(next_node) << std::endl;
//    TSNode body_node    = ts_node_named_child(next_node, 1);
//    TSNode body_sibling = ts_node_next_sibling(body_node);
//    std::cout << "Next sibling node: " << ts_node_type(body_sibling)
//              << std::endl;
    std::cout << "Children count " << ts_node_named_child_count(next_node)
              << std::endl;

    // Print the syntax tree as an S-expression.
    char* string = ts_node_string(TSNode(ts_tree_root_node(tree)));
    printf("Syntax tree: %s\n", string);

    // Get all nodes in the tree
    std::vector<TSNode> nodes = get_nodes(root_node);
    print_named_nodes(nodes, root_node);

    std::vector<TSNode> named_nodes = get_named_nodes(root_node);

    // Wallking through tree
    TSTreeCursor cursor = ts_tree_cursor_new(root_node);
    bool result         = ts_tree_cursor_goto_first_child(&cursor);
    std::cout << result << std::endl;
    std::cout << ts_node_type(ts_tree_cursor_current_node(&cursor))
              << std::endl;

    NodeGetter* nodeGetter = new NodeGetter(tree, source_code);
    std::vector<astfri::ClassDefStmt*> classes2 = nodeGetter->get_classes();

    ASTVisitor* visitor = new ASTVisitor();
    for (astfri::ClassDefStmt* classDef : classes2)
    {
        visitor->visit(*classDef);
    }
    visitor->write_file();

    // Free all of the heap-allocated memory.
    free(string);
    ts_tree_delete(tree);
    ts_parser_delete(parser);
    return 0;
}