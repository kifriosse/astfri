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

    SymbolNode* currentNmsNode_{};
    SymbolTable& symbTable_;
    SourceFile* currentSrc_{nullptr};
    const TSLanguage* lang_;

public:
    explicit TypeTranslator(SymbolTable& symbTable);
    void set_current_src(SourceFile* src);
    void set_current_namespace(SymbolNode* node);

    static Type* visit_predefined(TypeTranslator* self, const TSNode& node);
    static Type* visit_identitifier(TypeTranslator* self, const TSNode& node);
    static Type* visit_qualified_name(TypeTranslator* self, const TSNode& node);
    /**
     * Used for \c var keyword
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

private:
    [[nodiscard]] SourceFile* src() const;
    [[nodiscard]] std::string_view src_str() const;
};

} // namespace astfri::csharp

#endif // CSHARP_TYPE_TRANSLATOR