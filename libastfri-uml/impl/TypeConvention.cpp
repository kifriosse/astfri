#include <libastfri-uml/impl/TypeConvention.hpp>

namespace astfri::uml
{
std::string TypeConvention::get_string(
        std::string type,
        std::string identifier,
        char separator,
        TypeConventions conv
)
{
    switch (conv)
    {
        case TypeConventions::TYPE_BEFORE_IDENTIFIER:
            return type + separator + identifier;
        case TypeConventions::TYPE_AFTER_IDENTIFIER:
        default:
            return identifier + separator + type;
    }
}
} // namespace astfri::uml
