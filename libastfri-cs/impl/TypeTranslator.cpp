#include <libastfri-cs/impl/TypeTranslator.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

namespace astfri::csharp
{

TypeFactory& TypeTranslator::type_factory_ = TypeFactory::get_instance();

void TypeTranslator::set_current_src(SourceCode* src)
{
    current_src = src;
}

Type* TypeTranslator::visit_predefined(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_identitifier(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_qualified_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_implicit(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_nullable(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_pointer(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_ref(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_array(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_generic_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_tuple(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_func_pointer(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_scoped_type(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}
} // namespace astfri::csharp