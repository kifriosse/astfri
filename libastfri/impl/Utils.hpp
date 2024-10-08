#ifndef LIBASTFRI_IMPL_UTILS_HPP
#define LIBASTFRI_IMPL_UTILS_HPP

#include <libastfri/Visitor.hpp>

namespace astfri
{
namespace details
{
/**
 * @brief CRTP mixin that makes @c This child class visitable.
 * @tparam This Type of the child class.
 */
template<typename This>
struct MakeVisitable : virtual public IVisitable
{
    void accept (IVisitor& visitor) override
    {
        visitor.Visit(static_cast<This const&>(*this));
    }
};
}
}

#endif