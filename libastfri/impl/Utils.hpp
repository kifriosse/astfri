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
    struct MakeVisitable : IVisitable
    {
        void accept (IVisitor& visitor) override
        {
            visitor.visit(static_cast<This const&>(*this));
        }
    };
} // namespace details
} // namespace astfri

#endif