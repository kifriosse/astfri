#include <libastfri/Type.hpp>

namespace astfri
{
UserType::UserType(std::string name) :
    name_(std::move(name))
{
}
} // namespace astfri