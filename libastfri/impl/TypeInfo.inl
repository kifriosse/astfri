#include <libastfri/inc/TypeInfo.hpp>

namespace astfri
{

template<typename T>
bool is_a(Type *t)
{
    return as_a<T>(t) != nullptr;
}

template<typename T>
T *as_a(Type *t)
{
    return dynamic_cast<T*>(t);
}


template<typename T>
bool is_a(Stmt *t)
{
    return as_a<T>(t) != nullptr;
}

template<typename T>
T *as_a(Stmt *t)
{
    return dynamic_cast<T*>(t);
}


template<typename T>
bool is_a(Expr *t)
{
    return as_a<T>(t) != nullptr;
}

template<typename T>
T *as_a(Expr *t)
{
    return dynamic_cast<T*>(t);
}

} // namespace astfri
