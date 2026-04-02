#ifndef ASTFRI_IMPL_TOOLS_PTR_HPP
#    include <astfri/impl/tools/Ptr.hpp>
#endif

namespace astfri {


template<typename T>
ReqPtr<T>::ReqPtr() : m_ptr(nullptr) {
}

template<typename T>
ReqPtr<T>::ReqPtr(T *ptr) : m_ptr(ptr) {
}

template<typename T>
T *ReqPtr<T>::operator->() {
    this->check_pointer_except();
    return m_ptr;
}

template<typename T>
const T *ReqPtr<T>::operator->() const {
    this->check_pointer_except();
    return m_ptr;
}

template<typename T>
ReqPtr<T> &ReqPtr<T>::operator=(const ReqPtr &other) {
    m_ptr = other.m_ptr;
    return *this;
}

template<typename T>
ReqPtr<T> &ReqPtr<T>::operator=(T *ptr) {
    m_ptr = ptr;
    return *this;
}

template<typename T>
T &ReqPtr<T>::operator*() {
    this->check_pointer_except();
    return *m_ptr;
}

template<typename T>
const T &ReqPtr<T>::operator*() const {
    this->check_pointer_except();
    return *m_ptr;
}

template<typename T>
ReqPtr<T>::operator T*() {
    this->check_pointer_except();
    return m_ptr;
}

template<typename T>
ReqPtr<T>::operator const T*() const {
    this->check_pointer_except();
    return m_ptr;
}

template<typename T>
bool ReqPtr<T>::has_ptr() const noexcept {
    return m_ptr != nullptr;
}

template<typename T>
void ReqPtr<T>::check_pointer_except() const {
#ifndef NDEBUG
    if (m_ptr == nullptr) {
        throw NullPointerException();
    }
#endif
}


template<typename T>
OptPtr<T>::OptPtr() : m_ptr(nullptr), m_isExplicitlyInitialized(false) {
}

template<typename T>
OptPtr<T>::OptPtr(NullOptPtr) : m_ptr(nullptr), m_isExplicitlyInitialized(true) {
}

template<typename T>
OptPtr<T>::OptPtr(T *ptr) : m_ptr(ptr), m_isExplicitlyInitialized(true) {
}



} // namespace astfri

