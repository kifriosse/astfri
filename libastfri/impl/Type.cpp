#include <libastfri/Type.hpp>

namespace astfri
{
UserType::UserType(std::string name) :
    name_(std::move(name))
{
}

IndirectionType::IndirectionType(Type* const indirect) :
    indirect_(indirect)
{
}
} // namespace astfri