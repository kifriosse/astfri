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

    explicit ReqPtr(T *ptr);

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


/**
 * @brief Our null literal type for @c OptPtr type.
 */
struct NullOptPtr {
    enum class DummyTokenType { Token };
    explicit constexpr NullOptPtr(DummyTokenType) noexcept { }
};

/**
 * @brief Our null literal for @c OptPtr type.
 */
inline constexpr NullOptPtr null { NullOptPtr::DummyTokenType::Token };


template<typename T>
class OptPtr {
public:
    OptPtr();

    explicit OptPtr(NullOptPtr);

    explicit OptPtr(T *ptr);

    T *operator->();

    const T *operator->() const;

    OptPtr &operator=(const OptPtr &other);

    OptPtr &operator=(T *ptr);

    OptPtr &operator=(NullOptPtr);

    T &operator*();

    const T &operator*() const;

    operator T*();

    operator const T*() const;

    bool operator==(const OptPtr &other) const noexcept = default;

    bool operator!=(const OptPtr &other) const noexcept = default;



private:
    T *m_ptr;
    bool m_isExplicitlyInitialized;
};


#ifndef NDEBUG

class NullPointerException : public std::exception {
};

#endif // NDEBUG


} // namespace astfri


#include <astfri/impl/tools/Ptr.inl>


#endif
