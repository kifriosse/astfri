#include "CSharpTSTreeVisitor.hpp"

#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>

#include <algorithm>
#include <cstring>

namespace astfri::csharp
{

ExprFactory& CSharpTSTreeVisitor::expr_factory_ = ExprFactory::get_instance();
StmtFactory& CSharpTSTreeVisitor::stmt_factory_ = StmtFactory::get_instance();
TypeFactory& CSharpTSTreeVisitor::type_factory_ = TypeFactory::get_instance();

void CSharpTSTreeVisitor::handle_comp_unit_stmt(TranslationUnit& tr_unit, TSNode const* node)
{
    static std::string const type_decl_query = R"(
        (namespace_declaration
            body: (declaration_list
                (class_declaration) @class))
        (namespace_declaration
            body: (declaration_list
                (interface_declaration) @interface))

        (compilation_unit
            (class_declaration) @class)

        (compilation_unit
            (interface_declaration) @interface)
    )";

    std::vector<TSNode> const type_nodes     = find_nodes(*node, language_, type_decl_query);

    for (auto const& type_node : type_nodes)
    {
        StmtHandler handler = NodeRegistry::get_stmt_handler(type_node);
        Stmt* stmt          = handler(this, &type_node);
        // todo handle partial class
        if (is_a<ClassDefStmt>(stmt))
            tr_unit.classes_.push_back(as_a<ClassDefStmt>(stmt));
        else if (is_a<InterfaceDefStmt>(stmt))
            tr_unit.interfaces_.push_back(as_a<InterfaceDefStmt>(stmt));
    }
}

Type* CSharpTSTreeVisitor::make_type(CSharpTSTreeVisitor const* self, TSNode const* node)
{
    std::string type_name          = extract_node_text(*node, self->source_code_);
    char const last_char           = type_name[type_name.length() - 1];
    bool const is_indirection_type = last_char == '*' || last_char == '&';
    if (is_indirection_type)
    {
        type_name.pop_back();
    }

    std::erase_if(type_name, isspace);
    auto const res = NodeRegistry::get_type(type_name);
    // todo implement scope
    Type* type = res.has_value() ? *res : type_factory_.mk_class(type_name, {});

    return is_indirection_type ? type_factory_.mk_indirect(type) : type;
}

Expr* CSharpTSTreeVisitor::handle_int_lit(CSharpTSTreeVisitor* self, TSNode const* node)
{
    std::string int_str = extract_node_text(*node, self->source_code_);
    std::erase(int_str, '_');

    size_t const string_len  = int_str.length();
    std::string const prefix = int_str.substr(0, std::min<size_t>(2, string_len));
    std::string const suffix
        = int_str.substr(string_len >= 2 ? string_len - 2 : 0, std::min<size_t>(2, string_len));

    int base = 10;
    if (prefix == "0x")
    {
        base = 16;
    }
    else if (prefix == "0b")
    {
        base = 2;
    }

    IntSuffix const suffix_type = get_suffix_type(suffix);

    switch (suffix_type)
    {
    case IntSuffix::U:
    case IntSuffix::L:
    {
        int_str.pop_back();
        break;
    }
    case IntSuffix::UL:
    {
        int_str.erase(int_str.end() - 2, int_str.end());
        break;
    }
    default:
        break;
    }

    if (suffix_type == IntSuffix::None || suffix_type == IntSuffix::U)
    {
        // todo add handeling of unsigned integers
        return ExprFactory::get_instance().mk_int_literal(std::stoi(int_str, nullptr, base));
    }

    // todo handeling of long and unsigned long
    throw std::logic_error("This integer type is not implemented");
}

Expr* CSharpTSTreeVisitor::handle_float_lit(CSharpTSTreeVisitor* self, TSNode const* node)
{
    std::string float_str = extract_node_text(*node, self->source_code_);
    char const suffix     = static_cast<char>(std::tolower(float_str[float_str.length() - 1]));

    float_str.pop_back();
    std::erase(float_str, '_');

    if (! std::isalpha(suffix) || suffix == 'd')
    {
        // todo handle double
        throw std::logic_error("Handling of double floating point numbers is not implemented");
    }

    switch (suffix)
    {
    case 'f':
        return ExprFactory::get_instance().mk_float_literal(std::stof(float_str));
    case 'm':
        // decimal - 128-bit precision integer - used base 10, not base 2
        // todo handle decimal
        throw std::logic_error("Handling of Decimal literal not implemented");
    default:
        throw std::logic_error("Suffix \"" + std::to_string(suffix) + "\" Not Implemented");
    };
}

Expr* CSharpTSTreeVisitor::handle_bool_lit(CSharpTSTreeVisitor* self, TSNode const* node)
{
    std::string const bool_str = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_bool_literal(bool_str == "true");
}

Expr* CSharpTSTreeVisitor::handle_char_lit(CSharpTSTreeVisitor* self, TSNode const* node)
{
    TSNode const content_node       = ts_node_child(*node, 1);
    std::string const character_str = extract_node_text(content_node, self->source_code_);
    if (character_str.length() > 1)
    {
        // todo handle 16-bit unicode characters
        throw std::logic_error("Unicode characters not implemented");
    }
    return ExprFactory::get_instance().mk_char_literal(character_str[0]);
}

Expr* CSharpTSTreeVisitor::handle_str_lit(CSharpTSTreeVisitor* self, TSNode const* node)
{
    if (ts_node_child_count(*node) < 3)
        return ExprFactory::get_instance().mk_string_literal("");

    // todo handle escape sequences

    TSNode const str_content  = ts_node_child(*node, 1);
    std::string const content = extract_node_text(str_content, self->source_code_);
    return ExprFactory::get_instance().mk_string_literal(content);
}

Expr* CSharpTSTreeVisitor::handle_null_literal(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    return ExprFactory::get_instance().mk_null_literal();
}

Expr* CSharpTSTreeVisitor::handle_this_expr(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    return ExprFactory::get_instance().mk_this();
}

Expr* CSharpTSTreeVisitor::handle_verbatim_str_lit(CSharpTSTreeVisitor* self, TSNode const* node)
{
    std::string node_contet = extract_node_text(*node, self->source_code_);
    node_contet.pop_back();
    node_contet.erase(node_contet.begin(), node_contet.begin() + 2);

    return ExprFactory::get_instance().mk_string_literal(node_contet);
}

Expr* CSharpTSTreeVisitor::handle_interpolated_str_lit(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    throw std::logic_error("Interpolated string literal not implemented");
}

Expr* CSharpTSTreeVisitor::handle_param_var_ref_expr(CSharpTSTreeVisitor* self, TSNode const* node)
{
    // todo this might be not correctly done
    std::string const var_name = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_param_var_ref(var_name);
}

Expr* CSharpTSTreeVisitor::handle_local_var_ref_expr(CSharpTSTreeVisitor* self, TSNode const* node)
{
    std::string const var_name = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_local_var_ref(var_name);
}

Expr* CSharpTSTreeVisitor::handle_prefix_unary_op_expr(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSNode const right_side_node = ts_node_child(*node, 1);
    TSNode const op_node         = ts_node_child(*node, 0);
    std::string op               = extract_node_text(op_node, self->source_code_);
    std::erase_if(op, isspace);

    auto const res = NodeRegistry::get_prefix_unary_op(op);
    if (! res.has_value())
    {
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    UnaryOpType const op_type = *res;
    ExprHandler const handler = NodeRegistry::get_expr_handler(right_side_node);
    Expr* right_side          = handler(self, &right_side_node);
    return ExprFactory::get_instance().mk_unary_op(op_type, right_side);
}

Expr* CSharpTSTreeVisitor::handle_postfix_unary_op_expr(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSNode const left_side_node = ts_node_child(*node, 0);
    TSNode const op_node        = ts_node_child(*node, 1);
    std::string const op        = extract_node_text(op_node, self->source_code_);

    ExprHandler const handler   = NodeRegistry::get_expr_handler(left_side_node);
    Expr* left_side             = handler(self, &left_side_node);

    UnaryOpType op_type;
    if (op == "++")
    {
        op_type = UnaryOpType::PostIncrement;
    }
    else if (op == "--")
    {
        op_type = UnaryOpType::PostDecrement;
    }
    else if (op == "!")
    {
        return left_side;
    }
    else
    {
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    return ExprFactory::get_instance().mk_unary_op(op_type, left_side);
}

Expr* CSharpTSTreeVisitor::handle_binary_op_expr(CSharpTSTreeVisitor* self, TSNode const* node)
{
    ExprFactory& expr_factory       = ExprFactory::get_instance();

    TSNode const left               = ts_node_child(*node, 0);
    TSNode const op_node            = ts_node_child(*node, 1);
    TSNode const right              = ts_node_child(*node, 2);
    ExprHandler const left_handler  = NodeRegistry::get_expr_handler(left);
    ExprHandler const right_handler = NodeRegistry::get_expr_handler(right);
    std::string const op            = extract_node_text(op_node, self->source_code_);

    auto const res                  = NodeRegistry::get_bin_op(op);
    if (! res.has_value())
    {
        // `a ?? b` same as `a != null ? a : b`
        Expr* left_expr      = left_handler(self, &left);
        BinOpExpr* condition = expr_factory.mk_bin_on(
            left_expr,
            BinOpType::NotEqual,
            expr_factory.mk_null_literal()
        );
        IfExpr* ternary = expr_factory.mk_if(condition, left_expr, right_handler(self, &right));
        if (op == "\?\?")
        {
            return ternary;
        }
        if (op == "\?\?=")
        {
            // `a ??= b` is the same as `a = a ?? b` which is the same as `a = a != null ? a : b`
            return expr_factory.mk_bin_on(left_expr, BinOpType::Assign, ternary);
        }

        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    BinOpType const op_type = *res;

    return expr_factory.mk_bin_on(left_handler(self, &left), op_type, right_handler(self, &right));
}

Expr* CSharpTSTreeVisitor::handle_ternary_expr(CSharpTSTreeVisitor* self, TSNode const* node)
{
    TSNode const cond_node             = ts_node_child(*node, 0);
    TSNode const if_true               = ts_node_child(*node, 2);
    TSNode const if_false              = ts_node_child(*node, 4);
    ExprHandler const cond_handler     = NodeRegistry::get_expr_handler(cond_node);
    ExprHandler const if_true_handler  = NodeRegistry::get_expr_handler(if_true);
    ExprHandler const if_false_handler = NodeRegistry::get_expr_handler(if_false);

    return ExprFactory::get_instance().mk_if(
        cond_handler(self, &cond_node),
        if_true_handler(self, &if_true),
        if_false_handler(self, &if_false)
    );
}

Stmt* CSharpTSTreeVisitor::handle_class_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{

    // todo get base class and implemented interfaces - needs type resolver

    static std::vector<std::string> const class_memb_node_types = {
        // "class_declaration", // todo
        // "enum_declaration", // todo
        // "interface_declaration", // todo
        // "struct_declaration", // todo
        // "record_declaration", // todo
        "field_declaration",
        "delegate_declaration", // todo
        "event_field_declaration", // todo
        "constructor_declaration",
        "property_declaration", // todo
        "method_declaration",
        "destructor_declaration",
        "indexer_declaration", // todo
        "operator_declaration", // todo
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
    ClassDefStmt* base = nullptr;
    std::vector<InterfaceDefStmt*> interfaces;
    do
    {
        char const* field_name = ts_tree_cursor_current_field_name(&cursor);
        if (field_name != nullptr) // body node is named, needs to stop at the body node
        {
            break;
        }
        TSNode current = ts_tree_cursor_current_node(&cursor);
        std::string name = ts_node_type(current);
        if (name == "type_parameter_list")
        {
            // todo handle generic parameters;
        }
        else if (name == "base_list")
        {
            ts_tree_cursor_goto_first_child(&cursor);
            TSNode type_node = ts_tree_cursor_current_node(&cursor);
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
    class_def->interfaces_ = interfaces;

    TSNode const class_body_node = ts_tree_cursor_current_node(&cursor);
    ts_tree_cursor_delete(&cursor);

    TSTreeCursor body_cursor     = ts_tree_cursor_new(class_body_node);
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
            StmtHandler handler = NodeRegistry::get_stmt_handler(member_node);
            if (ts_node_is_null(member_node))
            {
                throw std::runtime_error("Node is null");
            }
            Stmt* member_stmt = handler(self, &member_node);

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

    Type* type                 = make_type(self, &type_node);
    std::vector<TSNode> const var_decl_nodes
        = find_nodes(var_decl_node, self->language_, decl_query);

    std::vector<VarDefStmt*> var_def_stmts;
    for (TSNode const& var_decltor_node : var_decl_nodes)
    {
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

Stmt* CSharpTSTreeVisitor::handle_decl_list_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{
    return stmt_factory_.mk_compound({});
}

Stmt* CSharpTSTreeVisitor::handle_arrow_expr_clause(CSharpTSTreeVisitor* self, TSNode const* node)
{
    return stmt_factory_.mk_compound({});
}

Scope CSharpTSTreeVisitor::create_scope(TSNode const* node) const
{
    enum NodeType
    {
        Class,
        Interface,
        Namespace,
        Root,
    };

    static std::unordered_map<std::string, NodeType> node_type_map = {
        {"class_declaration",     Class    },
        {"interface_declaration", Interface},
        {"namespace_declaration", Namespace},
        {"compilation_unit",      Root     },
    };

    std::stack<std::string> scope_str;
    Scope scope           = {};
    TSNode current        = *node;
    TSNode parent         = ts_node_parent(current);

    bool found_name_space = false;
    while (! ts_node_is_null(parent))
    {
        std::string const parent_type = ts_node_type(parent);
        auto const res                = node_type_map.find(parent_type);
        current                       = parent;
        parent                        = ts_node_parent(current);

        if (res == node_type_map.end())
            continue;

        switch (res->second)
        {
        case Class:
        case Interface:
        {
            TSNode const name_node = ts_node_child_by_field_name(current, "name", 4);
            std::string const name = extract_node_text(name_node, this->source_code_);
            scope_str.push(name);
            break;
        }
        case Root:
        {
            if (found_name_space)
                break;

            std::string file_namespace_query = "(file_scoped_namespace_declaration) @namespace";
            TSNode const namespace_node
                = find_first_node(current, this->language_, file_namespace_query);
            if (ts_node_is_null(namespace_node))
                break;

            TSNode const name_node = ts_node_child_by_field_name(namespace_node, "name", 4);
            std::string const name = extract_node_text(name_node, this->source_code_);
            split_namespace(scope_str, name);
            break;
        }
        case Namespace:
        {
            found_name_space       = true;
            TSNode const name_node = ts_node_child_by_field_name(current, "name", 4);
            std::string const name = extract_node_text(name_node, this->source_code_);
            split_namespace(scope_str, name);
            break;
        }
        }
    }

    while (! scope_str.empty())
    {
        scope.names_.push_back(scope_str.top());
        scope_str.pop();
    }
    return scope;
}

} // namespace astfri::csharp