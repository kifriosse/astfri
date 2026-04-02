#ifndef ASTFRI_IMPL_TOOLS_PTR_HPP
#define ASTFRI_IMPL_TOOLS_PTR_HPP

#ifndef NDEBUG
#include <exception>
#endif


namespace astfri {


template<typename T>
class ReqPtr {
public:
    ReqPtr();

    ReqPtr(T *ptr);

    T *operator->();

    const T *operator->() const;

    ReqPtr &operator=(const ReqPtr &other);

    ReqPtr &operator=(T *ptr);

    T &operator*();

    const T &operator*() const;

    operator T*();

    operator const T*() const;

    bool operator==(const ReqPtr &other) const noexcept = default;

    bool operator!=(const ReqPtr &other) const noexcept = default;

    bool has_ptr() const noexcept;

private:
    void check_pointer_except() const;

private:
    T *m_ptr;
};


template<typename T>
class OptPtr {
public:


private:
    T *m_ptr;
};


#ifndef NDEBUG

class NullPointerException : public std::exception {
};

#endif // NDEBUG


} // namespace astfri


#include <astfri/impl/tools/Ptr.inl>


#endif
