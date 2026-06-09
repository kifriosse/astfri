#ifndef ASTFRI_IMPL_TOOLS_REQ_PTR_HPP
#define ASTFRI_IMPL_TOOLS_REQ_PTR_HPP

#include <astfri/impl/tools/Exceptions.hpp>


namespace astfri {


/**
 * @brief Safe wrapper for raw pointer. Throws in debug build when used without being initialized.
 *
 * Use this for pointers that must allways point to something.
 * The raw pointer can be provided in the constructor or assigned later (before first usage).
 * There is no way to use this pointer without it pointing to something.
 *
 * @tparam T pointed to type
 */
template<typename T>
class ReqPtr {
public:
    /**
     * @brief Initializes the pointer to @c nullptr. Must be assigned later.
     */
    constexpr ReqPtr();

    /**
     * @brief Initializes the pointer to @p ptr. If @p ptr is nullptr, it must be assigned later.
     */
    constexpr explicit ReqPtr(T *ptr);

    /**
     * @brief Default copy constructor.
     * @param other Other to be copied.
     */
    constexpr ReqPtr(const ReqPtr &other) = default;

    /**
     * @brief Accesses the raw pointer.
     * @throw UninitializedException if raw pointer is @c nullptr in debug build.
     * @return Raw pointer to T.
     */
    [[nodiscard]] constexpr T *operator->();

    /**
     * @brief Accesses the raw pointer.
     * @throw UninitializedException if raw pointer is @c nullptr in debug build.
     * @return Raw pointer to const qualified T.
     */
    [[nodiscard]] constexpr const T *operator->() const;

    /**
     * @brief Default assignment.
     * @param other Other to be assigned to this.
     * @return Reference to this.
     */
    constexpr ReqPtr &operator=(const ReqPtr &other) = default;

    /**
     * @brief Assigns @p ptr raw pointer to this pointer.
     *
     * @p ptr can be @c nullptr but this will be still considered uninitialzied.
     *
     * @param ptr Pointer to be assigned.
     * @return Reference to this.
     */
    constexpr ReqPtr &operator=(T *ptr);

    /**
     * @brief Accesses the pointed to object.
     * @throw UninitializedException if raw pointer is @c nullptr in debug build.
     * @return Reference to T.
     */
    [[nodiscard]] constexpr T &operator*();

    /**
     * @brief Accesses the pointed to object.
     * @throw UninitializedException if raw pointer is @c nullptr in debug build.
     * @return Const reference to T.
     */
    [[nodiscard]] constexpr const T &operator*() const;

    /**
     * @brief Implicit conversion to raw pointer.
     * @throw @c UninitializedException if raw pointer is @c nullptr in debug build.
     */
    constexpr operator T*();

    /**
     * @brief Implicit conversion to raw pointer.
     * @throw @c UninitializedException if raw pointer is @c nullptr in debug build.
     */
    constexpr operator const T*() const;

    /**
     * @brief Default comparison.
     * @param other Other pointer wrapper to compare with.
     * @throw @c UninitializedException if raw pointer is @c nullptr in debug build.
     * @return @c true of this and @p other hold the same pointer.
     */
    [[nodiscard]] constexpr bool operator==(const ReqPtr &other) const noexcept;

    /**
     * @brief Default comparison.
     * @param other Other pointer wrapper to compare with.
     * @throw @c UninitializedException if raw pointer is @c nullptr in debug build.
     * @return @c true of this and @p other do not hold the same pointer.
     */
    [[nodiscard]] constexpr bool operator!=(const ReqPtr &other) const noexcept;

    /**
     * @brief Comparison with raw poiunter.
     * @param other Raw pointer to compare with.
     * @throw @c UninitializedException if raw pointer is @c nullptr in debug build.
     * @return @c true of this holds the same pointer as @p other.
     */
    [[nodiscard]] constexpr bool operator==(const T *other) const noexcept;

    /**
     * @brief Comparison with raw poiunter.
     * @param other Raw pointer to compare with.
     * @throw @c UninitializedException if raw pointer is @c nullptr in debug build.
     * @return @c true of this does not hold the same pointer as @p other.
     */
    [[nodiscard]] constexpr bool operator!=(const T* other) const noexcept;

private:
    /**
     * @brief Checks if @p ptr is @c nullptr and if so throws in debug build.
     * @throw @c UninitializedException if @p ptr is @c nullptr in debug build.
     * @param ptr Pointer to check.
     */
    constexpr static void check_uninitialized(T *ptr);

private:
    /** @brief Wrapped raw pointer. */
    T *m_ptr;
};


} // namespace astfri


#include <astfri/impl/tools/ReqPtr.inl>


#endif
