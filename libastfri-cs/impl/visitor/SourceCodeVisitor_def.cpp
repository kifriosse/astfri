#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri-cs/impl/util/utils.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace astfri::csharp
{
Stmt* SrcCodeVisitor::visit_class_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    // todo refactor this to use TSSymbol
    // todo refactor this whole method
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
        = util::symbol_for_name(self->lang_, "base_list", true);
    static const TSSymbol type_param_list_sym
        = util::symbol_for_name(self->lang_, "type_parameter_list", true);
    static const TSSymbol type_param_constr_sym = util::symbol_for_name(
        self->lang_,
        "type_parameter_constraints_clause",
        true
    );

    RegistryMap<std::vector<TSNode>> n_class_membs;
    for (const std::string_view node_type : class_memb_node_types)
    {
        n_class_membs[node_type];
    }

    Scope scope               = util::create_scope(node, self->src_str());
    const TSNode n_class_name = util::child_by_field_name(node, "name");
    std::string class_name  = util::extract_text(n_class_name, self->src_str());

    ClassDefStmt* class_def = stmt_f_.mk_class_def(class_name, scope);
    class_def->name_        = std::move(class_name); // todo remove this
    // todo use move semantic for the string

    self->semantic_context_.enter_type(class_def);
    self->type_tr_.set_current_namespace(std::move(scope));

    TSNode n_current          = ts_node_next_sibling(n_class_name);
    const TSNode n_class_body = util::child_by_field_name(node, "body");
    // handling of base class and interface implementations
    ClassDefStmt* base = nullptr;
    std::vector<InterfaceDefStmt*> interfaces;
    do
    {
        if (ts_node_eq(n_current, n_class_body))
            break;

        TSSymbol current_symb = ts_node_symbol(n_current);
        if (current_symb == base_list_symb) // base list handeling
        {
            TSNode n_type         = ts_node_named_child(n_current, 0);
            std::string type_name = util::extract_text(n_type, self->src_str());
            // todo temporary solution
            if (util::is_interface_name(type_name))
            {
                interfaces.emplace_back(
                    // todo temporary solution - needs proper scope resolving
                    stmt_f_.mk_interface_def(std::move(type_name), {})
                );
            }
            else
            {
                // todo temporary solution - needs proper scope resolving
                base = stmt_f_.mk_class_def(std::move(type_name), {});
            }

            n_type = ts_node_next_named_sibling(n_type);
            while (! ts_node_is_null(n_type))
            {
                type_name = util::extract_text(n_type, self->src_str());
                // todo temporary solution
                interfaces.emplace_back(
                    stmt_f_.mk_interface_def(std::move(type_name), {})
                );
                n_type = ts_node_next_named_sibling(n_type);
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
        n_current = ts_node_next_sibling(n_current);
    } while (! ts_node_is_null(n_current));

    if (base && class_def->bases_.empty())
    {
        class_def->bases_.push_back(base);
    }

    if (class_def->interfaces_.empty())
    {
        class_def->interfaces_ = std::move(interfaces);
    }

    // if its partial class doesn't have a body
    if (ts_node_is_null(n_class_body))
        return class_def;

    auto process = [&n_class_membs](TSNode current)
    {
        const std::string type = ts_node_type(current);
        const auto it          = n_class_membs.find(type);
        if (it != n_class_membs.end())
            it->second.push_back(current);
    };
    util::for_each_child_node(n_class_body, process);

    // handling of all member statements
    for (const std::string_view name : class_memb_node_types)
    {
        const std::vector<TSNode>& n_members = n_class_membs[name];
        for (const TSNode& n_member : n_members)
        {
            if (ts_node_is_null(n_member))
                continue;

            StmtHandler handler = RegManager::get_stmt_handler(n_member);
            Stmt* member_stmt   = handler(self, n_member);

            if (auto var_def = as_a<MemberVarDefStmt>(member_stmt))
                class_def->vars_.push_back(var_def);
            else if (auto constr = as_a<ConstructorDefStmt>(member_stmt))
                class_def->constructors_.push_back(constr);
            else if (auto destr = as_a<DestructorDefStmt>(member_stmt))
                class_def->destructors_.push_back(destr);
            else if (auto method = as_a<MethodDefStmt>(member_stmt))
                class_def->methods_.push_back(method);
        }
    }

    self->semantic_context_.leave_type();
    return class_def;
}

Stmt* SrcCodeVisitor::visit_interface_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    [[maybe_unused]] static const std::vector<std::string_view>
        class_memb_node_types = {
            // "class_declaration",              // todo
            // "enum_declaration",               // todo
            // "interface_declaration",          // todo
            // "struct_declaration",             // todo
            // "record_declaration",             // todo
            "field_declaration", // todo
            // "delegate_declaration",    // todo
            // "event_field_declaration", // todo
            "property_declaration", // todo
            "method_declaration",   // todo
            // "indexer_declaration",     // todo
        };

    const TSNode n_name = util::child_by_field_name(node, "name");
    std::string name    = util::extract_text(n_name, self->src_str());
    Scope scope         = util::create_scope(node, self->src_str());

    InterfaceDefStmt* intr_def
        = stmt_f_.mk_interface_def(std::move(name), std::move(scope));

    // todo not finished
    // needs type resolver to work properly

    return intr_def;
}

Stmt* SrcCodeVisitor::visit_memb_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    return self->visit_var_def_stmt(node, util::VarDefType::Member);
}

Stmt* SrcCodeVisitor::visit_local_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    return self->visit_var_def_stmt(node, util::VarDefType::Local);
}

Stmt* SrcCodeVisitor::visit_global_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode var_def = ts_node_child(node, 0);
    return self->visit_var_def_stmt(var_def, util::VarDefType::Global);
}

Stmt* SrcCodeVisitor::visit_param_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode var_name_node = util::child_by_field_name(node, "name");
    const TSNode init_node     = ts_node_next_named_sibling(var_name_node);

    ParamVarDefStmt* parameter
        = util::make_param_def(node, *self->src(), self->type_tr_);
    if (! ts_node_is_null(init_node))
    {
        const ExprHandler init_handler
            = RegManager::get_expr_handler(init_node);
        parameter->initializer_ = init_handler(self, init_node);
    }
    self->semantic_context_.reg_param(parameter);
    return parameter;
}

Stmt* SrcCodeVisitor::visit_constr_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    self->semantic_context_.enter_scope();
    self->semantic_context_.reg_return(type_f_.mk_void());
    ConstructorDefStmt* constr_def = stmt_f_.mk_constructor_def();

    const auto current_type        = self->semantic_context_.current_type();
    if (! current_type)
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(current_type))
        throw std::logic_error(
            "Constructor can only be defined for class type"
        );

    const TSNode n_param_list = util::child_by_field_name(node, "parameters");
    const TSNode n_body       = util::child_by_field_name(node, "body");
    const TSNode n_init       = ts_node_next_sibling(n_param_list);

    constr_def->owner_        = as_a<ClassDefStmt>(current_type);
    constr_def->params_       = self->make_param_list(n_param_list, false);

    const std::vector<TSNode> n_modifs = util::find_nodes(
        node,
        regs::QueryType::MethodModifier
    ); // todo move this into query registyr
    const CSModifiers modifs
        = CSModifiers::handle_modifiers(n_modifs, self->src_str());
    constr_def->access_
        = modifs.get_access_mod().value_or(AccessModifier::Private);

    const StmtHandler h_body = RegManager::get_stmt_handler(n_body);
    constr_def->body_        = as_a<CompoundStmt>(h_body(self, n_body));

    if (! ts_node_is_null(n_init))
    {
        const StmtHandler h_base_init = RegManager::get_stmt_handler(n_init);
        Stmt* init_stmt               = h_base_init(self, n_init);
        if (const auto base_init = as_a<BaseInitializerStmt>(init_stmt))
            constr_def->baseInit_.push_back(base_init);
        else if (const auto self_init = as_a<SelfInitializerStmt>(init_stmt))
            constr_def->selfInitializers.push_back(self_init);
    }

    self->semantic_context_.leave_scope();
    self->semantic_context_.unregister_return_type();
    return constr_def;
}

Stmt* SrcCodeVisitor::visit_construct_init(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    constexpr std::string_view this_init_sv = "this";
    std::string src       = util::extract_text(node, self->src_str());
    const auto bracket_it = std::ranges::find(src, '(');
    const auto this_it    = std::search(
        src.begin(),
        bracket_it,
        this_init_sv.begin(),
        this_init_sv.end()
    );

    const TSNode n_arg_list = ts_node_named_child(node, 0);
    std::vector<Expr*> args = self->visit_arg_list(n_arg_list);
    if (this_it != bracket_it)
        return stmt_f_.mk_self_initializer(args);

    const auto current_type = self->semantic_context_.current_type();
    if (! current_type)
        throw std::logic_error("Owner type not found");
    const auto* owner = as_a<ClassDefStmt>(current_type);
    // todo add records
    if (! owner)
        throw std::logic_error(
            "Constructor can only be defined for class type"
        );

    // todo fix this for Incomplete types
    if (owner->bases_.empty())
        throw std::logic_error(
            "Constructor can't have base initializer without having defined "
            "base class"
        );
    const ClassDefStmt* base = owner->bases_.back();
    return stmt_f_.mk_base_initializer(base->type_, std::move(args));
}

Stmt* SrcCodeVisitor::visit_destr_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    self->semantic_context_.reg_return(type_f_.mk_void());
    const TSNode n_body      = util::child_by_field_name(node, "body");
    const StmtHandler h_body = RegManager::get_stmt_handler(n_body);
    Stmt* body               = h_body(self, n_body);
    const auto current_type  = self->semantic_context_.current_type();

    if (! current_type)
        throw std::logic_error("Owner type not found");
    auto* owner = as_a<ClassDefStmt>(current_type);
    if (! owner)
        throw std::logic_error("Destructor can only be defined for class type");

    self->semantic_context_.unregister_return_type();
    return stmt_f_.mk_destructor_def(owner, as_a<CompoundStmt>(body));
}

Stmt* SrcCodeVisitor::visit_method_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const auto current_type = self->semantic_context_.current_type();
    if (! current_type)
        throw std::logic_error("Owner type not found");

    const TSNode n_params = util::child_by_field_name(node, "parameters");
    const std::vector<TSNode> n_modifs
        = util::find_nodes(node, regs::QueryType::MethodModifier);
    const CSModifiers modifs
        = CSModifiers::handle_modifiers(n_modifs, self->src_str());
    const TSNode n_name        = util::child_by_field_name(node, "name");
    const bool is_variadic     = util::has_variadic_param(n_params, nullptr);
    const size_t named_child_c = ts_node_named_child_count(n_params);
    const size_t param_c = is_variadic ? named_child_c - 1 : named_child_c;

    const MethodId method_id{
        .func_id
        = FuncId{.name = util::extract_text(n_name, self->src_str()), .param_count = param_c},
        .is_static = modifs.has(CSModifier::Static)
    };

    const MethodMetadata* method_meta
        = self->semantic_context_.find_method(method_id, current_type);
    if (! method_meta)
        throw std::logic_error(
            "Method \'" + method_id.func_id.name + "\' not found"
        );

    // if method could be resolved
    if (method_meta->method_def)
    {
        auto& method_def = method_meta->method_def;
        self->semantic_context_.enter_scope();
        self->semantic_context_.reg_return(method_def->func_->retType_);

        for (auto& [param_def, n_param, n_init] : method_meta->params)
        {
            if (! ts_node_is_null(n_init))
            {
                ExprHandler h_init      = RegManager::get_expr_handler(n_init);
                param_def->initializer_ = h_init(self, n_init);
            }
            self->semantic_context_.reg_param(param_def);
            // util::print_child_nodes_types(param_node, self->get_src_code());
        }

        const TSNode n_body
            = util::child_by_field_name(method_meta->method_node, "body");
        const StmtHandler h_body = RegManager::get_stmt_handler(n_body);
        method_def->func_->body_ = as_a<CompoundStmt>(h_body(self, n_body));

        self->semantic_context_.leave_scope();

        return method_def;
    }

    // if it wansn't able to resolve method
    MethodDefStmt* method_def_stmt = stmt_f_.mk_method_def();
    method_def_stmt->owner_        = current_type;
    method_def_stmt->func_         = self->make_func_stmt(node, true);
    method_def_stmt->access_
        = modifs.get_access_mod().value_or(AccessModifier::Internal);
    return method_def_stmt;
}

Stmt* SrcCodeVisitor::visit_func_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode n_name    = util::child_by_field_name(node, "name");
    const TSNode n_params  = util::child_by_field_name(node, "parameters");
    const bool is_variadic = util::has_variadic_param(n_params);
    const size_t named_c   = ts_node_named_child_count(n_params);

    const FuncId func_id{
        .name        = util::extract_text(n_name, self->src_str()),
        .param_count = is_variadic ? named_c - 1 : named_c
    };
    const FuncMetadata* func_meta = self->semantic_context_.find_func(func_id);
    if (! func_meta)
        throw std::logic_error(
            "Local function \'" + func_id.name + "\' not found"
        );

    if (! func_meta->func_def)
        return self->make_func_stmt(node, false);

    self->semantic_context_.enter_scope();
    self->semantic_context_.reg_return(func_meta->func_def->retType_);
    for (const auto& param_meta : func_meta->params)
    {
        const TSNode n_init = param_meta.initializer;
        if (! ts_node_is_null(n_init))
        {
            ExprHandler h_init = RegManager::get_expr_handler(n_init);
            param_meta.param_def->initializer_ = h_init(self, n_init);
        }
        self->semantic_context_.reg_param(param_meta.param_def);
    }

    const TSNode n_body        = util::child_by_field_name(node, "body");
    const StmtHandler h_body   = RegManager::get_stmt_handler(n_body);
    func_meta->func_def->body_ = as_a<CompoundStmt>(h_body(self, n_body));
    self->semantic_context_.leave_scope();
    self->semantic_context_.unregister_return_type();
    return func_meta->func_def;
}

} // namespace astfri::csharp