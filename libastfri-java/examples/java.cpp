#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>
#include <vector>

#include "../src/NodeGetter.hpp"

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

std::vector<Parameter_node> get_params (TSNode paramsNode)
{
    std::vector<Parameter_node> params;

    char const* queryString = "(formal_parameter type: (_) @param_type name: "
                              "(identifier) @param_name)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, paramsNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        Parameter_node param;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeText(ts_node_string(tsCapture.node));

            if (captureName == "param_type")
            {
                param.type = nodeText;
            }
            else if (captureName == "param_name")
            {
                param.name = nodeText;
            }
        }

        if (! param.name.empty())
        {
            params.push_back(param);
        }
    }

    return params;
}

std::vector<Local_variable_node> get_local_vars (TSNode methodNode)
{
    std::vector<Local_variable_node> localVars;

    char const* queryString
        = "(local_variable_declaration type: (_) @local_var_type "
          "(variable_declarator name: (identifier) @local_var_name value: (_) "
          "@local_var_value)?)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, methodNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        Local_variable_node localVar;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeText(ts_node_string(tsCapture.node));

            if (captureName == "local_var_type")
            {
                localVar.type = nodeText;
            }
            else if (captureName == "local_var_name")
            {
                localVar.name = nodeText;
            }
            else if (captureName == "local_var_value")
            {
                localVar.value = nodeText;
            }
        }

        if (! localVar.name.empty())
        {
            localVars.push_back(localVar);
        }
    }

    return localVars;
}

std::vector<Method_node> get_methods (TSNode classNode)
{
    std::vector<Method_node> methods;

    char const* queryString
        = "(method_declaration (modifiers)? @method_modifiers type: (_) "
          "@method_return_type name: (identifier) @method_name "
          "(formal_parameters)? @params body: (block) @method_body)";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, classNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        Method_node method;
        TSNode paramsNode;
        TSNode methodNode;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeText(ts_node_string(tsCapture.node));

            if (captureName == "method_modifiers")
            {
                method.modifiers.push_back(nodeText);
            }
            else if (captureName == "method_return_type")
            {
                method.returnType = nodeText;
            }
            else if (captureName == "method_name")
            {
                method.name = nodeText;
            }
            else if (captureName == "params")
            {
                paramsNode = tsCapture.node;
            }
            else if (captureName == "method_body")
            {
                methodNode = tsCapture.node;
            }
        }

        if (! ts_node_is_null(paramsNode))
        {
            method.params = get_params(paramsNode);
        }
        if (! ts_node_is_null(methodNode))
        {
            method.vars = get_local_vars(methodNode);
        }

        methods.push_back(method);
    }

    return methods;
}

std::vector<Member_variable_node> get_member_vars (TSNode classNode)
{
    std::vector<Member_variable_node> vars;

    char const* queryString = "(field_declaration"
                              "  (modifiers)? @member_modifier"
                              "  type: (_) @member_type"
                              "  (variable_declarator"
                              "    name: (identifier) @member_name"
                              "    value: (_)? @member_value"
                              "  )"
                              ")";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, classNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        Member_variable_node var;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeText(ts_node_string(tsCapture.node));

            if (captureName == "memeber_modifier")
            {
                var.modifiers.push_back(nodeText);
            }
            else if (captureName == "member_type")
            {
                var.type = nodeText;
            }
            else if (captureName == "member_name")
            {
                var.name = nodeText;
            }
            else if (captureName == "member_value")
            {
                var.value = nodeText;
            }
        }

        if (! var.name.empty())
        {
            vars.push_back(var);
        }
    }

    return vars;
}

std::vector<Class_node> get_classes (TSTree* tree)
{
    std::vector<Class_node> classes;
    TSNode rootNode         = ts_tree_root_node(tree);

    char const* queryString = "(class_declaration"
                              "(modifiers)? @modifier"
                              "(identifier) @class_name"
                              "(class_body)? @class_body"
                              ")";

    TSQuery* tsQuery        = make_query(queryString);
    TSQueryCursor* tsCursor = ts_query_cursor_new();
    ts_query_cursor_exec(tsCursor, tsQuery, rootNode);
    TSQueryMatch tsMatch;

    while (ts_query_cursor_next_match(tsCursor, &tsMatch))
    {
        Class_node classNode;
        TSNode tsNode;

        for (uint32_t i = 0; i < tsMatch.capture_count; i++)
        {
            TSQueryCapture tsCapture = tsMatch.captures[i];
            uint32_t length;
            std::string captureName = ts_query_capture_name_for_id(
                tsQuery,
                tsCapture.index,
                &length
            );
            std::string nodeText(ts_node_string(tsCapture.node));

            if (captureName == "class_modifier")
            {
                classNode.modifiers.push_back(nodeText);
            }
            else if (captureName == "class_name")
            {
                classNode.name = nodeText;
            }
            else if (captureName == "class_body")
            {
                tsNode = tsCapture.node;
            }
        }

        if (! ts_node_is_null(tsNode))
        {
            classNode.vars    = get_member_vars(tsNode);
            classNode.methods = get_methods(tsNode);
        }

        classes.push_back(classNode);
    }

    return classes;
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
    TSNode body_node    = ts_node_named_child(next_node, 1);
    TSNode body_sibling = ts_node_next_sibling(body_node);
    std::cout << "Next sibling node: " << ts_node_type(body_sibling)
              << std::endl;
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

    //    const char* queryString = "(formal_parameter type: (_) @param_type
    //    name: "
    //                              "(identifier) @param_name)";
    //
    //    TSQuery* tsQuery = make_query(queryString);
    //    TSQueryCursor* tsCursor = ts_query_cursor_new();
    //    ts_query_cursor_exec(tsCursor, tsQuery, root_node);
    //    TSQueryMatch tsMatch;
    //
    //    while (ts_query_cursor_next_match(tsCursor, &tsMatch)) {
    //        for (uint32_t i = 0; i < tsMatch.capture_count; i++) {
    //            TSQueryCapture capture = tsMatch.captures[i];
    //            std::string className = ts_node_string(capture.node);
    //            std::cout << className << std::endl;
    //        }
    //    }

    //  auto& exprFactory = astfri::ExprFactory::get_instance();
    //  auto& stmtFactory = astfri::StmtFactory::get_instance();
    //  auto& typeFactory = astfri::TypeFactory::get_instance();
    //
    //  std::vector<astfri::FunctionDefStmt*> functions;
    //  std::vector<astfri::ParamVarDefStmt*> params;
    //
    //  for (ulong i = 0; i < named_nodes.size(); i++) {
    //    if (ts_node_child_count(nodes[i]) > 0) {
    //        Tree_Sitter_Node node;
    //        node.node = nodes[i];
    //        node.type = ts_node_type(nodes[i]);
    //
    //        // Get child nodes and filter out "modifiers" if present
    //        std::vector<TSNode> children = get_nodes(nodes[i]);
    //        if (std::string_view(ts_node_type(children[0])) == "modifiers") {
    //        // string_view
    //            children.erase(children.begin());
    //        }
    //
    //        astfri::Type* function_return_type = typeFactory.mk_void();
    //
    //        // Check if the node is a method declaration
    //        if (node.type == "method_declaration") {
    //            if (std::string_view(ts_node_type(children[0])) ==
    //            "integral_type") {
    //                function_return_type = typeFactory.mk_int();
    //            }
    //
    //            // Extract function name
    //            TSNode identifier_node =
    //            ts_node_child_by_field_name(children[1], "identifier",
    //            strlen("identifier")); std::string function_name =
    //            ts_node_string(identifier_node);
    //
    //            // Extract parameters
    //            Tree_Sitter_Node parameters;
    //            parameters.node = children[2];
    //            parameters.type = ts_node_type(children[2]);
    //            parameters.children = get_named_nodes(children[2]);
    //
    //            for (auto& param : parameters.children) {
    //                Tree_Sitter_Node param_node;
    //                param_node.node = param;
    //                param_node.type = ts_node_type(param);
    //                param_node.children = get_named_nodes(param);
    //
    //                astfri::Type* param_type = nullptr;
    //                if (std::string_view(ts_node_type(param_node.children[0]))
    //                == "integral_type") {
    //                    param_type = typeFactory.mk_int();
    //                }
    //
    //                // Extract parameter name
    //                TSNode param_identifier_node =
    //                ts_node_child_by_field_name(param_node.children[1],
    //                "identifier", strlen("identifier")); std::string
    //                param_name = ts_node_string(param_identifier_node); auto
    //                parameter = stmtFactory.mk_param_var_def(param_name,
    //                param_type, nullptr); params.push_back(parameter);
    //            }
    //
    //            // Extract function body and create the function statement
    //            Tree_Sitter_Node body_node;
    //            body_node.node = children[3];
    //            body_node.type = ts_node_type(children[3]);
    //            body_node.children = get_named_nodes(children[3]);
    //
    //            astfri::ParamVarRefExpr* param1 =
    //            exprFactory.mk_param_var_ref("a"); astfri::ParamVarRefExpr*
    //            param2 = exprFactory.mk_param_var_ref("b");
    //
    //            auto body =
    //            stmtFactory.mk_compound(std::vector<astfri::Stmt*>{
    //              stmtFactory.mk_expr(exprFactory.mk_bin_on(param1,
    //              astfri::BinOpType::Add, param2))
    //            });
    //
    //            auto function = stmtFactory.mk_function_def(function_name,
    //            params, function_return_type, body);
    //        }
    //    }
    //}

    std::vector<Class_node> classes = get_classes(tree);

    for (Class_node classNode : classes)
    {
        for (std::string mod : classNode.modifiers)
        {
            std::cout << mod << " ";
        }
        std::cout << classNode.name << "\n";
        for (Member_variable_node memVar : classNode.vars)
        {
            for (std::string mod : memVar.modifiers)
            {
                std::cout << mod << " ";
            }
            std::cout << memVar.type << " " << memVar.name << " "
                      << memVar.value << "\n";
        }
        for (Method_node method : classNode.methods)
        {
            for (std::string mod : method.modifiers)
            {
                std::cout << mod << " ";
            }
            std::cout << method.returnType << " ";
            std::cout << method.name << " ";
            for (Parameter_node param : method.params)
            {
                std::cout << param.type << " ";
                std::cout << param.name << ", ";
            }
            for (Local_variable_node var : method.vars)
            {
                std::cout << "\n"
                          << var.type << " " << var.name << " " << var.value;
            }
        }
    }

    // Free all of the heap-allocated memory.
    // free(string);
    // ts_tree_delete(tree);
    // ts_parser_delete(parser);
    return 0;
}