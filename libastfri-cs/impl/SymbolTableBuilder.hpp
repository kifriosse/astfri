#ifndef CSHARP_SYMBOL_TABLE_BUILDER_HPP
#define CSHARP_SYMBOL_TABLE_BUILDER_HPP

#include "SemanticContext.hpp"

namespace astfri::csharp
{

class SymbolTableBuilder
{
public:
    using RegHandler = std::function<void(const TSNode&, SemanticContext&)>;

private:
    SemanticContext& context_;

public:
    explicit SymbolTableBuilder(SemanticContext& context);
    void register_symbols(const TSTree* source_tree) const;

    static void register_class(const TSNode& node, SemanticContext& context);
    static void register_struct(const TSNode& node, SemanticContext& context);
    static void register_interface(
        const TSNode& node,
        SemanticContext& context
    );
    static void register_record(const TSNode& node, SemanticContext& context);
    static void register_enum(const TSNode& node, SemanticContext& context);
    static void register_delegate(const TSNode& node, SemanticContext& context);

    static void register_field(const TSNode& node, SemanticContext& context);
    static void register_property(const TSNode& node, SemanticContext& context);
    static void register_method(const TSNode& node, SemanticContext& context);
};

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_BUILDER_HPP