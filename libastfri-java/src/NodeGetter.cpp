#include "NodeGetter.hpp"

#include <cstdint>
#include <cstring>
#include <string>
#include <tree_sitter/api.h>
#include <vector>

TSTree* NodeGetter::get_syntax_tree(char const* sourceCode)
{
    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());
    TSTree* tree
        = ts_parser_parse_string(parser, NULL, sourceCode, strlen(sourceCode));

    return tree;
}

TSQuery* NodeGetter::make_query (char const* queryString)
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

std::vector<Parameter_node> NodeGetter::get_params (TSNode paramsNode)
{
    std::vector<Parameter_node> params;

    char const* queryString = "(formal_parameter type: (_) @param_type name: "
                              "(identifier) @param_name)";

    TSQuery* tsQuery = make_query(queryString);
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

std::vector<Local_variable_node> NodeGetter::get_local_vars (TSNode methodNode)
{
    std::vector<Local_variable_node> localVars;

    char const* queryString
        = "(local_variable_declaration type: (_) @local_var_type "
          "(variable_declarator name: (identifier) @local_var_name value: (_) "
          "@local_var_value)?)";

    TSQuery* tsQuery = make_query(queryString);
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

std::vector<Method_node> NodeGetter::get_methods (TSNode classNode)
{
    std::vector<Method_node> methods;

    char const* queryString
        = "(method_declaration (modifiers)? @method_modifiers type: (_) "
          "@method_return_type name: (identifier) @method_name "
          "(formal_parameters)? @params body: (block) @method_body)";

    TSQuery* tsQuery = make_query(queryString);
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

std::vector<Member_variable_node> NodeGetter::get_member_vars (TSNode classNode)
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

    TSQuery* tsQuery = make_query(queryString);
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

std::vector<Class_node> NodeGetter::get_classes (TSTree* tree)
{
    std::vector<Class_node> classes;
    TSNode rootNode = ts_tree_root_node(tree);

    char const* queryString = "(class_declaration"
                              "(modifiers)? @modifier"
                              "(identifier) @class_name"
                              "(class_body)? @class_body"
                              ")";

    TSQuery* tsQuery = make_query(queryString);
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
            classNode.vars = get_member_vars(tsNode);
            classNode.methods = get_methods(tsNode);
        }

        classes.push_back(classNode);
    }

    return classes;
}