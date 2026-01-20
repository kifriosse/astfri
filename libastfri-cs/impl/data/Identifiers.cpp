#include <libastfri-cs/impl/data/Identifiers.hpp>

namespace astfri::csharp
{

bool MethodId::operator==(const MethodId& other) const
{
    return name == other.name && param_count == other.param_count
        && is_static == other.is_static;
}

} // namespace astfri::csharp