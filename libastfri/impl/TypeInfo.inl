#include <libastfri/inc/TypeInfo.hpp>

namespace astfri
{

template<typename T>
requires(std::derived_from<T, Type>)
bool is_a(Type *t)
{
    return as_a<T>(t) != nullptr;
}

template<typename T>
requires(std::derived_from<T, Type>)
T *as_a(Type *t)
{
    return dynamic_cast<T*>(t);
}


template<typename T>
requires(std::derived_from<T, Stmt>)
bool is_a(Stmt *t)
{
    return as_a<T>(t) != nullptr;
}

template<typename T>
requires(std::derived_from<T, Stmt>)
T *as_a(Stmt *t)
{
    return dynamic_cast<T*>(t);
}


template<typename T>
requires(std::derived_from<T, Expr>)
bool is_a(Expr *t)
{
    return as_a<T>(t) != nullptr;
}

template<typename T>
requires(std::derived_from<T, Expr>)
T *as_a(Expr *t)
{
    return dynamic_cast<T*>(t);
}

} // namespace astfri
