#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/CSharpTSTreeVisitor.hpp>

#include <algorithm>

namespace astfri::csharp
{
Stmt* CSharpTSTreeVisitor::handle_class_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    static const std::vector<std::string> class_memb_node_types = {
        // "class_declaration",              // todo
        // "enum_declaration",               // todo
        // "interface_declaration",          // todo
        // "struct_declaration",             // todo
        // "record_declaration",             // todo
        "field_declaration",
        // "delegate_declaration",    // todo
        // "event_field_declaration", // todo
        // "constructor_declaration",
        // "property_declaration", // todo
        "method_declaration",
        "destructor_declaration",
        // "indexer_declaration",            // todo
        // "operator_declaration",           // todo
        // "conversion_operator_declaration" // todo
    };

    std::unordered_map<std::string, std::vector<TSNode>> class_members_nodes;
    for (const std::string& node_type : class_memb_node_types)
    {
        class_members_nodes[node_type];
    }

    const Scope scope = self->create_scope(node);
    const TSNode class_name_node
        = ts_node_child_by_field_name(*node, "name", 4);
    const std::string class_name
        = extract_node_text(class_name_node, self->source_code_);

    ClassDefStmt* class_def
        = StmtFactory::get_instance().mk_class_def(class_name, scope);
    class_def->name_ = class_name;
    self->type_context_.enter_type(class_def);

    TSTreeCursor cursor = ts_tree_cursor_new(class_name_node);
    ts_tree_cursor_goto_next_sibling(&cursor);

    // handling of base class and interface implementations
    ClassDefStmt* base = nullptr;
    std::vector<InterfaceDefStmt*> interfaces;
    do
    {
        const char* field_name = ts_tree_cursor_current_field_name(&cursor);
        if (field_name
            != nullptr) // body node is named, needs to stop at the body node
            break;

        TSNode current   = ts_tree_cursor_current_node(&cursor);
        std::string name = ts_node_type(current);
        if (name == "base_list") // base list handeling
        {
            ts_tree_cursor_goto_first_child(&cursor);
            TSNode type_node = ts_tree_cursor_current_node(&cursor);
            std::string type_name
                = extract_node_text(type_node, self->source_code_);
            // todo temporary solution
            if (is_interface_name(type_name))
            {
                interfaces.emplace_back(
                    // todo temporary solution
                    stmt_factory_.mk_interface_def(type_name, {})
                );
            }
            else
            {
                base = stmt_factory_.mk_class_def(type_name, {});
            }

            while (ts_tree_cursor_goto_next_sibling(&cursor))
            {
                type_node = ts_tree_cursor_current_node(&cursor);
                type_name = extract_node_text(type_node, self->source_code_);
                // todo temporary solution
                interfaces.emplace_back(
                    stmt_factory_.mk_interface_def(type_name, {})
                );
            }
            ts_tree_cursor_goto_parent(&cursor);
        }
        else if (name == "type_parameter_list") // generic parameters
        {
            // todo handle generic parameters;
        }
        else if (name
                 == "type_parameter_constraints_clause") // constraints for
                                                         // generic parameters
        {
            // todo handle generic parameter constraints
        }
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    ts_tree_cursor_delete(&cursor);

    if (base)
    {
        class_def->bases_.push_back(base);
    }
    class_def->interfaces_ = interfaces;
    const TSNode class_body_node
        = ts_node_child_by_field_name(*node, "body", 4);

    TSTreeCursor body_cursor = ts_tree_cursor_new(class_body_node);
    ts_tree_cursor_goto_first_child(&body_cursor);

    // getting of all class member statements
    if (! ts_tree_cursor_goto_first_child(&body_cursor))
    {
        do
        {
            TSNode current   = ts_tree_cursor_current_node(&body_cursor);
            std::string type = ts_node_type(current);
            if (! class_members_nodes.contains(type))
                continue;
            class_members_nodes[type].push_back(current);
        } while (ts_tree_cursor_goto_next_sibling(&body_cursor));
    }

    ts_tree_cursor_delete(&body_cursor);

    // handling of all member statements
    for (const std::string& name : class_memb_node_types)
    {
        const std::vector<TSNode>& members_nodes = class_members_nodes[name];
        for (const TSNode& member_node : members_nodes)
        {
            if (ts_node_is_null(member_node))
                throw std::runtime_error("Node is null");

            StmtHandler handler = NodeRegistry::get_stmt_handler(member_node);
            Stmt* member_stmt   = handler(self, &member_node);

            if (is_a<MemberVarDefStmt>(member_stmt))
                class_def->vars_.push_back(as_a<MemberVarDefStmt>(member_stmt));
            else if (is_a<ConstructorDefStmt>(member_stmt))
                class_def->constructors_.push_back(
                    as_a<ConstructorDefStmt>(member_stmt)
                );
            else if (is_a<DestructorDefStmt>(member_stmt))
                class_def->destructors_.push_back(
                    as_a<DestructorDefStmt>(member_stmt)
                );
            else if (is_a<MethodDefStmt>(member_stmt))
                class_def->methods_.push_back(as_a<MethodDefStmt>(member_stmt));
        }
    }

    self->type_context_.leave_type();
    return class_def;
}

Stmt* CSharpTSTreeVisitor::handle_interface_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    static const std::vector<std::string> class_memb_node_types = {
        // "class_declaration",              // todo
        // "enum_declaration",               // todo
        // "interface_declaration",          // todo
        // "struct_declaration",             // todo
        // "record_declaration",             // todo
        "field_declaration",       // todo
        "delegate_declaration",    // todo
        "event_field_declaration", // todo
        "property_declaration",    // todo
        "method_declaration",      // todo
        "indexer_declaration",     // todo
    };

    const TSNode name_node = ts_node_child_by_field_name(*node, "name", 4);
    const std::string name = extract_node_text(name_node, self->source_code_);
    const Scope scope      = self->create_scope(node);

    InterfaceDefStmt* interface_def_stmt
        = stmt_factory_.mk_interface_def(name, scope);

    // todo not finished
    // needs type resolver to work properly

    return interface_def_stmt;
}

Stmt* CSharpTSTreeVisitor::handle_memb_var_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    return handle_var_def_stmt(self, node, VarDefType::Member);
}

Stmt* CSharpTSTreeVisitor::handle_local_var_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    return handle_var_def_stmt(self, node, VarDefType::Local);
}

Stmt* CSharpTSTreeVisitor::handle_global_var_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode local_var_def_node = ts_node_child(*node, 0);
    return handle_var_def_stmt(self, &local_var_def_node, VarDefType::Global);
}

Stmt* CSharpTSTreeVisitor::handle_var_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node,
    const VarDefType def_type
)
{
    static const std::string decl_query =
        R"(
        (variable_declaration
            (variable_declarator) @var_decl)
        )";
    std::string mod_query = def_type == VarDefType::Member
                              ? "(field_declaration"
                              : "(local_declaration_statement";
    mod_query.append("\n(modifier) @modifier)");

    const std::vector<TSNode> modifier_nodes
        = find_nodes(*node, self->language_, mod_query);
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, self->source_code_);

    const TSNode var_decl_node
        = modifier_nodes.empty() ? ts_node_child(*node, 0)
                                 : ts_node_next_sibling(modifier_nodes.back());

    const TSNode type_node
        = ts_node_child_by_field_name(var_decl_node, "type", 4);
    Type* type = make_type(self, type_node);

    const std::vector<TSNode> var_decltor_nodes
        = find_nodes(var_decl_node, self->language_, decl_query);

    std::vector<VarDefStmt*> var_def_stmts;
    for (const TSNode& var_decltor_node : var_decltor_nodes)
    {
        // todo refactor this into extra method
        TSNode var_name_node = ts_node_child(var_decltor_node, 0); // left side
        TSNode initializer_node
            = ts_node_child(var_decltor_node, 2); // right side

        const std::string var_name
            = extract_node_text(var_name_node, self->source_code_);
        Expr* initializer = nullptr;
        if (! ts_node_is_null(initializer_node))
        {
            ExprHandler initializer_handler
                = NodeRegistry::get_expr_handler(initializer_node);
            initializer = initializer_handler(self, &initializer_node);
        }

        VarDefStmt* var_def_stmt = nullptr;
        switch (def_type)
        {
        case VarDefType::Member:
        {
            // todo handle other modifiers
            const AccessModifier access_modifier
                = modifiers.get_access_mod().value_or(AccessModifier::Private);
            var_def_stmt = stmt_factory_.mk_member_var_def(
                var_name,
                type,
                initializer,
                access_modifier
            );
            break;
        }
        case VarDefType::Local:
            // todo handle const
            var_def_stmt
                = stmt_factory_.mk_local_var_def(var_name, type, initializer);
            break;
        case VarDefType::Global:
            // todo handle const
            var_def_stmt
                = stmt_factory_.mk_global_var_def(var_name, type, initializer);
            break;
        }
        // todo this might not be necessary in future
        if (var_def_stmt)
        {
            var_def_stmt->name_ = var_name;
            var_def_stmts.push_back(var_def_stmt);
        }
    }

    if (var_def_stmts.size() > 1)
        return StmtFactory::get_instance().mk_def(var_def_stmts);

    return var_def_stmts.front();
}

Stmt* CSharpTSTreeVisitor::handle_param_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode var_type_node = ts_node_child_by_field_name(*node, "type", 4);
    const TSNode var_name_node
        = ts_node_child_by_field_name(*node, "name", 4); // left side
    const TSNode initializer_node
        = ts_node_next_sibling(var_name_node); // right side
    const std::string var_name
        = extract_node_text(var_name_node, self->source_code_);
    Expr* initializer = nullptr;
    Type* var_type    = make_type(self, var_type_node);

    if (! ts_node_is_null(initializer_node))
    {
        const ExprHandler initializer_handler
            = NodeRegistry::get_expr_handler(initializer_node);
        initializer = initializer_handler(self, &initializer_node);
    }
    return stmt_factory_.mk_param_var_def(var_name, var_type, initializer);
}

Stmt* CSharpTSTreeVisitor::handle_constr_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const auto result = self->type_context_.top();
    if (! result.has_value())
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(*result))
        throw std::logic_error(
            "Constructor can only be defined for class type"
        );

    const TSNode param_list_node
        = ts_node_child_by_field_name(*node, "parameters", 10);
    const TSNode body_node = ts_node_child_by_field_name(*node, "body", 4);
    const TSNode initializer_node = ts_node_next_sibling(body_node);

    const std::vector<ParamVarDefStmt*> parameters
        = handle_param_list(self, &param_list_node);
    const std::vector<TSNode> modifier_nodes
        = find_nodes(*node, self->language_, "(modifier) @mod");
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, self->source_code_);
    const AccessModifier access_modifier
        = modifiers.get_access_mod().value_or(AccessModifier::Private);
    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);

    std::vector<BaseInitializerStmt*> base_init_stmts;

    if (! ts_node_is_null(initializer_node))
    {
        const StmtHandler base_init_handler
            = NodeRegistry::get_stmt_handler(initializer_node);
        Stmt* base_init_stmt = base_init_handler(self, &initializer_node);
        if (is_a<BaseInitializerStmt>(base_init_stmt))
        {
            base_init_stmts.emplace_back(
                as_a<BaseInitializerStmt>(base_init_stmt)
            );
        }
        // todo handle `this` initializer
    }

    return stmt_factory_.mk_constructor_def(
        as_a<ClassDefStmt>(*result),
        parameters,
        base_init_stmts,
        as_a<CompoundStmt>(body),
        access_modifier
    );
}

Stmt* CSharpTSTreeVisitor::handle_base_init_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    constexpr std::string_view this_init_sw = "this";
    std::string source_code = extract_node_text(*node, self->source_code_);
    const auto bracket_it   = std::ranges::find(source_code, '(');
    const auto this_it      = std::search(
        source_code.begin(),
        bracket_it,
        this_init_sw.begin(),
        this_init_sw.end()
    );

    if (this_it != bracket_it)
        return stmt_factory_.mk_uknown(); // todo handle this initializer

    const auto result = self->type_context_.top();
    if (result.has_value())
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(result.value()))
        throw std::logic_error("Destructor can only be defined for class type");

    const auto* owner = as_a<ClassDefStmt>(*result);
    if (owner->bases_.empty())
        throw std::logic_error(
            "Constructor can't have base initializer without having defined "
            "base class"
        );

    const ClassDefStmt* base   = owner->bases_.back();
    const TSNode arg_list_node = ts_node_child(*node, 0);
    // todo might be wrong index, needs testing
    const std::vector<Expr*> arguments
        = handle_argument_list(self, &arg_list_node);
    return stmt_factory_.mk_base_initializer(base->type_, arguments);
}

Stmt* CSharpTSTreeVisitor::handle_destr_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode body_node = ts_node_child_by_field_name(*node, "body", 4);
    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);
    const auto owner               = self->type_context_.top();

    if (! owner.has_value())
    {
        throw std::logic_error("Owner type not found");
    }
    if (! is_a<ClassDefStmt>(owner.value()))
    {
        throw std::logic_error("Destructor can only be defined for class type");
    }

    return StmtFactory::get_instance().mk_destructor_def(
        as_a<ClassDefStmt>(owner.value()),
        as_a<CompoundStmt>(body)
    );
}

Stmt* CSharpTSTreeVisitor::handle_method_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    MethodDefStmt* method_def_stmt = stmt_factory_.mk_method_def();
    const auto result              = self->type_context_.top();
    if (! result.has_value())
        throw std::logic_error("Owner type not found");

    static const std::string mod_query =
        R"(
        (method_declaration
            (modifier) @modifier)
        )";

    method_def_stmt->owner_ = *result;
    const std::vector<TSNode> modifier_nodes
        = find_nodes(*node, self->language_, mod_query);
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, self->source_code_);
    method_def_stmt->access_
        = modifiers.get_access_mod().value_or(AccessModifier::Internal);

    const TSNode name_node = ts_node_child_by_field_name(*node, "name", 4);
    const TSNode parameters_node
        = ts_node_child_by_field_name(*node, "parameters", 10);
    const TSNode return_node = ts_node_child_by_field_name(*node, "returns", 7);
    const TSNode body_node   = ts_node_child_by_field_name(*node, "body", 4);

    // todo handle generic parameters

    const std::string method_name
        = extract_node_text(name_node, self->source_code_);
    const std::vector<ParamVarDefStmt*> parameters
        = handle_param_list(self, &parameters_node);
    Type* return_type              = make_type(self, return_node);
    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);

    FunctionDefStmt* function_def_stmt = stmt_factory_.mk_function_def(
        method_name,
        parameters,
        return_type,
        as_a<CompoundStmt>(body)
    );

    method_def_stmt->func_ = function_def_stmt;
    return method_def_stmt;
}

} // namespace astfri::csharp