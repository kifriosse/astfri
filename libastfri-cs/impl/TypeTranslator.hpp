#ifndef CSHARP_TYPE_TRANSLATOR
#define CSHARP_TYPE_TRANSLATOR
#include <libastfri-cs/impl/data/Source.hpp>

namespace astfri::csharp
{

class TypeTranslator
{
private:
    static TypeFactory& type_factory_;

    SourceCode* current_src{nullptr};

public:
    void set_current_src(SourceCode* src);

    static Type* visit_predefined(TypeTranslator* self, const TSNode& node);
    static Type* visit_identitifier(TypeTranslator* self, const TSNode& node);
    static Type* visit_qualified_name(TypeTranslator* self, const TSNode& node);
    static Type* visit_implicit(TypeTranslator* self, const TSNode& node);
    static Type* visit_nullable(TypeTranslator* self, const TSNode& node);
    static Type* visit_pointer(TypeTranslator* self, const TSNode& node);
    static Type* visit_ref(TypeTranslator* self, const TSNode& node);
    static Type* visit_array(TypeTranslator* self, const TSNode& node);
    static Type* visit_generic_name(TypeTranslator* self, const TSNode& node);
    static Type* visit_tuple(TypeTranslator* self, const TSNode& node);
    static Type* visit_func_pointer(TypeTranslator* self, const TSNode& node);
    static Type* visit_scoped_type(TypeTranslator* self, const TSNode& node);
};

} // namespace astfri::csharp

#endif // CSHARP_TYPE_TRANSLATOR