#include <libastfri-cs/impl/data/Identifiers.hpp>

namespace astfri::csharp
{

bool MethodId::operator==(const MethodId& other) const
{
    return name == other.name && paramCount == other.paramCount
        && isStatic == other.isStatic;
}

} // namespace astfri::csharp