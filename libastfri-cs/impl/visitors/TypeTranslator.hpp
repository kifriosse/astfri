#ifndef CSHARP_TYPE_TRANSLATOR
#define CSHARP_TYPE_TRANSLATOR

#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

namespace astfri::csharp
{
struct SourceFile;

class TypeTranslator
{
private:
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
    void set_current_src(SourceFile* src);
    void set_current_namespace(ScopeNode* node);

    static Type* visit_predefined(TypeTranslator* self, const TSNode& node);
    static Type* visit_identitifier(TypeTranslator* self, const TSNode& node);
    static Type* visit_qualified_name(TypeTranslator* self, const TSNode& node);
    /**
     * Used for \code var\endcode keyword
     * @param self reference to instance of TypeTranslator
     * @param node TSNode that is being visited
     * @return Deduced type
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
    static Type* visit_inderect(TypeTranslator* self, const TSNode& node);
    static Type* visit_array(TypeTranslator* self, const TSNode& node);
    static Type* visit_generic_name(TypeTranslator* self, const TSNode& node);
    static Type* visit_tuple(TypeTranslator* self, const TSNode& node);
    static Type* visit_func_pointer(TypeTranslator* self, const TSNode& node);

    ScopeNode* resolve_qualif_name(
        const TSNode& nQualif,
        util::SearchScope searchScope,
        ScopeNode* start
    ) const;

private:
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
    [[nodiscard]] ScopeNode* find_entry_point(
        std::string_view qualif,
        util::SearchScope searchScope,
        ScopeNode* start,
        SourceFile* src
    ) const;
    [[nodiscard]] ScopeNode* bottom_up_search(
        std::string_view qualif,
        ScopeNode* start,
        SourceFile* src
    ) const;
    [[nodiscard]] ScopeNode* search_parents(
        std::string_view qualif,
        const TypeBinding& start
    ) const;
    [[nodiscard]] SourceFile* src() const;
    [[nodiscard]] std::string_view src_str() const;
};

} // namespace astfri::csharp

#endif // CSHARP_TYPE_TRANSLATOR