
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/Source.hpp>
#include <libastfri-cs/impl/SymbolTableBuilder.hpp>
#include <libastfri-cs/impl/utils.hpp>

#include <tree_sitter/tree-sitter-c-sharp.h>

#include <cstring>
#include <ranges>
#include <utility>

namespace astfri::csharp
{

StmtFactory& SymbolTableBuilder::stmt_factory_ = StmtFactory::get_instance();

SymbolTableBuilder::SymbolTableBuilder(std::vector<SourceCode>& source_codes) :
    src_codes(source_codes),
    language_(tree_sitter_c_sharp())
{
}

void SymbolTableBuilder::register_user_types(SymbolTable& symbol_table)
{
    // todo add loading of using directives
    for (auto& src : src_codes)
    {
        auto& [file, tree] = src;
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

void SymbolTableBuilder::register_class(
    SymbolTableBuilder* self,
    const TSNode& node,
    SymbolTable& type_table
)
{
    if (! self->current_src)
        throw std::logic_error("Current source code is not set");

    const std::string& source_code_ = self->current_src->file.content;
    const TSNode name_node = ts_node_child_by_field_name(node, "name", 4);
    // todo handle generic parameters
    ClassDefStmt* class_def = stmt_factory_.mk_class_def(
        util::extract_node_text(name_node, source_code_),
        util::create_scope(node, self->language_, source_code_)
    );

    // todo handle partial classes
    type_table.user_type_keys.push_back(class_def);
    type_table.user_types_metadata.emplace(
        class_def,
        TypeMetadata{
            .user_type = class_def,
            .defs      = {{node, self->current_src}}
        }
    );
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
    const std::string& source_code           = self->current_src->file.content;
    const std::vector<TSNode> modifier_nodes = util::find_nodes(
        node,
        self->language_,
        regs::Queries::var_modif_query
    );
    const CSModifiers modifiers
        = CSModifiers::handle_modifiers(modifier_nodes, source_code);

    const TSNode var_decl_node
        = modifier_nodes.empty() ? ts_node_child(node, 0)
                                 : ts_node_next_sibling(modifier_nodes.back());

    const TSNode type_node
        = ts_node_child_by_field_name(var_decl_node, "type", 4);
    Type* type = util::make_type(type_node, source_code);

    const std::vector<TSNode> var_decltor_nodes = util::find_nodes(
        var_decl_node,
        self->language_,
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

    const std::string& source_code           = self->current_src->file.content;
    const std::vector<TSNode> modifier_nodes = util::find_nodes(
        node,
        self->language_,
        regs::Queries::method_modif_query
    );
    const CSModifiers method_modif
        = CSModifiers::handle_modifiers(modifier_nodes, source_code);
    const TSNode return_type_node
        = ts_node_child_by_field_name(node, "returns", 7);
    const TSNode func_name_node = ts_node_child_by_field_name(node, "name", 4);
    const TSNode param_list_node
        = ts_node_child_by_field_name(node, "parameters", 10);
    TSTreeCursor param_cursor = ts_tree_cursor_new(param_list_node);
    std::vector<ParamVarDefStmt*> params;
    std::vector<ParamMetadata> param_metadata;
    Type* return_type = util::make_type(return_type_node, source_code);

    const auto it_type_metadata
        = type_table.user_types_metadata.find(current_type);
    if (! type_table.user_types_metadata.contains(current_type))
        throw std::logic_error("Type wasn't discovered yet");

    // util::print_child_nodes_types(param_list_node);

    if (ts_tree_cursor_goto_first_child(&param_cursor))
    {
        static const TSSymbol param_keyword = ts_language_symbol_for_name(
            self->language_,
            "params",
            std::strlen("params"),
            false
        );
        bool is_variadic = false;
        do
        {
            const TSNode current = ts_tree_cursor_current_node(&param_cursor);
            if (! ts_node_is_named(current))
            {
                is_variadic = ts_node_symbol(current) == param_keyword;
                if (is_variadic)
                    break;

                continue;
            }

            CSModifiers param_mod = CSModifiers::handle_modifiers(
                util::find_nodes(
                    current,
                    self->language_,
                    regs::Queries::var_modif_query
                ),
                source_code
            );

            const TSNode type_node
                = ts_node_child_by_field_name(current, "type", 4);
            const TSNode param_name_node
                = ts_node_child_by_field_name(current, "name", 4);
            Type* param_type = util::make_type(type_node, source_code);
            TypeFactory& type_factory = TypeFactory::get_instance();
            if (param_mod.has(CSModifier::Out) || param_mod.has(CSModifier::Ref))
            {
                param_type = type_factory.mk_indirect(param_type);
            }
            else if (param_mod.has(CSModifier::In) ||
                (param_mod.has(CSModifier::Readonly) && !param_mod.has(CSModifier::Ref)))
            {
                // todo should be a constat reference
                // for now, just make it indirect
                param_type = type_factory.mk_indirect(param_type);
            }

            ParamVarDefStmt* param = stmt_factory_.mk_param_var_def(
                util::extract_node_text(param_name_node, source_code),
                param_type,
                nullptr
            );
            params.push_back(param);
            param_metadata.emplace_back(
                ParamMetadata{
                    .param_def  = param,
                    .param_node = current,
                    .processed  = false
                }
            );
        } while (ts_tree_cursor_goto_next_sibling(&param_cursor));

        if (is_variadic && params.empty())
        {
            // todo redo this when we add variadic parameters
            // temporary solution
            TSNode type_node
                = ts_node_child_by_field_name(param_list_node, "type", 4);
            TSNode name_node
                = ts_node_child_by_field_name(param_list_node, "name", 4);
            ParamVarDefStmt* param = stmt_factory_.mk_param_var_def(
                util::extract_node_text(name_node, source_code),
                util::make_type(type_node, source_code),
                nullptr
            );
            params.emplace_back(param);
            param_metadata.emplace_back(
                ParamMetadata{
                    .param_def  = param,
                    .param_node = param_list_node,
                    .processed  = false
                }
            );
        }
    }

    ts_tree_cursor_delete(&param_cursor);

    const std::string name
        = util::extract_node_text(func_name_node, source_code);
    MethodIdentifier method_id{
        .func_id
        = FunctionIdentifier{.name = name, .param_count = params.size()},
        .is_static = method_modif.has(CSModifier::Static),
    };

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
        .params      = std::move(param_metadata),
        .method_def  = method_def_stmt,
        .method_node = node,
        .processed   = false,
    };

    it_type_metadata->second.methods.emplace(method_id, method_metadata);
}
} // namespace astfri::csharp
