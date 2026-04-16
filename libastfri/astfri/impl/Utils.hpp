#ifndef ASTFRI_IMPL_UTILS_HPP
#define ASTFRI_IMPL_UTILS_HPP

#include <astfri/impl/Visitor.hpp>

#include <memory>
#include <utility>


namespace astfri::details {

/**
 * @brief Map-like container that supports @c find, @c begin, @c end, and @c try_emplace.
 * Semantics of the mentioned operations must match that of @c std::map operations.
 */
template<typename Map>
concept map_like = requires(Map m, typename Map::key_type k) {
    m.find(k);
    m.begin();
    m.end();
    m.try_emplace(k);
};

/**
 * @brief Satisfied by any container that supports @c push_back operation.
 */
template<typename Vector>
concept vector_like = requires(Vector v, typename Vector::value_type x) { v.push_back(x); };

/**
 * @brief Gets pointer to value from @p map if @p key exists or constructs a new value
 * using @p creator and returns pointer to that value.
 * @return Pointer to a value that was already present or pointer to a new value.
 */
template<typename Val, map_like Map, typename Key, typename Creator>
Val *emplace_get(Map& map, Key&& key, Creator creator) {
    const auto it = map.find(key);
    if (it != map.end()) {
        return &it->second;
    }
    return &map.try_emplace(std::move(key), creator()).first->second;
}

/**
 * @brief Creates new instance of @c T from @p args and stores it in @p store.
 * @param store Vector-like container for storing unique pointers to @c T
 * @param args Param pack used to construct an instance of @c T
 * @return Pointer to the new @c T instance.
 */
template<typename T, vector_like Vector, typename Initializer>
T* create_store_get(Vector& store, Initializer initialize) {
    auto e = std::make_unique<T>();
    T *raw = e.get();
    initialize(*raw);
    store.push_back(std::move(e));
    return raw;
}

} // namespace astfri::details

#endif
