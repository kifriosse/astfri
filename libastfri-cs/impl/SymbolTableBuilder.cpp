#include <libastfri-cs/impl/data/CSModifiers.hpp>
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

SymbolTableBuilder::SymbolTableBuilder(std::vector<SourceCode>& source_codes) :
    src_codes(source_codes),
    lang_(tree_sitter_c_sharp())
{
}

void SymbolTableBuilder::register_user_types(SymbolTable& symbol_table)
{
    for (auto& src : src_codes)
    {
        auto& [context, file, tree] = src;
        if (! tree)
            continue;

        current_src                               = &src;
        const std::vector<TSNode> top_level_nodes = util::find_nodes(
            ts_tree_root_node(tree),
            ts_tree_language(tree),
            regs::Queries::top_level_stmt_query
        );
        for (const TSNode& node : top_level_nodes)
        {
            const RegHandler handler = RegManager::get_reg_handler(node);
            handler(this, node, symbol_table);
        }
    }
    current_src = nullptr;
}

void SymbolTableBuilder::register_members(SymbolTable& symbol_table)
{
    for (auto& metadata : symbol_table.user_types_metadata | std::views::values)
    {
        for (auto& [node, src] : metadata.defs)
        {
            current_src             = src;
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
                handler(this, current, symbol_table);
            } while (ts_tree_cursor_goto_next_sibling(&cursor));

            type_context_.type_stack.pop();
            ts_tree_cursor_delete(&cursor);
        }
    }
}

void SymbolTableBuilder::load_using_directives()
{
    for (auto& src : src_codes)
    {
        FileContext& file_context      = src.file_context;
        TSNode root                    = ts_tree_root_node(src.tree);
        std::vector<TSNode> directives = util::find_nodes(
            root,
            lang_,
            regs::Queries::using_directives_query
        );
        for (auto directive : directives)
        {
            file_context
                .add_using_directive(directive, lang_, src.file.content);
        }
    }
}

void SymbolTableBuilder::register_class(
    SymbolTableBuilder* self,
    const TSNode& node,
    SymbolTable& type_table
)
{
    if (! self->current_src)
        throw std::logic_error("Current source code is not set");

    const std::string& source_code_ = self->current_src->file.content;
    const TSNode name_node          = util::child_by_field_name(node, "name");
    // todo handle generic parameters
    ClassDefStmt* class_def = stmt_factory_.mk_class_def(
        util::extract_node_text(name_node, source_code_),
        util::create_scope(node, self->lang_, source_code_)
    );

    auto it_metadata = type_table.user_types_metadata.find(class_def);
    // partial class handling
    if (it_metadata == type_table.user_types_metadata.end())
    {
        type_table.user_type_keys.push_back(class_def);
        type_table.user_types_metadata.emplace(
            class_def,
            TypeMetadata{
                .user_type = class_def,
                .defs      = {{node, self->current_src}}
            }
        );
    }
    else
    {
        it_metadata->second.defs.emplace_back(node, self->current_src);
    }
}

void SymbolTableBuilder::register_interface(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node,
    [[maybe_unused]] SymbolTable& type_table
)
{
}

void SymbolTableBuilder::register_record(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node,
    [[maybe_unused]] SymbolTable& type_table
)
{
}

void SymbolTableBuilder::register_enum(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node,
    [[maybe_unused]] SymbolTable& type_table
)
{
}

void SymbolTableBuilder::register_delegate(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node,
    [[maybe_unused]] SymbolTable& type_table
)
{
}

void SymbolTableBuilder::register_memb_var(
    SymbolTableBuilder* self,
    const TSNode& node,
    SymbolTable& type_table
)
{
    const std::string& source_code = self->current_src->file.content;
    const std::vector<TSNode> modifier_nodes
        = util::find_nodes(node, self->lang_, regs::Queries::var_modif_query);
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, source_code);

    const TSNode var_decl_node
        = modifier_nodes.empty() ? ts_node_child(node, 0)
                                 : ts_node_next_sibling(modifier_nodes.back());

    const TSNode type_node = util::child_by_field_name(var_decl_node, "type");
    Type* type             = util::make_type(type_node, source_code);

    const std::vector<TSNode> var_decltor_nodes = util::find_nodes(
        var_decl_node,
        self->lang_,
        regs::Queries::decl_query
    );

    TypeMetadata& type_metadata = type_table.user_types_metadata.at(
        self->type_context_.type_stack.top()
    );

    std::vector<VarDefStmt*> var_def_stmts;
    for (const TSNode& var_decltor_node : var_decltor_nodes)
    {
        TSNode var_name_node = ts_node_child(var_decltor_node, 0); // left side
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
            .var_def     = var_def,
            .var_node    = var_decltor_node,
            .initializer = ts_node_child(var_decltor_node, 2) // right side
        };
        type_metadata.member_vars.emplace(var_name, member_var_metadata);
    }
}

void SymbolTableBuilder::register_property(
    [[maybe_unused]] SymbolTableBuilder* self,
    [[maybe_unused]] const TSNode& node,
    [[maybe_unused]] SymbolTable& type_table
)
{
}

void SymbolTableBuilder::register_method(
    SymbolTableBuilder* self,
    const TSNode& node,
    SymbolTable& type_table
)
{
    const auto current_type = self->type_context_.type_stack.top();
    if (! current_type)
        throw std::logic_error("Owner type not found");

    const auto it_type_metadata
        = type_table.user_types_metadata.find(current_type);
    if (! type_table.user_types_metadata.contains(current_type))
        throw std::logic_error("Type wasn't discovered yet");

    const std::vector<TSNode> modif_nodes = util::find_nodes(
        node,
        self->lang_,
        regs::Queries::method_modif_query
    );
    const CSModifiers method_modif
        = CSModifiers::handle_modifiers(modif_nodes, self->get_src());
    const TSNode ret_type_node  = util::child_by_field_name(node, "returns");
    const TSNode func_name_node = util::child_by_field_name(node, "name");
    const TSNode params_node    = util::child_by_field_name(node, "parameters");
    Type* return_type      = util::make_type(ret_type_node, self->get_src());

    const bool is_variadic = util::has_variadic_param(params_node, nullptr);
    const size_t named_child_count = ts_node_named_child_count(params_node);
    const size_t param_count
        = is_variadic ? named_child_count - 1 : named_child_count;
    const std::string name
        = util::extract_node_text(func_name_node, self->get_src());
    MethodId method_id{
        .func_id   = {name, param_count},
        .is_static = method_modif.has(CSModifier::Static),
    };
    // util::print_child_nodes_types(param_list_node);

    auto [params, params_metadata]
        = util::discover_params(params_node, self->get_src());
    auto& methods                  = it_type_metadata->second.methods;
    const auto it_method           = methods.find(method_id);
    MethodDefStmt* method_def_stmt = nullptr;
    if (it_method == methods.end())
    {
        method_def_stmt = stmt_factory_.mk_method_def(
            current_type,
            stmt_factory_.mk_function_def(name, params, return_type, nullptr),
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

std::string_view SymbolTableBuilder::get_src() const
{
    if (! current_src)
        throw std::logic_error("Current source code is not set");
    return current_src->file.content;
}

const TSLanguage* SymbolTableBuilder::get_lang() const
{
    if (! lang_)
        throw std::logic_error("Language is not set");
    return lang_;
}
} // namespace astfri::csharp
