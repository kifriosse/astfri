#ifndef LIBASTFRI_IMPL_UTILS_HPP
#define LIBASTFRI_IMPL_UTILS_HPP

#include <libastfri/inc/Visitor.hpp>

#include <memory>
#include <utility>

namespace astfri::details {

/**
 * @brief TODO
 */
template<typename Map>
concept map_like = requires(Map m, typename Map::key_type k) {
    m.find(k);
    m.begin();
    m.end();
    m.try_emplace(k);
};

/**
 * @brief TODO
 */
template<typename Vector>
concept vector_like = requires(Vector v, typename Vector::value_type x) { v.push_back(x); };

/**
 * @brief Gets pointer to value from @p map if exists or constructs a new value
 * from @p args and returns pointer to that value
 * @return pointer to a value that was already present or pointer to a new value
 * constructed from @p args
 */
template<typename Val, map_like Map, typename Key, typename... Args>
Val* emplace_get(Key&& key, Map& map, Args&&... args) {
    const auto it = map.find(key);
    if (it != map.end()) {
        return &it->second;
    }
    return &map.try_emplace(std::move(key), std::forward<Args>(args)...).first->second;
}

/**
 * @brief Creates new instance of @c T from @p args and stores it in @p store.
 * @param store Vector-like container for storing unique pointers to @c T
 * @param args Param pack used to construct an instance of @c T
 * @return Pointer to the new @c T instance.
 */
template<typename T, vector_like Vector, typename... Args>
T* emplace_get(Vector& store, Args&&... args) {
    auto e = std::make_unique<T>(std::forward<Args>(args)...);
    store.push_back(std::move(e));
    return static_cast<T*>(store.back().get());
}

} // namespace astfri::details

#endif
