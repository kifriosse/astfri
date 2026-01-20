#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
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

StmtFactory& SymbolTableBuilder::stmt_f_       = StmtFactory::get_instance();
regs::QueryReg& SymbolTableBuilder::query_reg_ = regs::QueryReg::get();

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
    auto process = [this](const TSQueryMatch& match)
    {
        const TSNode node        = match.captures[0].node;
        const RegHandler handler = RegManager::get_reg_handler(node);
        handler(this, node);
    };

    for (auto& src : srcs_)
    {
        auto& [context, file, tree] = src;
        if (! tree)
            continue;

        current_src_ = &src;
        type_tr_.set_current_src(&src);
        util::for_each_match(
            ts_tree_root_node(tree),
            regs::QueryType::TopLevel,
            process
        );
    }
    current_src_ = nullptr;
}

void SymbolTableBuilder::reg_using_directives()
{
    auto process = [this](const TSQueryMatch match) -> void
    { add_using_directive(match.captures[0].node); };
    for (auto& src : srcs_)
    {
        type_tr_.set_current_src(&src);
        current_src_      = &src;
        const TSNode root = ts_tree_root_node(src.tree);
        util::for_each_match(root, regs::QueryType::Using, process);
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
            const TSNode n_class_body = util::child_by_field_name(node, "body");
            TSTreeCursor cursor       = ts_tree_cursor_new(n_class_body);
            type_context_.type_stack.push(metadata.user_type);

            if (! ts_tree_cursor_goto_first_child(&cursor))
                continue;

            do
            {
                TSNode n_current = ts_tree_cursor_current_node(&cursor);
                if (! ts_node_is_named(n_current))
                    continue;

                RegHandler handler = RegManager::get_reg_handler(n_current);
                handler(this, n_current);
            } while (ts_tree_cursor_goto_next_sibling(&cursor));

            type_context_.type_stack.pop();
            ts_tree_cursor_delete(&cursor);
        }
    }
}

void SymbolTableBuilder::visit_class(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    const std::string_view src = self->src_str();
    const TSNode n_name        = util::child_by_field_name(node, "name");
    // todo handle generic parameters
    Scope scope             = util::create_scope(node, src);
    ClassDefStmt* class_def = stmt_f_.mk_class_def(
        util::extract_text(n_name, src),
        std::move(scope)
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

void SymbolTableBuilder::visit_interface(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_record(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_enum(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_delegate(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_memb_var(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    using namespace regs;
    static const Query* query      = query_reg_.get_query(QueryType::VarDecl);
    static const uint32_t decl_id  = query->id("decl");
    static const uint32_t modif_id = query->id("modifier");

    const std::string_view src     = self->src_str();

    CSModifiers modifiers;
    TSNode n_var_decl;
    auto process_var = [&](const TSQueryMatch& match)
    {
        for (uint32_t i = 0; i < match.capture_count; ++i)
        {
            auto [n_current, index] = match.captures[i];
            if (index == decl_id)
            {
                n_var_decl = n_current;
            }
            else if (index == modif_id)
            {
                const CSModifier mod = RegManager::get_modifier(n_current, src);
                modifiers.add_modifier(mod);
            }
        }
    };
    util::for_each_match(node, QueryType::VarDecl, process_var);

    const TSNode n_type     = util::child_by_field_name(n_var_decl, "type");
    const TypeHandler th    = RegManager::get_type_handler(n_type);
    Type* type              = th(&self->type_tr_, n_type);
    TypeMetadata& type_meta = self->symb_table_.user_types_metadata.at(
        self->type_context_.type_stack.top()
    );

    std::vector<VarDefStmt*> var_defs;
    auto process_decl = [&](const TSQueryMatch& match)
    {
        const TSNode n_decltor = match.captures[0].node;
        const TSNode n_name    = util::child_by_field_name(n_decltor, "name");
        MemberVarDefStmt* var_def = stmt_f_.mk_member_var_def(
            util::extract_text(n_name, src),
            type,
            nullptr,
            modifiers.get_access_mod().value_or(AccessModifier::Private)
        );

        var_defs.push_back(var_def);
        MemberVarMetadata member_var_meta{
            .var_def     = var_def,
            .var_node    = n_decltor,
            .initializer = ts_node_named_child(n_decltor, 1) // right side
        };
        type_meta.member_vars.emplace(var_def->name_, member_var_meta);
    };
    util::for_each_match(n_var_decl, QueryType::VarDecltor, process_decl);
}

void SymbolTableBuilder::visit_property(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node
)
{
}

void SymbolTableBuilder::visit_method(
    SymbolTableBuilder* self,
    const TSNode& node
)
{
    const auto current_type = self->type_context_.type_stack.top();
    if (! current_type)
        throw std::logic_error("Owner type not found");

    auto& symb_table        = self->symb_table_;
    const auto it_type_meta = symb_table.user_types_metadata.find(current_type);
    if (! symb_table.user_types_metadata.contains(current_type))
        throw std::logic_error("Type wasn't discovered yet");

    const CSModifiers modifs
        = CSModifiers::handle_modifs_memb(node, self->src_str());

    const TSNode n_ret_type    = util::child_by_field_name(node, "returns");
    const TSNode n_func_name   = util::child_by_field_name(node, "name");
    const TSNode n_params      = util::child_by_field_name(node, "parameters");

    const bool is_variadic     = util::has_variadic_param(n_params);
    const size_t named_child_c = ts_node_named_child_count(n_params);
    const size_t param_c = is_variadic ? named_child_c - 1 : named_child_c;
    std::string name     = util::extract_text(n_func_name, self->src_str());
    MethodId method_id{
        .name        = name,
        .param_count = param_c,
        .is_static   = modifs.has(CSModifier::Static),
    };

    auto [params, params_meta]
        = util::discover_params(n_params, self->src_str(), self->type_tr_);
    MethodDefStmt* method_def = nullptr;
    auto& methods             = it_type_meta->second.methods;
    const auto& [it_method, inserted]
        = methods.try_emplace(std::move(method_id));

    if (inserted)
    {
        const TypeHandler th = RegManager::get_type_handler(n_ret_type);
        Type* ret_type       = th(&self->type_tr_, n_ret_type);
        method_def           = stmt_f_.mk_method_def(
            current_type,
            stmt_f_.mk_function_def(
                std::move(name),
                std::move(params),
                ret_type,
                nullptr
            ),
            modifs.get_access_mod().value_or(AccessModifier::Internal),
            modifs.get_virtuality()
        );
    }

    MethodMetadata metadata{
        .params      = std::move(params_meta),
        .method_def  = method_def,
        .method_node = node,
    };
    it_method->second = std::move(metadata);
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
        const TSNode n_qualif = ts_node_named_child(node, 0);
        // std::cout << ts_node_type(qualifier_node) << std::endl;
        // util::print_child_nodes_types(qualifier_node, src_code);
        const std::string nms_str = util::extract_text(n_qualif, src_str());
        Scope scope               = util::create_scope(nms_str);
        bool is_global            = false;
        bool is_static            = false;
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
                src()->file_context.static_usings.push_back(type->def);
            }
        }
        else if (is_global)
        {
            symb_table_.glob_usings.push_back(std::move(scope));
        }
        else
        {
            src()->file_context.usings.push_back(std::move(scope));
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

SourceCode* SymbolTableBuilder::src() const
{
    return current_src_
             ? current_src_
             : throw std::logic_error("Current source code is not set");
}

std::string_view SymbolTableBuilder::src_str() const
{
    return src()->file.content;
}

} // namespace astfri::csharp
