#ifndef CSHARP_SYMBOL_TABLE_BUILDER_HPP
#define CSHARP_SYMBOL_TABLE_BUILDER_HPP

#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri-cs/impl/regs/QueryRegistry.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/visitors/TypeTranslator.hpp>
#include <libastfri-cs/inc/SDKProfile.hpp>

#include <tree_sitter/api.h>

#include <vector>

namespace astfri {
// Forward declarations
class StmtFactory;

} // namespace astfri

namespace astfri::csharp {

/**
 * @brief Scan C# source files and builds a symbol table from them
 * @note This class stores a reference to vector of SourceCode structs and
 * reference to symbol table.
 * Caller has to ensure that both objects live longer than SymbolTableBuilder
 */
class SymbTableBuilder {

private:
    friend maps::Mappers;

    static StmtFactory& stmtFact_;
    static maps::QueryReg& queryReg_;

    TypeContext typeContext_;
    TypeTranslator typeTrs_;
    SymbolTable& symbTable_;
    std::vector<std::unique_ptr<SourceFile>>& srcs_;
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

    /**
     * @brief Registers all user defined types in loaded source files into
     * symbol table.
     */
    void reg_user_types();
    /**
     * @brief Registers all using directives in loaded source files into symbol
     * table.
     */
    void reg_using_directives();
    /**
     * @brief Loads global implicit using directives for the given SDK profile
     * into symbol table.
     * @param profile profile for which to load implicit usings
     */
    void load_implicit_usings(SDKProfile profile);
    /**
     * @brief Registers members of user defined types in loaded source file
     * into symbol table
     */
    void load_external_types(std::filesystem::path& jsonPath);
    /**
     * @brief Register all members of registered types in current project
     */
    void reg_members();

private:
    static ScopeNode* visit_class(SymbTableBuilder* self, const TSNode& node);
    static ScopeNode* visit_interface(SymbTableBuilder* self, const TSNode& node);
    static ScopeNode* visit_record(SymbTableBuilder* self, const TSNode& node);
    static ScopeNode* visit_enum(SymbTableBuilder* self, const TSNode& node);
    static ScopeNode* visit_delegate(SymbTableBuilder* self, const TSNode& node);
    static void visit_memb_var(SymbTableBuilder* self, const TSNode& node);
    static void visit_property(SymbTableBuilder* self, const TSNode& node);
    static void visit_method(SymbTableBuilder* self, const TSNode& node);
    static void visit_base_list(SymbTableBuilder* self, const TSNode& node);
    static void visit_type_param_constraint(SymbTableBuilder* self, const TSNode& node);

    /**
     * @brief Registers a single using directive.
     * @param nUsingDirective TSNode representing the using directive to
     * register
     */
    void reg_using_directive(const TSNode& nUsingDirective);
    /**
     * Collects/Registers types defined in the given tree-sitter tree into
     * symbol table.
     * @param tree tree-sitter tree to collect types from
     */
    void collect_types(const TSTree* tree);
    /**
     * @brief General visit function for registering user defined types.
     * @param node TSNode representing the type definition
     * @param type kind of the type definition (class, interface, struct,
     * enum or delegate)
     * @return pointer to the ScopeNode created for the that Type
     */
    ScopeNode* visit_type_def(const TSNode& node, util::TypeKind type);
    void visit_base_list_class(const TSNode& node, ClassDefStmt* classDef);
    void visit_base_list_interface(const TSNode& node, InterfaceDefStmt* intfDef);

    /**
     * @brief Gets the source code currently being visited.
     * @return string view of current source code
     * @throws std::logic_error if source file is not set
     */
    [[nodiscard]] std::string_view src_str() const;
    /**
     * @brief Gets the source file currently being visited.
     * @return pointer to current source file
     * @throws std::logic_error if source file is not set
     */
    [[nodiscard]] SourceFile* src() const;

    /**
     * @brief Makes type binding of defined type kind
     * @param type type of user defined type
     * @param scope scope of the type
     * @param name name of the type
     * @param genericParams
     * @param genericParams
     * @return type binding for that type
     */
    [[nodiscard]] static TypeBinding mk_type_binding(
        util::TypeKind type,
        Scope scope,
        std::string name,
        std::vector<GenericParam*> genericParams
    );
};

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_BUILDER_HPP