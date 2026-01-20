#ifndef CSHARP_IDENTIFIERS_HPP
#define CSHARP_IDENTIFIERS_HPP

// ReSharper disable once CppUnusedIncludeDirective
#include <libastfri-cs/impl/util/common.hpp>

// ReSharper disable once CppUnusedIncludeDirective
#include <functional>
#include <string>

namespace astfri::csharp
{

struct InvocationId
{
    std::string name;
    size_t param_count{0};
};

struct MethodId
{
    std::string name;
    size_t param_count{0};
    bool is_static;

    bool operator==(const MethodId& other) const;
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/Identifiers.inl>

#endif
