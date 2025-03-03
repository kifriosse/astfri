#ifndef LIBASTFRI_INC_TYPE_QUERY_HPP
#define LIBASTFRI_INC_TYPE_QUERY_HPP

#include <libastfri/impl/TypeFwd.hpp>
#include <libastfri/impl/ExprFwd.hpp>
#include <libastfri/impl/StmtFwd.hpp>

#include <type_traits>

namespace astfri
{
    template<class This>
    struct MkTypeQueryable
    {
        template<class T>
        bool is_a()
        {
            return std::is_base_of<T, This>::value;
        }
    };

    template<class T>
    bool is_a(Type* e)
    {
        static_assert(
          std::is_base_of<T, MkTypeQueryable<T>>::value,
          "Unsafe type query!"
        );
        return // TODO static cast to mktypequeryable
    }

    template<class T>
    bool is_a(Expr* e)
    {

    }

    template<class T>
    bool is_a(Stmt* e)
    {

    }

}

#endif