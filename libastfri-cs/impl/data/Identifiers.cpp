#include <libastfri-cs/impl/data/Identifiers.hpp>

namespace astfri::csharp
{

bool FuncId::operator==(const FuncId& other) const
{
    return name == other.name && param_count == other.param_count;
}

bool MethodId::operator==(const MethodId& other) const
{
    return func_id == other.func_id && is_static == other.is_static;
}

} // namespace astfri::csharp