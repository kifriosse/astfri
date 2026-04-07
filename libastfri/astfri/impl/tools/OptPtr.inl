#ifndef ASTFRI_IMPL_TOOLS_OPT_PTR_HPP
#    include <astfri/impl/tools/OptPtr.hpp>
#endif

namespace astfri {


template<typename T>
constexpr OptPtr<T>::OptPtr() : m_state(Uninitialized{}) {
}

template<typename T>
constexpr OptPtr<T>::OptPtr(NoneOptPtr) : m_state(None{}) {
}

template<typename T>
constexpr OptPtr<T>::OptPtr(T *ptr) : m_state(ptr ? Value{ptr} : Uninitialized{}) {
}

template<typename T>
constexpr T *OptPtr<T>::operator->() {
    check_uninitialized(m_state);
    check_value(m_state);
    return this->get_value_unsafe();
}

template<typename T>
constexpr const T *OptPtr<T>::operator->() const {
    check_uninitialized(m_state);
    check_value(m_state);
    return this->get_value_unsafe();
}

template<typename T>
constexpr OptPtr<T> &OptPtr<T>::operator=(T *ptr) {
    m_state = ptr ? Value{ptr} : Uninitialized{};
}

template<typename T>
constexpr OptPtr<T> &OptPtr<T>::operator=(NoneOptPtr) {
    m_state = None{};
    return *this;
}

template<typename T>
constexpr T &OptPtr<T>::operator*() {
    check_uninitialized(m_state);
    check_value(m_state);
    return *this->get_value_unsafe();
}

template<typename T>
constexpr const T &OptPtr<T>::operator*() const {
    check_uninitialized(m_state);
    check_value(m_state);
    return *this->get_value_unsafe();
}

template<typename T>
constexpr OptPtr<T>::operator T*() {
    check_uninitialized(m_state);
    check_value(m_state);
    return this->get_value_unsafe();
}

template<typename T>
constexpr OptPtr<T>::operator const T*() const {
    check_uninitialized(m_state);
    check_value(m_state);
    return this->get_value_unsafe();
}

template<typename T>
constexpr bool OptPtr<T>::operator==(const OptPtr &other) const noexcept {
    check_uninitialized(m_state);
    check_uninitialized(other.m_state);
    if (this->is_none()) {
        return other.is_none();
    }
    return !other.is_none() && this->get_value_unsafe() == other.get_value_unsafe();
}

template<typename T>
constexpr bool OptPtr<T>::operator!=(const OptPtr &other) const noexcept {
    check_uninitialized(m_state);
    check_uninitialized(other.m_state);
    if (this->is_none()) {
        return !other.is_none();
    }
    return other.is_none() || this->get_value_unsafe() != other.get_value_unsafe();
}

template<typename T>
constexpr bool OptPtr<T>::operator==(const T *other) const noexcept {
    check_uninitialized(m_state);
    if (this->is_none()) {
        return other == nullptr;
    }
    return this->get_value_unsafe() == other;
}

template<typename T>
constexpr bool OptPtr<T>::operator!=(const T *other) const noexcept {
    check_uninitialized(m_state);
    if (this->is_none()) {
        return other != nullptr;
    }
    return this->get_value_unsafe() != other;
}

template<typename T>
constexpr bool OptPtr<T>::is_none() const noexcept {
    check_uninitialized(m_state);
    return std::holds_alternative<None>(m_state);
}

template<typename T>
constexpr void OptPtr<T>::check_uninitialized(
    [[maybe_unused]] const std::variant<Uninitialized, None, Value> &ptr
) {
#ifndef NDEBUG
    if (std::holds_alternative<Uninitialized>(ptr)) {
        throw UninitializedException();
    }
#endif
}

template<typename T>
constexpr void OptPtr<T>::check_value(
    [[maybe_unused]] const std::variant<Uninitialized, None, Value> &ptr
) {
#ifndef NDEBUG
    if (!std::holds_alternative<Value>(ptr)) {
        throw ValuelessException();
    }
#endif
}

template<typename T>
T *OptPtr<T>::get_value_unsafe() const {
    return std::get<Value>(m_state).ptr;
}


} // namespace astfri
