#ifndef LIBASTFRIUML_TYPE_CONVENTION_HPP
#define LIBASTFRIUML_TYPE_CONVENTION_HPP

#include <string>

namespace astfri::uml
{
enum class TypeConventions
{
    TYPE_BEFORE_IDENTIFIER = 0,
    TYPE_AFTER_IDENTIFIER
};

class TypeConvention
{
public:
    static std::string get_string(
            std::string type,
            std::string identifier,
            char separator,
            TypeConventions conv
    );

    TypeConvention() = delete;
};
} // namespace astfri::uml

#endif
