#ifndef ASTFRI_IMPL_TOOLS_REQ_PTR_HPP
#    include <astfri/impl/tools/ReqPtr.hpp>
#endif


namespace astfri {


template<typename T>
constexpr ReqPtr<T>::ReqPtr() : m_ptr(nullptr) {
}

template<typename T>
constexpr ReqPtr<T>::ReqPtr(T *ptr) : m_ptr(ptr) {
}

template<typename T>
constexpr T *ReqPtr<T>::operator->() {
    check_uninitialized(m_ptr);
    return m_ptr;
}

template<typename T>
constexpr const T *ReqPtr<T>::operator->() const {
    check_uninitialized(m_ptr);
    return m_ptr;
}

template<typename T>
constexpr ReqPtr<T> &ReqPtr<T>::operator=(T *ptr) {
    m_ptr = ptr;
    return *this;
}

template<typename T>
constexpr T &ReqPtr<T>::operator*() {
    check_uninitialized(m_ptr);
    return *m_ptr;
}

template<typename T>
constexpr const T &ReqPtr<T>::operator*() const {
    check_uninitialized(m_ptr);
    return *m_ptr;
}

template<typename T>
constexpr ReqPtr<T>::operator T*() {
    check_uninitialized(m_ptr);
    return m_ptr;
}

template<typename T>
constexpr ReqPtr<T>::operator const T*() const {
    check_uninitialized(m_ptr);
    return m_ptr;
}

template<typename T>
constexpr bool ReqPtr<T>::operator==(const ReqPtr &other) const noexcept {
    check_uninitialized(m_ptr);
    check_uninitialized(other.m_ptr);
    return m_ptr == other.m_ptr;
}

template<typename T>
constexpr bool ReqPtr<T>::operator!=(const ReqPtr &other) const noexcept {
    check_uninitialized(m_ptr);
    check_uninitialized(other.m_ptr);
    return m_ptr != other.m_ptr;
}

template<typename T>
constexpr bool ReqPtr<T>::operator==(const T *other) const noexcept {
    check_uninitialized(m_ptr);
    return m_ptr == other;
}

template<typename T>
constexpr bool ReqPtr<T>::operator!=(const T* other) const noexcept {
    check_uninitialized(m_ptr);
    return m_ptr != other;
}

template<typename T>
constexpr void ReqPtr<T>::check_uninitialized([[maybe_unused]] T *ptr) {
#ifndef NDEBUG
    if (ptr == nullptr) {
        throw UninitializedException();
    }
#endif
}


} // namespace astfri
