#ifndef CSHARP_IDENTIFIERS_HPP
#define CSHARP_IDENTIFIERS_HPP

#include <libastfri-cs/impl/util/common.hpp>

#include <functional>
#include <string>

namespace astfri::csharp
{

struct FuncId
{
    std::string name;
    size_t param_count{0};

    bool operator==(const FuncId& other) const;
};

struct MethodId
{
    FuncId func_id;
    bool is_static{false};

    bool operator==(const MethodId& other) const;
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/Identifiers.inl>

#endif
