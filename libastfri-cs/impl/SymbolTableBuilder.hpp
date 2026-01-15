#ifndef CSHARP_SYMBOL_TABLE_BUILDER_HPP
#define CSHARP_SYMBOL_TABLE_BUILDER_HPP

#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/TypeTranslator.hpp>

namespace astfri
{
// Forward declarations
class StmtFactory;

} // namespace astfri

namespace astfri::csharp
{

/**
 * @brief Scan C# source files and builds a symbol table from them
 * @note This class stores a reference to vector of SourceCode structs and
 * reference to symbol table.
 * Caller has to ensure that both objects live longer than SymbolTableBuilder
 */
class SymbolTableBuilder
{

private:
    static StmtFactory& stmt_factory_;

    TypeContext type_context_;
    TypeTranslator type_tr_;
    std::vector<SourceCode>& srcs_;
    SymbolTable& symb_table_;
    SourceCode* current_src_{nullptr};
    const TSLanguage* lang_;

public:
    /**
     * @brief Constructs a SymbolTableBuilder
     * @param srcs reference to a vector of source codes. Vector's lifetime
     * must exceed lifetime of Symbol table builder
     * @param symb_table reference to symbol table that user wants to fill up.
     * Symbol table's lifetime must exceed lifetime of Symbol table builder
     */
    explicit SymbolTableBuilder(
        std::vector<SourceCode>& srcs,
        SymbolTable& symb_table
    );

    void reg_user_types();
    void reg_using_directives();
    void reg_members();

    static void reg_class(SymbolTableBuilder* self, const TSNode& node);
    static void reg_interface(SymbolTableBuilder* self, const TSNode& node);
    static void reg_record(SymbolTableBuilder* self, const TSNode& node);
    static void reg_enum(SymbolTableBuilder* self, const TSNode& node);
    static void reg_delegate(SymbolTableBuilder* self, const TSNode& node);
    static void reg_memb_var(SymbolTableBuilder* self, const TSNode& node);
    static void reg_property(SymbolTableBuilder* self, const TSNode& node);
    static void reg_method(SymbolTableBuilder* self, const TSNode& node);

private:
    void add_using_directive(const TSNode& node);
    [[nodiscard]] SourceCode* get_src() const;
    [[nodiscard]] std::string_view get_src_str() const;
};

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_BUILDER_HPP