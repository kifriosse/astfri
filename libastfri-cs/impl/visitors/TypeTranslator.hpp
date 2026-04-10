#ifndef CSHARP_TYPE_TRANSLATOR
#define CSHARP_TYPE_TRANSLATOR

#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <astfri/Astfri.hpp>

#include <tree_sitter/api.h>

namespace astfri::csharp {
struct SourceFile;

/**
 * @brief Class responsible for translating tree-sitter type nodes to
 * AST FRI Types. Also responsible for resolving FQN and PQN for both types and
 * namespaces.
 * @note This class contains reference to symbol table. User needs to ensure
 * that symbol table lives longer than TypeTranslator instance.
 */
class TypeTranslator {
private:
    friend maps::Mappers;

    static TypeFactory& typeFact_;
    /**
     * @brief Node used for marking types that can't be resolved. It's used as
     * a temporary storage for FQN or PQN type or namespace names that can't be
     * resolved
     */
    static ScopeNode extMarkNode_;

    SymbolTable& symbTable_;
    ScopeNode* currentScope_{nullptr};
    SourceFile* currentSrc_{nullptr};
    const TSLanguage* lang_;

public:
    explicit TypeTranslator(SymbolTable& symbTable);
    /**
     * @brief Sets the current source file for the type translator.
     * @param src pointer to the current source file being translated.
     */
    void set_current_src(SourceFile* src);
    /**
     * @brief Sets the current namespace for the type translator.
     * @param node pointer to the ScopeNode representing the current namespace
     * where the translator is currently located.
     */
    void set_current_namespace(ScopeNode* node);

    /**
     * @brief Tries to resolve given type name in given scope.
     * @param name name of the type to resolve. Should be just name of the type
     * without namespace qualifiers and nested type qualifiers
     * @param scope scope in which to look for.
     * @return pointer to TypeBinding if type is found, otherwise nullptr.
     */
    [[nodiscard]] TypeBinding* get_type(std::string_view name, const Scope& scope) const;
    /**
     * @brief Resolves given node representing qualified name to ScopeNode
     * representing type or namespace
     * @param nQualif node representing qualified name to resolve
     * @param searchScope defines search scope
     * @param start scope node that represents the starting point of the
     * search - where the qualified name is located.
     * @return scope node representing type or namespace. If qualified name
     * can't be resolved, it will return nullptr.
     */
    ScopeNode* resolve_qualif_name(
        const TSNode& nQualif,
        util::SearchScope searchScope,
        ScopeNode* start
    ) const;
    /**
     * @brief Finds entry point for first qualifier in given FQN or PQN.
     * @param qualif first qualifier in FQN or PQN. (except for explicit
     * aliases)
     * @param searchScope defines search scope
     * @param start scope node that represents the starting point of the
     * search - where the qualified name is located.
     * @param src pointer to source file where the qualified name is located.
     * For certain search scopes, source can be nullptr:
     * - file scoped using directives
     * - all global using directives (normal, static and aliases)
     * @return starting point scope node for resolvinf FQN or PQN. If entry
     * point can't be found, it will return nullptr.
     */
    [[nodiscard]] ScopeNode* find_entry_point(
        std::string_view qualif,
        util::SearchScope searchScope,
        ScopeNode* start,
        SourceFile* src
    ) const;

private:
    static Type* visit_predefined(TypeTranslator* self, const TSNode& node);
    static Type* visit_identitifier(TypeTranslator* self, const TSNode& node);
    static Type* visit_qualified_name(TypeTranslator* self, const TSNode& node);
    /**
     * Used for \code var\endcode keyword
     * @param self reference to instance of TypeTranslator.
     * @param node TSNode that is being visited.
     * @return Deduced type.
     */
    static Type* visit_implicit(TypeTranslator* self, const TSNode& node);
    /**
     * Used for nodes that just wrap another type node - nullable types and
     * scoped types
     * @param self reference to instance of TypeTranslator
     * @param node TSNode that is being visited
     * @return type inside the wrapper
     */
    static Type* visit_wrapper(TypeTranslator* self, const TSNode& node);
    static Type* visit_indirect(TypeTranslator* self, const TSNode& node);
    static Type* visit_array(TypeTranslator* self, const TSNode& node);
    static Type* visit_generic_name(TypeTranslator* self, const TSNode& node);
    static Type* visit_tuple(TypeTranslator* self, const TSNode& node);
    static Type* visit_func_pointer(TypeTranslator* self, const TSNode& node);

    /**
     * Resolves alias qualifier node - alias qualifier:
     * \code Alias::Namespace.OtherNamespace.Type\endcode
     * @param aliasName alias qualifier node
     * @param searchScope search scope for looking up alias.
     * @param start node representing scope where this alias is being resolved.
     * @param src source file where this alias is being resolved. Include even
     * when resolving global using directives which are not file scoped.
     * @return resolved alias. If alias can't be resolved it will return
     * \c nullptr
     * @note if alias qualifier is \code global\endcode it will also return
     * \c nullptr
     */
    [[nodiscard]] const Alias* resolve_explicit_alias(
        std::string_view aliasName,
        util::SearchScope searchScope,
        ScopeNode* start,
        SourceFile* src
    ) const;
    /**
     * @brief Performs bottom-up search for given qualifier starting from the
     * given scope node.
     * @param qualif qualifier to search for.
     * @param start starting scope node from which to start the search.
     * @param src source file where the search is being performed.
     * @return scope node representing type or namespace with given qualifier.
     */
    [[nodiscard]] ScopeNode* bottom_up_search(
        std::string_view qualif,
        ScopeNode* start,
        SourceFile* src
    ) const;
    /**
     * @brief Searches for direct nested types in parent classes with given
     * qualifier.
     * @param qualif qualifier to search for.
     * @param start type representing point from which to start the search
     * @return scope node representing type with given qualifier. If cann't find
     * type with given qualifier, returns nullptr.
     * @note parent class in this context mean base classes not the nesting
     * class.
     */
    [[nodiscard]] ScopeNode* search_parents(
        std::string_view qualif,
        const TypeBinding& start
    ) const;

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
};

} // namespace astfri::csharp

#endif // CSHARP_TYPE_TRANSLATOR