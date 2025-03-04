#include <libastfri/inc/TypeQuery.hpp>
#include <libastfri/impl/TypeId.hpp>

namespace astfri
{

template<class T>
bool is_type(Type* e)
{
    // TODO this one will get type if via virtual method from `e`
    // and compared with details::get_type_id<T>().
    // Later, the type ids could even capture base<-->derived relationship
    // and thus, is_a query could be implemented.
    return false;
}

template<class T>
bool is_type(Expr* e)
{
    return false;
}

template<class T>
bool is_type(Stmt* e)
{
    return false;
}

} // namespace astfri
