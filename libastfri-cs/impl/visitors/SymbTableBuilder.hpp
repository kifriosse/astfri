#ifndef CSHARP_SYMBOL_TABLE_BUILDER_HPP
#define CSHARP_SYMBOL_TABLE_BUILDER_HPP

#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri-cs/impl/regs/QueryRegistry.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/visitors/TypeTranslator.hpp>
#include <libastfri-cs/inc/SDKProfile.hpp>

#include <tree_sitter/api.h>

#include <vector>

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
class SymbTableBuilder
{

private:
    friend regs::Handlers;

    static StmtFactory& stmtFact_;
    static regs::QueryReg& queryReg_;

    std::vector<std::unique_ptr<SourceFile>>& srcs_;
    TypeContext typeContext_;
    TypeTranslator typeTrs_;
    SymbolTable& symbTable_;
    SourceFile* currentSrc_{nullptr};
    ScopeNode* currentParent_{nullptr};
    const TSLanguage* lang_;

public:
    /**
     * @brief Constructs a SymbolTableBuilder
     * @param srcs reference to a vector of source codes. Vector's lifetime
     * must exceed lifetime of Symbol table builder
     * @param symbTable reference to symbol table that user wants to fill up.
     * Symbol table's lifetime must exceed lifetime of Symbol table builder
     */
    explicit SymbTableBuilder(
        std::vector<std::unique_ptr<SourceFile>>& srcs,
        SymbolTable& symbTable
    );

    void reg_user_types();
    void reg_using_directives();
    void collect_types(const TSTree* tree);
    void load_implicit_usings(SDKProfile profile);
    void reg_members();

private:
    static ScopeNode* visit_class(SymbTableBuilder* self, const TSNode& node);
    static ScopeNode* visit_interface(
        SymbTableBuilder* self,
        const TSNode& node
    );
    static ScopeNode* visit_record(SymbTableBuilder* self, const TSNode& node);
    static ScopeNode* visit_enum(SymbTableBuilder* self, const TSNode& node);
    static ScopeNode* visit_delegate(
        SymbTableBuilder* self,
        const TSNode& node
    );
    static void visit_memb_var(SymbTableBuilder* self, const TSNode& node);
    static void visit_property(SymbTableBuilder* self, const TSNode& node);
    static void visit_method(SymbTableBuilder* self, const TSNode& node);

    void reg_using_directive(const TSNode& nUsingDirective);
    ScopeNode* visit_type_def(const TSNode& node, util::TypeKind typeKind);
    [[nodiscard]] SourceFile* src() const;
    [[nodiscard]] std::string_view src_str() const;
};

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_BUILDER_HPP