#include "NodeGetter.hpp"

NodeGetter::NodeGetter(TSTree* tree, const std::string& sourceCode) {
    classes = get_classes(tree, sourceCode);
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

std::string NodeGetter::get_node_text(const TSNode& node, const std::string& sourceCode) {
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);
    return sourceCode.substr(start, end - start);
}

std::vector<Parameter_node> NodeGetter::get_params (TSNode paramsNode, const std::string& sourceCode)
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
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

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

std::vector<Local_variable_node> NodeGetter::get_local_vars (TSNode methodNode, const std::string& sourceCode)
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
            std::string nodeName(ts_node_string(tsCapture.node));
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

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
                localVar.literal = nodeName;
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

std::vector<Method_node> NodeGetter::get_methods (TSNode classNode, const std::string& sourceCode)
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
            std::string nodeName(ts_node_string(tsCapture.node));
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

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
            method.params = get_params(paramsNode, sourceCode);
        }
        if (! ts_node_is_null(methodNode))
        {
            method.vars = get_local_vars(methodNode, sourceCode);
        }

        methods.push_back(method);
    }

    return methods;
}

std::vector<Member_variable_node> NodeGetter::get_member_vars (TSNode classNode, const std::string& sourceCode)
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
            std::string nodeName(ts_node_string(tsCapture.node));
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

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
                var.literal = nodeName;
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

std::vector<Class_node> NodeGetter::get_classes (TSTree* tree, const std::string& sourceCode)
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
            std::string nodeText(get_node_text(tsCapture.node, sourceCode));

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
            classNode.vars = get_member_vars(tsNode, sourceCode);
            classNode.methods = get_methods(tsNode, sourceCode);
        }

        classes.push_back(classNode);
    }

    return classes;
}

std::vector<Class_node> NodeGetter::get_classes() {
    return this->classes;
}