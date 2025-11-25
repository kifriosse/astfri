#include <libastfri-cs/impl/CSharpTSTreeVisitor.hpp>
#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>

#include <algorithm>

namespace astfri::csharp
{
Stmt* CSharpTSTreeVisitor::handle_class_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{
    static std::vector<std::string> const class_memb_node_types = {
        // "class_declaration",              // todo
        // "enum_declaration",               // todo
        // "interface_declaration",          // todo
        // "struct_declaration",             // todo
        // "record_declaration",             // todo
        "field_declaration",
        "delegate_declaration",    // todo
        "event_field_declaration", // todo
        "constructor_declaration",
        "property_declaration", // todo
        "method_declaration",   // todo
        "destructor_declaration",
        "indexer_declaration",            // todo
        "operator_declaration",           // todo
        "conversion_operator_declaration" // todo,
    };

    std::unordered_map<std::string, std::vector<TSNode>> class_members_nodes;
    for (std::string const& node_type : class_memb_node_types)
    {
        class_members_nodes[node_type];
    }

    Scope const scope            = self->create_scope(node);
    TSNode const class_name_node = ts_node_child_by_field_name(*node, "name", 4);
    std::string const class_name = extract_node_text(class_name_node, self->source_code_);

    ClassDefStmt* class_def      = StmtFactory::get_instance().mk_class_def(class_name, scope);
    class_def->name_             = class_name;
    self->type_context_.enter_type(class_def);

    TSTreeCursor cursor = ts_tree_cursor_new(class_name_node);
    ts_tree_cursor_goto_next_sibling(&cursor);

    // handling of base class and interface implementations
    ClassDefStmt* base = nullptr;
    std::vector<InterfaceDefStmt*> interfaces;
    do
    {
        char const* field_name = ts_tree_cursor_current_field_name(&cursor);
        if (field_name != nullptr) // body node is named, needs to stop at the body node
            break;

        TSNode current   = ts_tree_cursor_current_node(&cursor);
        std::string name = ts_node_type(current);
        if (name == "type_parameter_list")
        {
            // todo handle generic parameters;
        }
        else if (name == "base_list")
        {
            ts_tree_cursor_goto_first_child(&cursor);
            TSNode type_node      = ts_tree_cursor_current_node(&cursor);
            std::string type_name = extract_node_text(type_node, self->source_code_);
            // todo temporary solution
            if (is_interface_name(type_name))
            {
                interfaces.emplace_back(stmt_factory_.mk_interface_def(type_name));
            }
            else
            {
                base = stmt_factory_.mk_class_def(type_name, {});
            }

            while (ts_tree_cursor_goto_next_sibling(&cursor))
            {
                type_node = ts_tree_cursor_current_node(&cursor);
                type_name = extract_node_text(type_node, self->source_code_);
                interfaces.emplace_back(stmt_factory_.mk_interface_def(type_name));
            }
            ts_tree_cursor_goto_parent(&cursor);
        }
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    if (base)
    {
        class_def->bases_.push_back(base);
    }
    class_def->interfaces_       = interfaces;
    TSNode const class_body_node = ts_tree_cursor_current_node(&cursor);

    ts_tree_cursor_delete(&cursor);

    TSTreeCursor body_cursor = ts_tree_cursor_new(class_body_node);
    ts_tree_cursor_goto_first_child(&body_cursor);

    if (! ts_tree_cursor_goto_first_child(&body_cursor))
    {
        do
        {
            TSNode current = ts_tree_cursor_current_node(&body_cursor);
            class_members_nodes[ts_node_type(current)].push_back(current);
        } while (ts_tree_cursor_goto_next_sibling(&body_cursor));
    }

    ts_tree_cursor_delete(&body_cursor);

    for (std::string const& name : class_memb_node_types)
    {
        std::vector<TSNode> const& members_nodes = class_members_nodes[name];
        for (TSNode const& member_node : members_nodes)
        {
            if (ts_node_is_null(member_node))
            {
                throw std::runtime_error("Node is null");
            }
            StmtHandler handler = NodeRegistry::get_stmt_handler(member_node);
            Stmt* member_stmt   = handler(self, &member_node);

            if (is_a<MemberVarDefStmt>(member_stmt))
                class_def->vars_.push_back(as_a<MemberVarDefStmt>(member_stmt));
            else if (is_a<ConstructorDefStmt>(member_stmt))
                class_def->constructors_.push_back(as_a<ConstructorDefStmt>(member_stmt));
            else if (is_a<DestructorDefStmt>(member_stmt))
                class_def->destructors_.push_back(as_a<DestructorDefStmt>(member_stmt));
            else if (is_a<MethodDefStmt>(member_stmt))
                class_def->methods_.push_back(as_a<MethodDefStmt>(member_stmt));
        }
    }

    self->type_context_.leave_type();
    return class_def;
}

Stmt* CSharpTSTreeVisitor::handle_memb_var_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{
    static std::string const decl_query =
        R"(
        (variable_declaration
            (variable_declarator) @var_decl)
        )";
    static std::string const mod_query =
        R"(
        (field_declaration
            (modifier) @modifier)
        )";

    std::vector<TSNode> const modifier_nodes = find_nodes(*node, self->language_, mod_query);

    auto access_modifier                     = AccessModifier::Private;
    bool is_private                          = false;
    bool is_protected                        = false;
    bool is_internal                         = false;
    bool is_static                           = false;
    bool is_readonly                         = false;
    bool is_const                            = false;
    bool is_volatile                         = false;
    bool is_new                              = false;

    for (TSNode modifier_node : modifier_nodes)
    {
        std::string modifier_str = extract_node_text(modifier_node, self->source_code_);
        auto const res           = NodeRegistry::get_access_modifier(modifier_str);
        if (res.has_value())
        {
            access_modifier = *res;
            switch (access_modifier)
            {
            case AccessModifier::Private:
                is_private = true;
                break;
            case AccessModifier::Protected:
                is_protected = true;
                break;
            case AccessModifier::Internal:
                is_internal = true;
                break;
            default:
                break;
            }
            continue;
        }

        if (modifier_str == "static")
            is_static = true;
        else if (modifier_str == "readonly")
            is_readonly = true;
        else if (modifier_str == "const")
            is_const = true;
        else if (modifier_str == "volatile")
            is_volatile = true;
        else if (modifier_str == "new")
            is_new = true;
    }

    if (is_internal && is_protected)
    {
        // todo handle protected internal
    }
    else if (is_private && is_protected)
    {
        // todo handle private protected
    }
    // todo handle other modifiers

    TSNode const var_decl_node = modifier_nodes.empty()
                                   ? ts_node_child(*node, 0)
                                   : ts_node_next_sibling(modifier_nodes.back());

    TSNode const type_node     = ts_node_child_by_field_name(var_decl_node, "type", 4);

    Type* type                 = make_type(self, type_node);
    std::vector<TSNode> const var_decl_nodes
        = find_nodes(var_decl_node, self->language_, decl_query);

    std::vector<VarDefStmt*> var_def_stmts;
    for (TSNode const& var_decltor_node : var_decl_nodes)
    {
        // todo refactor this into extra method
        TSNode var_name_node       = ts_node_child(var_decltor_node, 0); // left side
        TSNode initializer_node    = ts_node_child(var_decltor_node, 2); // right side

        std::string const var_name = extract_node_text(var_name_node, self->source_code_);
        Expr* initializer          = nullptr;
        if (! ts_node_is_null(initializer_node))
        {
            ExprHandler initializer_handler = NodeRegistry::get_expr_handler(initializer_node);
            initializer                     = initializer_handler(self, &initializer_node);
        }

        // todo handle access modifiers
        MemberVarDefStmt* memb_var_def
            = stmt_factory_.mk_member_var_def(var_name, type, initializer, access_modifier);
        memb_var_def->name_ = var_name;
        var_def_stmts.push_back(memb_var_def);
    }

    if (var_def_stmts.size() > 1)
    {
        return StmtFactory::get_instance().mk_def(var_def_stmts);
    }

    return var_def_stmts.front();
}

Stmt* CSharpTSTreeVisitor::handle_param_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{
    TSNode const var_type_node    = ts_node_child_by_field_name(*node, "type", 4);
    TSNode const var_name_node    = ts_node_child_by_field_name(*node, "name", 4); // left side
    TSNode const initializer_node = ts_node_next_sibling(var_name_node);           // right side
    std::string const var_name    = extract_node_text(var_name_node, self->source_code_);
    Expr* initializer             = nullptr;
    Type* var_type                = make_type(self, var_type_node);

    if (! ts_node_is_null(initializer_node))
    {
        ExprHandler const initializer_handler = NodeRegistry::get_expr_handler(initializer_node);
        initializer                           = initializer_handler(self, &initializer_node);
    }
    return stmt_factory_.mk_param_var_def(var_name, var_type, initializer);
}

Stmt* CSharpTSTreeVisitor::handle_constr_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{
    auto const result = self->type_context_.top();
    if (! result.has_value())
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(*result))
        throw std::logic_error("Constructor can only be defined for class type");

    TSNode const param_list_node = ts_node_child_by_field_name(*node, "parameters", 10);
    std::vector<ParamVarDefStmt*> const parameters = handle_param_list(self, &param_list_node);
    std::vector<TSNode> const modif_nodes = find_nodes(*node, self->language_, "(modifier) @mod");

    auto modifier                         = AccessModifier::Internal;
    bool is_private                       = false;
    bool is_protected                     = false;
    bool is_internal                      = false;
    for (auto modif_node : modif_nodes)
    {
        std::string modif_str = extract_node_text(modif_node, self->source_code_);
        auto result_modif     = NodeRegistry::get_access_modifier(modif_str);
        if (! result.has_value())
            continue;

        modifier = *result_modif;
        switch (modifier)
        {
        case AccessModifier::Private:
            is_private = true;
            break;
        case AccessModifier::Protected:
            is_protected = true;
            break;
        case AccessModifier::Internal:
            is_internal = true;
            break;
        default:
            break;
        }
    }

    if (is_protected && is_private)
    {
        // todo
    }
    else if (is_protected && is_internal)
    {
        // todo
    }

    TSNode const body_node         = ts_node_child_by_field_name(*node, "body", 4);
    StmtHandler const body_handler = NodeRegistry::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);
    TSNode const initializer_node  = ts_node_next_sibling(body_node);
    std::vector<BaseInitializerStmt*> base_init_stmts;

    if (! ts_node_is_null(initializer_node))
    {
        StmtHandler const base_init_handler = NodeRegistry::get_stmt_handler(initializer_node);
        Stmt* base_init_stmt                = base_init_handler(self, &initializer_node);
        if (is_a<BaseInitializerStmt>(base_init_stmt))
        {
            base_init_stmts.emplace_back(as_a<BaseInitializerStmt>(base_init_stmt));
            // todo handle `this` initializer
        }
    }

    return stmt_factory_.mk_constructor_def(
        as_a<ClassDefStmt>(*result),
        parameters,
        base_init_stmts,
        as_a<CompoundStmt>(body),
        modifier
    );
}

Stmt* CSharpTSTreeVisitor::handle_base_init_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{
    std::string source_code                 = extract_node_text(*node, self->source_code_);
    auto const bracket_it                   = std::ranges::find(source_code, '(');
    std::string_view constexpr this_init_sw = "this";
    auto const this_it
        = std::search(source_code.begin(), bracket_it, this_init_sw.begin(), this_init_sw.end());

    if (this_it != bracket_it)
        return stmt_factory_.mk_uknown(); // todo handle this initializer

    auto const result = self->type_context_.top();
    if (result.has_value())
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(result.value()))
        throw std::logic_error("Destructor can only be defined for class type");

    auto const* owner = as_a<ClassDefStmt>(*result);
    if (owner->bases_.empty())
        throw std::logic_error(
            "Constructor can't have base initializer without having defined base class"
        );

    ClassDefStmt const* base   = owner->bases_.back();
    TSNode const arg_list_node = ts_node_child(*node, 0);
    // todo might be wrong index, needs testing
    std::vector<Expr*> const arguments = handle_argument_list(self, &arg_list_node);
    return stmt_factory_.mak_base_initializer(base->name_, arguments);
}

Stmt* CSharpTSTreeVisitor::handle_destr_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{
    TSNode const body_node         = ts_node_child_by_field_name(*node, "body", 4);
    StmtHandler const body_handler = NodeRegistry::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);
    auto const owner               = self->type_context_.top();

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

} // namespace astfri::csharp