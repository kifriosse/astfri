#ifndef CSHARP_SYMBOL_TABLE_BUILDER_HPP
#define CSHARP_SYMBOL_TABLE_BUILDER_HPP

#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/data/Source.hpp>

namespace astfri::csharp
{

/**
 * @brief Scan C# source files and builds a symbol table from them
 * @note This class stores a reference to vector of SourceCode structs.
 * Caller has to ensure that the vector's lifetime exceeds or is the same as the
 * lifetime of SymbolTableBuilder
 */
class SymbolTableBuilder
{
public:
    using RegHandler = std::function<void(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    )>;

private:
    static StmtFactory& stmt_factory_;

    TypeContext type_context_;
    std::vector<SourceCode>& src_codes;
    SourceCode* current_src{nullptr};
    const TSLanguage* lang_;

public:
    /**
     * @brief Constructs a SymbolTableBuilder
     * @param source_codes A reference to a vector of source codes,
     * vector's lifetime must exceed or be same as lifetime of
     * SymbolTableBuilder
     */
    explicit SymbolTableBuilder(std::vector<SourceCode>& source_codes);

    void register_user_types(SymbolTable& symbol_table);
    void register_members(SymbolTable& symbol_table);

    static void register_class(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    );
    static void register_interface(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    );
    static void register_record(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    );
    static void register_enum(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    );
    static void register_delegate(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    );
    static void register_memb_var(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    );
    static void register_property(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    );
    static void register_method(
        SymbolTableBuilder* self,
        const TSNode& node,
        SymbolTable& type_table
    );

private:
    [[nodiscard]] std::string_view get_src() const;
    [[nodiscard]] const TSLanguage* get_lang() const;
};

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_BUILDER_HPP