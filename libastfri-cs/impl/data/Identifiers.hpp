#ifndef CSHARP_IDENTIFIERS_HPP
#define CSHARP_IDENTIFIERS_HPP

// ReSharper disable once CppUnusedIncludeDirective
#include <libastfri-cs/impl/util/Common.hpp>

// ReSharper disable once CppUnusedIncludeDirective
#include <functional>
#include <string>

namespace astfri::csharp
{

/**
 * @brief Identifier for invocations - method call, local function call,
 * delegate call, etc.
 */
struct InvocationId
{
    std::string name;
    size_t paramCount{0};
};

/**
 * @brief Identifier for method definitions
 */
struct MethodId
{
    std::string name;
    size_t paramCount{0};
    bool isStatic;

    bool operator==(const MethodId& other) const;
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/Identifiers.inl>

#endif
