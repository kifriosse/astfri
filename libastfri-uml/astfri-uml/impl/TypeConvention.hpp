#ifndef LIBASTFRIUML_TYPE_CONVENTION_HPP
#define LIBASTFRIUML_TYPE_CONVENTION_HPP

#include <string>

namespace astfri::uml {
enum class TypeConventions {
    // type var
    TypeBeforeIdentifier = 0,

    // var : type
    TypeAfterIdentifier
};

std::string_view to_string(TypeConventions tc);

TypeConventions from_string(std::string_view tcStr);


class TypeConvention {
public:
    static std::string get_string(
        const std::string &type,
        const std::string &identifier,
        const std::string &separator,
        TypeConventions conv
    );

    TypeConvention() = delete;
};
} // namespace astfri::uml

#endif
