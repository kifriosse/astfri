#ifndef CSHARP_TYPE_TRANSLATOR
#define CSHARP_TYPE_TRANSLATOR
#include <libastfri-cs/impl/data/Source.hpp>

namespace astfri::csharp
{
class SemanticContext;

class TypeTranslator
{
private:
    static TypeFactory& type_f_;

    Scope current_ns_{};
    SymbolTable& symb_table_;
    SourceCode* current_src_{nullptr};
    const TSLanguage* lang_;

public:
    explicit TypeTranslator(SymbolTable& symbol_table);
    void set_current_src(SourceCode* src);
    void set_current_namespace(Scope scope);

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
    [[nodiscard]] SourceCode* src() const;
    [[nodiscard]] std::string_view src_str() const;
};

} // namespace astfri::csharp

#endif // CSHARP_TYPE_TRANSLATOR