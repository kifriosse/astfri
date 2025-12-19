
#include <libastfri-cs/impl/SymbolTableBuilder.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/utils.hpp>

namespace astfri::csharp
{

SymbolTableBuilder::SymbolTableBuilder(SemanticContext& context) :
    context_(context)
{
}

void SymbolTableBuilder::register_symbols(const TSTree* source_tree) const
{
    // todo add loading of using directives
    const TSNode root                         = ts_tree_root_node(source_tree);
    const std::vector<TSNode> top_level_nodes = find_nodes(
        root,
        ts_tree_language(source_tree),
        regs::top_level_stmt_query
    );
    for (const TSNode& node : top_level_nodes)
    {
        const RegHandler handler = RegManager::get_reg_handler(node);
        handler(node, context_);
    }
}

void SymbolTableBuilder::register_class(
    const TSNode& node,
    SemanticContext& context
)
{
}

void SymbolTableBuilder::register_struct(
    const TSNode& node,
    SemanticContext& context
)
{
}

void SymbolTableBuilder::register_interface(
    const TSNode& node,
    SemanticContext& context
)
{
}

void SymbolTableBuilder::register_record(
    const TSNode& node,
    SemanticContext& context
)
{
}

void SymbolTableBuilder::register_enum(
    const TSNode& node,
    SemanticContext& context
)
{
}

void SymbolTableBuilder::register_delegate(
    const TSNode& node,
    SemanticContext& context
)
{
}

void SymbolTableBuilder::register_field(
    const TSNode& node,
    SemanticContext& context
)
{
}

void SymbolTableBuilder::register_property(
    const TSNode& node,
    SemanticContext& context
)
{
}

void SymbolTableBuilder::register_method(
    const TSNode& node,
    SemanticContext& context
)
{
}
} // namespace astfri::csharp