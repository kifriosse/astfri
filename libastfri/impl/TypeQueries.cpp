#include <libastfri/impl/TypeQueries.hpp>

namespace astfri
{
bool TypeQueriesAdapter::is_dynamic() const noexcept
{
    return false;
}

bool TypeQueriesAdapter::is_int() const noexcept
{
    return false;
}

bool TypeQueriesAdapter::is_float() const noexcept
{
    return false;
}

bool TypeQueriesAdapter::is_char() const noexcept
{
    return false;
}

bool TypeQueriesAdapter::is_bool() const noexcept
{
    return false;
}

bool TypeQueriesAdapter::is_void() const noexcept
{
    return false;
}

bool TypeQueriesAdapter::is_user() const noexcept
{
    return false;
}

bool TypeQueriesAdapter::is_indirection() const noexcept
{
    return false;
}
}