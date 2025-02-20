#ifndef LIBASTFRI_IMPL_TYPE_QUERIS_HPP
#define LIBASTFRI_IMPL_TYPE_QUERIS_HPP

#include <libastfri/impl/TypeFwd.hpp>

namespace astfri::details
{
struct ITypeQueries
{
    virtual ~ITypeQueries()                             = default;

    virtual DynamicType* as_dynamic () noexcept         = 0;
    virtual IntType* as_int () noexcept                 = 0;
    virtual FloatType* as_float () noexcept             = 0;
    virtual CharType* as_char () noexcept               = 0;
    virtual BoolType* as_bool () noexcept               = 0;
    virtual VoidType* as_void () noexcept               = 0;
    virtual UserType* as_user () noexcept               = 0;
    virtual IndirectionType* as_indirection () noexcept = 0;
};

struct TypeQueriesAdapter : ITypeQueries
{
    DynamicType* as_dynamic () noexcept override;
    IntType* as_int () noexcept override;
    FloatType* as_float () noexcept override;
    CharType* as_char () noexcept override;
    BoolType* as_bool () noexcept override;
    VoidType* as_void () noexcept override;
    UserType* as_user () noexcept override;
    IndirectionType* as_indirection () noexcept override;
};
} // namespace astfri::details

#endif
