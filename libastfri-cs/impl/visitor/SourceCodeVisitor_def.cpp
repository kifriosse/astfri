
#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri-cs/impl/util/utils.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace astfri::csharp
{
Stmt* SrcCodeVisitor::handle_class_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    static const std::vector<std::string_view> class_memb_node_types = {
        // "class_declaration",              // todo
        // "enum_declaration",               // todo
        // "interface_declaration",          // todo
        // "struct_declaration",             // todo
        // "record_declaration",             // todo
        "field_declaration",
        // "property_declaration",    // todo
        // "delegate_declaration",    // todo
        // "event_field_declaration", // todo
        "constructor_declaration",
        "method_declaration",
        // "indexer_declaration",            // todo
        // "operator_declaration",           // todo
        // "conversion_operator_declaration" // todo
        "destructor_declaration",
    };
    static const TSSymbol base_list_symb
        = util::symbol_for_name(self->get_lang(), "base_list", true);
    static const TSSymbol type_param_list_sym
        = util::symbol_for_name(self->get_lang(), "type_parameter_list", true);
    static const TSSymbol type_param_constr_sym = util::symbol_for_name(
        self->get_lang(),
        "type_parameter_constraints_clause",
        true
    );

    std::unordered_map<std::string_view, std::vector<TSNode>>
        class_members_nodes;
    for (const std::string_view node_type : class_memb_node_types)
    {
        class_members_nodes[node_type];
    }

    const Scope scope
        = util::create_scope(*node, self->get_lang(), self->get_src_code());
    const TSNode class_name_node = util::child_by_field_name(*node, "name");
    const std::string class_name
        = util::extract_node_text(class_name_node, self->get_src_code());

    ClassDefStmt* class_def = stmt_factory_.mk_class_def(class_name, scope);
    class_def->name_        = class_name; // todo remove this

    self->semantic_context_.enter_type(class_def);

    TSNode current_node          = ts_node_next_sibling(class_name_node);
    const TSNode class_body_node = util::child_by_field_name(*node, "body");
    // handling of base class and interface implementations
    ClassDefStmt* base = nullptr;
    std::vector<InterfaceDefStmt*> interfaces;
    do
    {
        if (ts_node_eq(current_node, class_body_node))
            break;

        TSSymbol current_symb = ts_node_symbol(current_node);
        if (current_symb == base_list_symb) // base list handeling
        {
            TSNode type_node = ts_node_child(current_node, 1);
            std::string type_name
                = util::extract_node_text(type_node, self->get_src_code());
            // todo temporary solution
            if (util::is_interface_name(type_name))
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
                type_name
                    = util::extract_node_text(type_node, self->get_src_code());
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

    if (base && class_def->bases_.empty())
    {
        class_def->bases_.push_back(base);
    }

    if (class_def->interfaces_.empty())
    {
        class_def->interfaces_ = std::move(interfaces);
    }

    // if its partial class doesn't have a body
    if (ts_node_is_null(class_body_node))
        return class_def;

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
    for (const std::string_view name : class_memb_node_types)
    {
        const std::vector<TSNode>& members_nodes = class_members_nodes[name];
        for (const TSNode& member_node : members_nodes)
        {
            if (ts_node_is_null(member_node))
                throw std::runtime_error("Node is null");

            StmtHandler handler = RegManager::get_stmt_handler(member_node);
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

Stmt* SrcCodeVisitor::handle_interface_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    static const std::vector<std::string_view> class_memb_node_types = {
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

    const TSNode name_node = util::child_by_field_name(*node, "name");
    const std::string name
        = util::extract_node_text(name_node, self->get_src_code());
    const Scope scope
        = util::create_scope(*node, self->get_lang(), self->get_src_code());

    InterfaceDefStmt* interface_def_stmt
        = stmt_factory_.mk_interface_def(name, scope);

    // todo not finished
    // needs type resolver to work properly

    return interface_def_stmt;
}

Stmt* SrcCodeVisitor::handle_memb_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    return handle_var_def_stmt(self, node, util::VarDefType::Member);
}

Stmt* SrcCodeVisitor::handle_local_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    return handle_var_def_stmt(self, node, util::VarDefType::Local);
}

Stmt* SrcCodeVisitor::handle_global_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode local_var_def_node = ts_node_child(*node, 0);
    return handle_var_def_stmt(
        self,
        &local_var_def_node,
        util::VarDefType::Global
    );
}

Stmt* SrcCodeVisitor::handle_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node,
    const util::VarDefType def_type
)
{
    // todo refactor to use the Semantic context and Type Table
    const std::vector<TSNode> modifier_nodes = util::find_nodes(
        *node,
        self->get_lang(),
        regs::Queries::var_modif_query
    );

    const TSNode var_decl_node
        = modifier_nodes.empty() ? ts_node_child(*node, 0)
                                 : ts_node_next_sibling(modifier_nodes.back());

    const TSNode type_node = util::child_by_field_name(var_decl_node, "type");
    Type* type             = util::make_type(type_node, self->get_src_code());

    const std::vector<TSNode> var_decltor_nodes = util::find_nodes(
        var_decl_node,
        self->get_lang(),
        regs::Queries::decl_query
    );

    std::vector<VarDefStmt*> var_def_stmts;
    for (const TSNode& var_decltor_node : var_decltor_nodes)
    {
        // todo refactor this into extra method
        TSNode var_name_node = ts_node_child(var_decltor_node, 0); // left side
        TSNode init_node     = ts_node_child(var_decltor_node, 2); // right side

        const std::string var_name
            = util::extract_node_text(var_name_node, self->get_src_code());
        Expr* init = nullptr;
        if (! ts_node_is_null(init_node))
        {
            ExprHandler init_handler = RegManager::get_expr_handler(init_node);
            init                     = init_handler(self, &init_node);
        }

        VarDefStmt* var_def_stmt = nullptr;
        switch (def_type)
        {
        case util::VarDefType::Member:
        {
            // todo handle other modifiers
            MemberVarMetadata* var_data = self->semantic_context_.find_memb_var(
                var_name,
                self->semantic_context_.current_type()
            );
            var_def_stmt               = var_data->var_def;
            var_def_stmt->initializer_ = init;
            var_data->processed        = true;
            break;
        }
        case util::VarDefType::Local:
            // const CSModifiers modifiers
            //     = CSModifiers::handle_modifiers(modifier_nodes,
            //     self->src_code_);
            // todo handle const
            var_def_stmt = stmt_factory_.mk_local_var_def(var_name, type, init);
            self->semantic_context_.reg_local_var(
                as_a<LocalVarDefStmt>(var_def_stmt)
            );
            break;
        case util::VarDefType::Global:
            // todo handle const
            var_def_stmt
                = stmt_factory_.mk_global_var_def(var_name, type, init);
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

Stmt* SrcCodeVisitor::handle_param_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode var_name_node = util::child_by_field_name(*node, "name");
    const TSNode init_node     = ts_node_next_named_sibling(var_name_node);

    ParamVarDefStmt* parameter
        = util::make_param_def(*node, self->get_lang(), self->get_src_code());
    if (! ts_node_is_null(init_node))
    {
        const ExprHandler init_handler
            = RegManager::get_expr_handler(init_node);
        parameter->initializer_ = init_handler(self, &init_node);
    }
    self->semantic_context_.reg_param(parameter);
    return parameter;
}

Stmt* SrcCodeVisitor::handle_constr_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    self->semantic_context_.enter_scope();
    self->semantic_context_.reg_return(type_factory_.mk_void());
    ConstructorDefStmt* constructor_def = stmt_factory_.mk_constructor_def();

    const auto result = self->semantic_context_.current_type();
    if (! result)
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(result))
        throw std::logic_error(
            "Constructor can only be defined for class type"
        );

    const TSNode param_list_node
        = util::child_by_field_name(*node, "parameters");
    const TSNode body_node        = util::child_by_field_name(*node, "body");
    const TSNode initializer_node = ts_node_next_sibling(param_list_node);

    constructor_def->owner_       = as_a<ClassDefStmt>(result);
    constructor_def->params_ = make_param_list(self, &param_list_node, false);

    const std::vector<TSNode> modifier_nodes
        = util::find_nodes(*node, self->get_lang(), "(modifier) @mod");
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, self->get_src_code());
    constructor_def->access_
        = modifiers.get_access_mod().value_or(AccessModifier::Private);

    const StmtHandler body_handler = RegManager::get_stmt_handler(body_node);
    constructor_def->body_ = as_a<CompoundStmt>(body_handler(self, &body_node));

    if (! ts_node_is_null(initializer_node))
    {
        const StmtHandler base_init_handler
            = RegManager::get_stmt_handler(initializer_node);
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

Stmt* SrcCodeVisitor::handle_construct_init(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    constexpr std::string_view this_init_sv = "this";
    std::string source_code
        = util::extract_node_text(*node, self->get_src_code());
    const auto bracket_it = std::ranges::find(source_code, '(');
    const auto this_it    = std::search(
        source_code.begin(),
        bracket_it,
        this_init_sv.begin(),
        this_init_sv.end()
    );

    const auto result = self->semantic_context_.current_type();
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

Stmt* SrcCodeVisitor::handle_destr_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    // self->semantic_context_.enter_scope();
    self->semantic_context_.reg_return(type_factory_.mk_void());
    const TSNode body_node         = util::child_by_field_name(*node, "body");
    const StmtHandler body_handler = RegManager::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);
    const auto owner               = self->semantic_context_.current_type();

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

Stmt* SrcCodeVisitor::handle_method_def_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    const auto current_type = self->semantic_context_.current_type();
    if (! current_type)
        throw std::logic_error("Owner type not found");

    const TSNode params_node = util::child_by_field_name(*node, "parameters");
    const std::vector<TSNode> modif_node = util::find_nodes(
        *node,
        self->get_lang(),
        regs::Queries::method_modif_query
    );
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modif_node, self->get_src_code());
    const TSNode name_node = util::child_by_field_name(*node, "name");
    const bool is_variadic = util::has_variadic_param(params_node, nullptr);
    const size_t named_child_count = ts_node_named_child_count(params_node);
    const size_t param_count
        = is_variadic ? named_child_count - 1 : named_child_count;

    const MethodId method_id{
        .func_id
        = FuncId{.name = util::extract_node_text(name_node, self->get_src_code()), .param_count = param_count},
        .is_static = modifiers.has(CSModifier::Static)
    };

    const MethodMetadata* method_data
        = self->semantic_context_.find_method(method_id, current_type);
    if (! method_data)
        throw std::logic_error(
            "Method \'" + method_id.func_id.name + "\' not found"
        );

    // if method could be resolved
    if (method_data->method_def)
    {
        auto& method_def = method_data->method_def;
        self->semantic_context_.enter_scope();
        self->semantic_context_.reg_return(method_def->func_->retType_);

        for (auto& [param_def, param_node, init] : method_data->params)
        {
            if (! ts_node_is_null(init))
            {
                ExprHandler init_handler = RegManager::get_expr_handler(init);
                param_def->initializer_  = init_handler(self, &init);
            }
            self->semantic_context_.reg_param(param_def);
            // util::print_child_nodes_types(param_node, self->get_src_code());
        }

        const TSNode body_node
            = util::child_by_field_name(method_data->method_node, "body");
        const StmtHandler handler = RegManager::get_stmt_handler(body_node);
        method_def->func_->body_
            = as_a<CompoundStmt>(handler(self, &body_node));

        self->semantic_context_.leave_scope();

        return method_def;
    }

    // if it wansn't able to resolve method
    MethodDefStmt* method_def_stmt = stmt_factory_.mk_method_def();
    method_def_stmt->owner_        = current_type;
    method_def_stmt->func_         = handle_function_stmt(self, node, true);
    method_def_stmt->access_
        = modifiers.get_access_mod().value_or(AccessModifier::Internal);
    return method_def_stmt;
}

Stmt* SrcCodeVisitor::handle_func_stmt(SrcCodeVisitor* self, const TSNode* node)
{
    const TSNode name_node    = util::child_by_field_name(*node, "name");
    const TSNode paramas_node = util::child_by_field_name(*node, "parameters");
    const bool is_variadic    = util::has_variadic_param(paramas_node, nullptr);
    const size_t named_count  = ts_node_named_child_count(paramas_node);

    const FuncId func_id{
        .name        = util::extract_node_text(name_node, self->get_src_code()),
        .param_count = is_variadic ? named_count - 1 : named_count
    };
    const FunctionMetadata* func = self->semantic_context_.find_func(func_id);
    if (! func)
        throw std::logic_error(
            "Local function \'" + func_id.name + "\' not found"
        );

    if (! func->func_def)
        return handle_function_stmt(self, node, false);

    self->semantic_context_.enter_scope();
    self->semantic_context_.reg_return(func->func_def->retType_);
    for (const auto& param_data : func->params)
    {
        const TSNode init_node = param_data.initializer;
        if (! ts_node_is_null(init_node))
        {
            ExprHandler init_hanlder = RegManager::get_expr_handler(init_node);
            param_data.param_def->initializer_ = init_hanlder(self, &init_node);
        }
        self->semantic_context_.reg_param(param_data.param_def);
    }

    const TSNode body              = util::child_by_field_name(*node, "body");
    const StmtHandler body_handler = RegManager::get_stmt_handler(body);
    func->func_def->body_ = as_a<CompoundStmt>(body_handler(self, &body));
    self->semantic_context_.leave_scope();
    self->semantic_context_.unregister_return_type();
    return func->func_def;
}

FunctionDefStmt* SrcCodeVisitor::handle_function_stmt(
    SrcCodeVisitor* self,
    const TSNode* node,
    const bool is_method
)
{
    self->semantic_context_.enter_scope();

    FunctionDefStmt* func_def = stmt_factory_.mk_function_def();
    const TSNode ret_type_node
        = util::child_by_field_name(*node, is_method ? "returns" : "type");
    const TSNode name_node  = util::child_by_field_name(*node, "name");
    const TSNode param_node = util::child_by_field_name(*node, "parameters");
    const TSNode body_node  = util::child_by_field_name(*node, "body");

    // todo handle generic parameters
    Type* ret_type = util::make_type(ret_type_node, self->get_src_code());
    self->semantic_context_.reg_return(ret_type);

    func_def->retType_ = ret_type;
    func_def->name_ = util::extract_node_text(name_node, self->get_src_code());
    func_def->params_              = make_param_list(self, &param_node, false);
    const StmtHandler body_handler = RegManager::get_stmt_handler(body_node);
    func_def->body_ = as_a<CompoundStmt>(body_handler(self, &body_node));

    self->semantic_context_.leave_scope();
    self->semantic_context_.unregister_return_type();
    return func_def;
}

} // namespace astfri::csharp