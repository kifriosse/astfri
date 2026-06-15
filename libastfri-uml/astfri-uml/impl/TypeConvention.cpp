#include <astfri-uml/impl/TypeConvention.hpp>

#include <stdexcept>


namespace astfri::uml {


std::string_view to_string(TypeConventions tc) {
    switch (tc) {
        case TypeConventions::TypeAfterIdentifier:
            return "TypeAfterIdentifier";

        case TypeConventions::TypeBeforeIdentifier:
            return "TypeBeforeIdentifier";

        default:
            throw std::runtime_error(
                "Unhandled enum value in `to_string(TypeConventions)`.");
    }
}


TypeConventions from_string(std::string_view tcStr) {
    if (tcStr == "TypeBeforeIdentifier") {
        return TypeConventions::TypeBeforeIdentifier;
    }
    else if (tcStr == "TypeAfterIdentifier") {
        return TypeConventions::TypeAfterIdentifier;
    }
    else {
        throw std::runtime_error(
            "Invalid enum string value in `TypeConventions from_string(std::string_view)`.");
    }
}


std::string TypeConvention::get_string(
    const std::string &type,
    const std::string &identifier,
    const std::string &separator,
    TypeConventions conv
) {
    switch (conv) {
    case TypeConventions::TypeBeforeIdentifier:
        return type + separator + identifier;
    case TypeConventions::TypeAfterIdentifier:
    default:
        return identifier + separator + type;
    }
}


} // namespace astfri::uml
