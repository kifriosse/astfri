#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/SymbolTableBuilder.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <ranges>
#include <string>
#include <vector>

namespace astfri::csharp
{
struct SourceCode;

StmtFactory& SymbolTableBuilder::stmt_factory_ = StmtFactory::get_instance();

SymbolTableBuilder::SymbolTableBuilder(
    std::vector<SourceCode>& srcs,
    SymbolTable& symb_table
) :
    type_tr_(symb_table),
    srcs_(srcs),
    symb_table_(symb_table),
    lang_(tree_sitter_c_sharp())
{
}

void SymbolTableBuilder::reg_user_types()
{
    for (auto& src : srcs_)
    {
        auto& [context, file, tree] = src;
        if (! tree)
            continue;

        current_src_ = &src;
        type_tr_.set_current_src(&src);
        const std::vector<TSNode> top_level_nodes = util::find_nodes(
            ts_tree_root_node(tree),
            ts_tree_language(tree),
            regs::Queries::top_level_stmt_query
        );
        for (const TSNode& node : top_level_nodes)
        {
            const RegHandler handler = RegManager::get_reg_handler(node);
            handler(this, node);
        }
    }
    current_src_ = nullptr;
}

void SymbolTableBuilder::reg_using_directives()
{
    for (auto& src : srcs_)
    {
        type_tr_.set_current_src(&src);
        current_src_                   = &src;
        TSNode root                    = ts_tree_root_node(src.tree);
        std::vector<TSNode> directives = util::find_nodes(
            root,
            lang_,
            regs::Queries::using_directives_query
        );
        for (auto& directive : directives)
        {
            // util::print_child_nodes_types(directive);
            add_using_directive(directive);
        }
    }
}

void SymbolTableBuilder::reg_members()
{
    for (auto& metadata : symb_table_.user_types_metadata | std::views::values)
    {
        for (auto& [node, src] : metadata.defs)
        {
            current_src_ = src;
            type_tr_.set_current_src(src);
            const TSNode class_body = util::child_by_field_name(node, "body");
            TSTreeCursor cursor     = ts_tree_cursor_new(class_body);
            type_context_.type_stack.push(metadata.user_type);

            if (! ts_tree_cursor_goto_first_child(&cursor))
                continue;

            do
            {
                TSNode current = ts_tree_cursor_current_node(&cursor);
                if (! ts_node_is_named(current))
                    continue;

                auto handler = RegManager::get_reg_handler(current);
                handler(this, current);
            } while (ts_tree_cursor_goto_next_sibling(&cursor));

            type_context_.type_stack.pop();
            ts_tree_cursor_delete(&cursor);
        }
    }
}

void SymbolTableBuilder::reg_class(SymbolTableBuilder* self, const TSNode& node)
{
    const std::string_view src = self->get_src_str();
    const TSNode n_name        = util::child_by_field_name(node, "name");
    // todo handle generic parameters
    const Scope scope       = util::create_scope(node, self->lang_, src);
    ClassDefStmt* class_def = stmt_factory_.mk_class_def(
        util::extract_node_text(n_name, src),
        scope
    );

    self->symb_table_.symb_tree.add_type(scope, class_def->type_, class_def);

    auto [it, inserted] = self->symb_table_.user_types_metadata.try_emplace(
        class_def,
        TypeMetadata{.user_type = class_def}
    );
    if (inserted)
    {
        self->symb_table_.user_type_keys.push_back(class_def);
    }
    it->second.defs.emplace_back(node, self->current_src_);
}

void SymbolTableBuilder::reg_interface(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::reg_record(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::reg_enum(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::reg_delegate(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::reg_memb_var(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    const std::string_view source_code = self->get_src_str();
    const std::vector<TSNode> modifier_nodes
        = util::find_nodes(node, self->lang_, regs::Queries::var_modif_query);
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, source_code);

    const TSNode var_decl_node
        = modifier_nodes.empty() ? ts_node_child(node, 0)
                                 : ts_node_next_sibling(modifier_nodes.back());

    const TSNode type_node = util::child_by_field_name(var_decl_node, "type");
    const TypeHandler th   = RegManager::get_type_handler(type_node);
    Type* type             = th(&self->type_tr_, type_node);

    const std::vector<TSNode> var_decltor_nodes = util::find_nodes(
        var_decl_node,
        self->lang_,
        regs::Queries::decl_query
    );

    TypeMetadata& type_metadata = self->symb_table_.user_types_metadata.at(
        self->type_context_.type_stack.top()
    );

    std::vector<VarDefStmt*> var_def_stmts;
    for (const TSNode& var_decltor_node : var_decltor_nodes)
    {
        TSNode var_name_node
            = ts_node_named_child(var_decltor_node, 0); // left side
        const std::string var_name
            = util::extract_node_text(var_name_node, source_code);
        MemberVarDefStmt* var_def = stmt_factory_.mk_member_var_def(
            var_name,
            type,
            nullptr,
            modifiers.get_access_mod().value_or(AccessModifier::Private)
        );

        var_def->name_ = var_name;
        var_def_stmts.push_back(var_def);
        MemberVarMetadata member_var_metadata{
            .var_def  = var_def,
            .var_node = var_decltor_node,
            .initializer
            = ts_node_named_child(var_decltor_node, 1) // right side
        };
        type_metadata.member_vars.emplace(var_name, member_var_metadata);
    }
}

void SymbolTableBuilder::reg_property(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::reg_method(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    const auto current_type = self->type_context_.type_stack.top();
    if (! current_type)
        throw std::logic_error("Owner type not found");

    auto& symb_table = self->symb_table_;
    const auto it_type_metadata
        = symb_table.user_types_metadata.find(current_type);
    if (! symb_table.user_types_metadata.contains(current_type))
        throw std::logic_error("Type wasn't discovered yet");

    const std::vector<TSNode> modif_nodes = util::find_nodes(
        node,
        self->lang_,
        regs::Queries::method_modif_query
    );
    const CSModifiers method_modif
        = CSModifiers::handle_modifiers(modif_nodes, self->get_src_str());
    const TSNode ret_type_node  = util::child_by_field_name(node, "returns");
    const TSNode func_name_node = util::child_by_field_name(node, "name");
    const TSNode params_node    = util::child_by_field_name(node, "parameters");

    const bool is_variadic = util::has_variadic_param(params_node, nullptr);
    const size_t named_child_count = ts_node_named_child_count(params_node);
    const size_t param_count
        = is_variadic ? named_child_count - 1 : named_child_count;
    const std::string name
        = util::extract_node_text(func_name_node, self->get_src_str());
    MethodId method_id{
        .func_id   = {name, param_count},
        .is_static = method_modif.has(CSModifier::Static),
    };
    // util::print_child_nodes_types(param_list_node);

    auto [params, params_metadata] = util::discover_params(
        params_node,
        self->get_src_str(),
        self->type_tr_
    );
    auto& methods                  = it_type_metadata->second.methods;
    const auto it_method           = methods.find(method_id);
    MethodDefStmt* method_def_stmt = nullptr;
    if (it_method == methods.end())
    {
        const TypeHandler th = RegManager::get_type_handler(ret_type_node);
        Type* t_ret          = th(&self->type_tr_, ret_type_node);
        method_def_stmt      = stmt_factory_.mk_method_def(
            current_type,
            stmt_factory_.mk_function_def(name, params, t_ret, nullptr),
            method_modif.get_access_mod().value_or(AccessModifier::Internal),
            method_modif.get_virtuality()
        );
    }

    MethodMetadata method_metadata{
        .params      = std::move(params_metadata),
        .method_def  = method_def_stmt,
        .method_node = node,
    };

    it_type_metadata->second.methods.emplace(method_id, method_metadata);
}

void SymbolTableBuilder::add_using_directive(const TSNode& node)
{
    // const static TSSymbol qualified_name_symb
    //     = util::symbol_for_name(lang, "qualified_name", true);
    // const static TSSymbol generic_name_symb
    //     = util::symbol_for_name(lang, "generic_name", true);
    static const TSSymbol s_global
        = util::symbol_for_name(lang_, "global", false);
    static const TSSymbol s_static
        = util::symbol_for_name(lang_, "static", false);

    const TSNode n_name = util::child_by_field_name(node, "name");
    // util::print_child_nodes_types(node, src_code, true);

    if (ts_node_is_null(n_name))
    {
        const TSNode qualifier_node = ts_node_named_child(node, 0);
        // std::cout << ts_node_type(qualifier_node) << std::endl;
        // util::print_child_nodes_types(qualifier_node, src_code);
        const std::string namespace_str
            = util::extract_node_text(qualifier_node, get_src_str());
        Scope scope    = util::create_scope(namespace_str);
        bool is_global = false;
        bool is_static = false;
        for (uint32_t i = 0; i < ts_node_child_count(node); ++i)
        {
            const TSSymbol s_current = ts_node_symbol(ts_node_child(node, i));
            if (s_current == s_global)
                is_global = true;
            else if (s_current == s_static)
                is_static = true;
        }

        if (is_global && is_static)
        {
            const std::string type_name = std::move(scope.names_.back());
            scope.names_.pop_back();
            if (const auto type
                = symb_table_.symb_tree.find_type(scope, type_name))
            {
                symb_table_.glob_static_usings.push_back(type->def);
            }
        }
        else if (is_static)
        {
            const std::string type_name = std::move(scope.names_.back());
            scope.names_.pop_back();
            if (const auto type
                = symb_table_.symb_tree.find_type(scope, type_name))
            {
                SourceCode* src = get_src();
                src->file_context.static_usings.push_back(type->def);
            }
        }
        else if (is_global)
        {
            symb_table_.glob_usings.push_back(scope);
        }
        else
        {
            get_src()->file_context.usings.push_back(scope);
        }
    }
    else
    {
        // todo aliases not implemented
        // const TSNode qualif_name_node = ts_node_named_child(node, 1);
        // std::cout << ts_node_type(qualif_name_node) << std::endl;
        // const std::string alias_name = util::extract_node_text(name_node,
        // src_code); if (ts_node_symbol(qualif_name_node) ==
        // qualified_name_symb)
        // {
        //     Scope scope =
        //     mk_scope(util::extract_node_text(qualif_name_node));
        // } else
        // {
        //
        // }

        // std::stack<std::string> scope_stack;
        // util::print_child_nodes_types(qualif_name_node, src_code);
        // util::split_namespace(scope_stack, qualifier);

        // if (ClassDefStmt* class_def = stmt_factory_.get_class_def(, ))
    }
}

SourceCode* SymbolTableBuilder::get_src() const
{
    return current_src_
             ? current_src_
             : throw std::logic_error("Current source code is not set");
}

std::string_view SymbolTableBuilder::get_src_str() const
{
    return current_src_
             ? current_src_->file.content
             : throw std::logic_error("Current source code is not set");
}

} // namespace astfri::csharp
