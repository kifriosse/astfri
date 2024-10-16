#ifndef LIBASTFRI_IMPL_TYPE_QUERIS_HPP
#define LIBASTFRI_IMPL_TYPE_QUERIS_HPP

namespace astfri
{
struct ITypeQueries
{
    virtual ~ITypeQueries() = default;

    virtual bool is_dynamic() const noexcept = 0;
    virtual bool is_int() const noexcept = 0;
    virtual bool is_float() const noexcept = 0;
    virtual bool is_char() const noexcept = 0;
    virtual bool is_bool() const noexcept = 0;
    virtual bool is_void() const noexcept = 0;
    virtual bool is_user() const noexcept = 0;
    virtual bool is_indirection() const noexcept = 0;
};

struct TypeQueriesAdapter : ITypeQueries
{
    bool is_dynamic() const noexcept override;
    bool is_int() const noexcept override;
    bool is_float() const noexcept override;
    bool is_char() const noexcept override;
    bool is_bool() const noexcept override;
    bool is_void() const noexcept override;
    bool is_user() const noexcept override;
    bool is_indirection() const noexcept override;
};
}

#endif