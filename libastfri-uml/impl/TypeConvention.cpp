#include <libastfri-uml/inc/TypeConvention.hpp>

namespace astfri::uml
{
std::string TypeAfterConvention::get_string(std::string type, std::string val, char separator)
{
    return val + separator + type;
}

std::string TypeBeforeConvention::get_string(std::string type, std::string val, char separator)
{
    return type + separator + val;
}
} // namespace astfri::uml