#ifndef LIBASTFRI_IMPL_UTILS_HPP
#define LIBASTFRI_IMPL_UTILS_HPP

#include <utility>
#include <libastfri/Visitor.hpp>

namespace astfri::details
{
    /**
     * @brief CRTP mixin that makes @c This child class visitable.
     * @tparam This Type of the child class.
     */
    template<typename This>
    struct MkVisitable : virtual IVisitable
    {
        void accept(IVisitor& visitor) override
        {
            visitor.visit(static_cast<This const&>(*this));
        }
    };

    /**
     * @brief Gets pointer to value from @p map if exists or constructs a new value from @p args and returns pointer to that value
     * @return pointer to a value that was already present or pointer to a new value constructed from @p args
     */
    template<typename Val, typename Map, typename Key, typename... Args>
    Val* emplace_get(Key&& key, Map& map, Args&&... args)
    {
        const auto it = map.find(key);
        if (it != map.end())
        {
            return &it->second;
        }
        return &map.try_emplace(std::move(key), std::forward<Args>(args)...).first->second;
    }
}

#endif