#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/CSharpTSTreeVisitor.hpp>

#include <algorithm>
#include <cstring>

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
        // "property_declaration", // todo
        // "delegate_declaration",    // todo
        // "event_field_declaration", // todo
        "constructor_declaration",
        "method_declaration",
        // "indexer_declaration",            // todo
        // "operator_declaration",           // todo
        // "conversion_operator_declaration" // todo
        "destructor_declaration",
    };
    static const TSSymbol base_list_symb = ts_language_symbol_for_name(
        self->language_,
        "base_list",
        std::strlen("base_list"),
        true
    );
    static const TSSymbol type_param_list_sym = ts_language_symbol_for_name(
        self->language_,
        "type_parameter_list",
        std::strlen("type_parameter_list"),
        true
    );
    static const TSSymbol type_param_constr_sym = ts_language_symbol_for_name(
        self->language_,
        "type_parameter_constraints_clause",
        std::strlen("type_parameter_constraints_clause"),
        true
    );

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

    ClassDefStmt* class_def = stmt_factory_.mk_class_def(class_name, scope);
    class_def->name_        = class_name; // todo remove this

    self->semantic_context_.enter_type(class_def);

    TSNode current_node = ts_node_next_sibling(class_name_node);
    const TSNode class_body_node
        = ts_node_child_by_field_name(*node, "body", 4);
    // handling of base class and interface implementations
    ClassDefStmt* base = nullptr;
    std::vector<InterfaceDefStmt*> interfaces;
    do
    {
        std::string name = ts_node_type(current_node);
        if (ts_node_eq(current_node, class_body_node))
            break;

        TSSymbol current_symb = ts_node_symbol(current_node);
        if (current_symb == base_list_symb) // base list handeling
        {
            TSNode type_node = ts_node_child(current_node, 1);
            std::string type_name
                = extract_node_text(type_node, self->source_code_);
            // todo temporary solution
            if (is_interface_name(type_name))
            {
                interfaces.emplace_back(
                    // todo temporary solution - needs proper scope resolving
                    stmt_factory_.mk_interface_def(type_name, {})
                );
            }
            else
            {
                // todo temporary solution - needs proper scope resolving
                base = stmt_factory_.mk_class_def(type_name, {});
            }

            type_node = ts_node_next_sibling(current_node);
            while (! ts_node_is_null(type_node))
            {
                type_name = extract_node_text(type_node, self->source_code_);
                // todo temporary solution
                interfaces.emplace_back(
                    stmt_factory_.mk_interface_def(type_name, {})
                );
                type_node = ts_node_next_sibling(type_node);
            }
        }
        else if (current_symb == type_param_list_sym) // generic parameters
        {
            // todo handle generic parameters;
        }
        else if (current_symb == type_param_constr_sym) // constraints for
                                                        // generic parameters
        {
            // todo handle generic parameter constraints
        }
        current_node = ts_node_next_sibling(current_node);
    } while (! ts_node_is_null(current_node));

    if (base)
    {
        class_def->bases_.push_back(base);
    }
    class_def->interfaces_   = interfaces;

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

    self->semantic_context_.leave_type();
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
            self->semantic_context_.add_member_var(
                as_a<MemberVarDefStmt>(var_def_stmt)
            );
            break;
        }
        case VarDefType::Local:
            // todo handle const
            var_def_stmt
                = stmt_factory_.mk_local_var_def(var_name, type, initializer);
            self->semantic_context_.add_local_var(
                as_a<LocalVarDefStmt>(var_def_stmt)
            );
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
        return stmt_factory_.mk_def(var_def_stmts);

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
    ParamVarDefStmt* parameter
        = stmt_factory_.mk_param_var_def(var_name, var_type, initializer);
    self->semantic_context_.add_param_var(parameter);
    return parameter;
}

Stmt* CSharpTSTreeVisitor::handle_constr_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    self->semantic_context_.enter_scope();
    self->semantic_context_.register_return_type(type_factory_.mk_void());
    ConstructorDefStmt* constructor_def = stmt_factory_.mk_constructor_def();

    const auto result = self->semantic_context_.current_user_type();
    if (! result)
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(result))
        throw std::logic_error(
            "Constructor can only be defined for class type"
        );

    const TSNode param_list_node
        = ts_node_child_by_field_name(*node, "parameters", 10);
    const TSNode body_node = ts_node_child_by_field_name(*node, "body", 4);
    const TSNode initializer_node = ts_node_next_sibling(param_list_node);

    constructor_def->owner_       = as_a<ClassDefStmt>(result);
    constructor_def->params_      = handle_param_list(self, &param_list_node);

    const std::vector<TSNode> modifier_nodes
        = find_nodes(*node, self->language_, "(modifier) @mod");
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, self->source_code_);
    constructor_def->access_
        = modifiers.get_access_mod().value_or(AccessModifier::Private);

    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    constructor_def->body_ = as_a<CompoundStmt>(body_handler(self, &body_node));

    if (! ts_node_is_null(initializer_node))
    {
        const StmtHandler base_init_handler
            = NodeRegistry::get_stmt_handler(initializer_node);
        Stmt* init_stmt = base_init_handler(self, &initializer_node);
        if (const auto base_init = as_a<BaseInitializerStmt>(init_stmt))
            constructor_def->baseInit_.push_back(base_init);
        else if (const auto self_init = as_a<SelfInitializerStmt>(init_stmt))
            constructor_def->selfInitializers.push_back(self_init);
    }

    self->semantic_context_.leave_scope();
    self->semantic_context_.unregister_return_type();
    return constructor_def;
}

Stmt* CSharpTSTreeVisitor::handle_construct_init(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    constexpr std::string_view this_init_sv = "this";
    std::string source_code = extract_node_text(*node, self->source_code_);
    const auto bracket_it   = std::ranges::find(source_code, '(');
    const auto this_it      = std::search(
        source_code.begin(),
        bracket_it,
        this_init_sv.begin(),
        this_init_sv.end()
    );

    const auto result = self->semantic_context_.current_user_type();
    if (! result)
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(result))
        throw std::logic_error("Destructor can only be defined for class type");

    const TSNode arg_list_node = ts_node_child(*node, 2);
    const std::vector<Expr*> arguments
        = handle_argument_list(self, &arg_list_node);
    if (this_it != bracket_it)
        return stmt_factory_.mk_self_initializer(arguments);

    const auto* owner = as_a<ClassDefStmt>(result);
    if (owner->bases_.empty())
        throw std::logic_error(
            "Constructor can't have base initializer without having defined "
            "base class"
        );
    const ClassDefStmt* base = owner->bases_.back();
    return stmt_factory_.mk_base_initializer(base->type_, arguments);
}

Stmt* CSharpTSTreeVisitor::handle_destr_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    // self->semantic_context_.enter_scope();
    self->semantic_context_.register_return_type(type_factory_.mk_void());
    const TSNode body_node = ts_node_child_by_field_name(*node, "body", 4);
    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);
    const auto owner = self->semantic_context_.current_user_type();

    if (! owner)
        throw std::logic_error("Owner type not found");
    if (! is_a<ClassDefStmt>(owner))
        throw std::logic_error("Destructor can only be defined for class type");

    // self->semantic_context_.leave_scope();
    self->semantic_context_.unregister_return_type();
    return stmt_factory_.mk_destructor_def(
        as_a<ClassDefStmt>(owner),
        as_a<CompoundStmt>(body)
    );
}

Stmt* CSharpTSTreeVisitor::handle_method_def_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    static const std::string mod_query =
        R"(
        (method_declaration
            (modifier) @modifier)
        )";

    self->semantic_context_.enter_scope();
    MethodDefStmt* method_def_stmt = stmt_factory_.mk_method_def();
    const auto result = self->semantic_context_.current_user_type();
    if (! result)
        throw std::logic_error("Owner type not found");

    method_def_stmt->owner_ = result;

    const std::vector<TSNode> modifier_nodes
        = find_nodes(*node, self->language_, mod_query);
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, self->source_code_);
    method_def_stmt->access_
        = modifiers.get_access_mod().value_or(AccessModifier::Internal);
    method_def_stmt->func_ = handle_function_stmt(self, node, true);
    self->semantic_context_.unregister_return_type();
    return method_def_stmt;
}

Stmt* CSharpTSTreeVisitor::handle_local_func_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    return handle_function_stmt(self, node, false);
}

FunctionDefStmt* CSharpTSTreeVisitor::handle_function_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node,
    const bool is_method
)
{
    FunctionDefStmt* func_def = stmt_factory_.mk_function_def();
    std::string node_name = is_method ? "returns" : "type";
    const TSNode ret_type_node
        = ts_node_child_by_field_name(*node, node_name.c_str(), node_name.length());
    const TSNode name_node = ts_node_child_by_field_name(*node, "name", 4);
    const TSNode param_node
        = ts_node_child_by_field_name(*node, "parameters", 10);
    const TSNode body_node = ts_node_child_by_field_name(*node, "body", 4);

    // todo handle generic parameters
    Type* ret_type = make_type(self, ret_type_node);
    self->semantic_context_.register_return_type(ret_type);
    func_def->retType_ = ret_type;
    func_def->name_    = extract_node_text(name_node, self->source_code_);
    func_def->params_  = handle_param_list(self, &param_node);
    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    if (const auto body = as_a<CompoundStmt>(body_handler(self, &body_node)))
        func_def->body_ = body;

    self->semantic_context_.leave_scope();
    self->semantic_context_.unregister_return_type();
    return func_def;
}

} // namespace astfri::csharp