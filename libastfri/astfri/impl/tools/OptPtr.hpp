#ifndef ASTFRI_IMPL_TOOLS_OPT_PTR_HPP
#define ASTFRI_IMPL_TOOLS_OPT_PTR_HPP

#include <astfri/impl/tools/Exceptions.hpp>

#include <variant>


namespace astfri {


/**
 * @brief Our none literal type for @c OptPtr type.
 */
struct NoneOptPtr {
    enum class DummyTokenType { Token };
    explicit constexpr NoneOptPtr(DummyTokenType) noexcept { }
};

/**
 * @brief Our none literal for @c OptPtr type.
 */
inline constexpr NoneOptPtr NONE { NoneOptPtr::DummyTokenType::Token };

/**
 * @brief Safe wrapper for raw pointer. Throws in debug when used without being initialized.
 *
 * Use this for pointers that do not allways need to point to something.
 * The raw pointer can be provided in the constructor or assigned later (before first usage).
 * When the pointer intentionally does not point to anything (practically, is nullptr),
 * it must be assigned above-defined @c NONE literal before first usage.
 * There is no way to use this pointer without it pointing to something
 * (including explicit nothing).
 *
 * @tparam T pointed to type
 */
template<typename T>
class OptPtr {
public:
    /**
     * @brief Default constructor. This is still considered unitialized.
     * Must be assigned either @c NONE or valid pointer later.
     */
    constexpr OptPtr();

    /**
     * @brief Initializes this with @c NONE.
     * @param none @c NONE literal @c ::astfri::NONE
     */
    constexpr explicit OptPtr(NoneOptPtr none);

    /**
     * @brief Initializes this with @p ptr.
     *
     * @p ptr can be @c nullptr but this will be still considered uninitialzied.
     *
     * @param ptr Pointer to be wrapped.
     */
    constexpr explicit OptPtr(T *ptr);

    /**
     * @brief Default copy constructor.
     * @param other Other to be copied.
     */
    constexpr OptPtr(const OptPtr &other) = default;

    /**
     * @brief Accesses the raw pointer.
     * @throw ValuelessException in debug build if this is @c NONE or uninitialized.
     * @return Raw pointer to T.
     */
    [[nodiscard]] constexpr T *operator->();

    /**
     * @brief Accesses the raw pointer.
     * @throw ValuelessException in debug build if this is @c NONE or uninitialized.
     * @return Raw pointer to const qualified T.
     */
    [[nodiscard]] constexpr const T *operator->() const;

    /**
     * @brief Default assignment.
     * @param other Other to be assigned to this.
     * @return Reference to this.
     */
    constexpr OptPtr &operator=(const OptPtr &other) = default;

    /**
     * @brief Assigns @p ptr to this.
     *
     * @p ptr can be @c nullptr but this will be still considered uninitialzied.
     *
     * @param ptr Pointer to be assigned.
     * @return Reference to this.
     */
    constexpr OptPtr &operator=(T *ptr);

    /**
     * @brief Explicitly assigns @c NONE.
     * @param none @c NONE literal @c ::astfri::NONE
     * @return Reference to this.
     */
    constexpr OptPtr &operator=(NoneOptPtr none);

    /**
     * @brief Accesses the pointed to object.
     * @throw ValuelessException in debug build if this is @c NONE or uninitialized.
     * @return Reference to T.
     */
    [[nodiscard]] constexpr T &operator*();

    /**
     * @brief Accesses the pointed to object.
     * @throw ValuelessException in debug build if this is @c NONE or uninitialized.
     * @return Const reference to T.
     */
    [[nodiscard]] constexpr const T &operator*() const;

    /**
     * @brief Implicit conversion to raw pointer.
     * @throw ValuelessException in debug build if this is @c NONE or uninitialized.
     */
    constexpr operator T*();

    /**
     * @brief Implicit conversion to raw pointer.
     * @throw ValuelessException in debug build if this is @c NONE or uninitialized.
     */
    constexpr operator const T*() const;

    /**
     * @brief Equality comparison.
     * @param other Other pointer wrapper to compare with.
     * @throw UninitializedException in debug build if this or other was not initialized.
     * @return @c true of this and @p other hold the same pointer or both @c NONE.
     */
    [[nodiscard]] constexpr bool operator==(const OptPtr &other) const noexcept;

    /**
     * @brief Non-equality comparison.
     * @param other Other pointer wrapper to compare with.
     * @throw UninitializedException in debug build if this or other was not initialized.
     * @return @c true of this and @p other are in different states or hold different pointers.
     */
    [[nodiscard]] constexpr bool operator!=(const OptPtr &other) const noexcept;

    /**
     * @brief Equality comparison with raw pointer.
     * @param other Raw pointer to compare with.
     * @throw UninitializedException in debug build if this was not initialized.
     * @return @c true of this holds pointer @p other or this is @c NONE and @p other @c nullptr.
     */
    [[nodiscard]] constexpr bool operator==(const T *other) const noexcept;

    /**
     * @brief Non-equality comparison with raw pointer.
     * @param other Raw pointer to compare with.
     * @throw UninitializedException in debug build if this was not initialized.
     * @return @c true of this and @p other are different.
     */
    [[nodiscard]] constexpr bool operator!=(const T *other) const noexcept;

    /**
     * @brief Checks if this was explicitly initialized to @c NONE.
     * @throw UninitializedException if not initialized at all.
     * @return @c true if this was explicitly initialized to @c NONE.
     */
    constexpr bool is_none() const noexcept;

    /**
     * @brief Check if this points to something.
     * @throw UninitializedException if not initialized at all.
     * @return @c true if this is points to valid object.
     */
    constexpr bool has_value () const noexcept;

private:
    /**
     * @brief Represents uninitialized state.
     */
    struct Uninitialized {
    };

    /**
     * @brief Represents state in which the pointer is explicitly set to @c NONE.
     */
    struct None {
    };

    /**
     * @brief Represents state in which the pointer points to valid object.
     */
    struct Value {
        T *ptr;
    };

private:
    /**
     * @brief Checks if @p state is not @c Uninitialized. If not, throws @c UninitialziedException.
     * @param state State to check.
     * @throw UninitializedException if @p state is @c Uninitialized.
     */
    constexpr static void check_uninitialized(
        const std::variant<Uninitialized, None, Value> &state);

    /**
     * @brief Checks if @p state is @c Value. If not, throws @c ValuelessException.
     * @param state State to check.
     * @throw ValuelessException if @p state is not in state @c Value.
     */
    constexpr static void check_value(
        const std::variant<Uninitialized, None, Value> &state);

    /**
     * Assumes that this is in state @c Value and returns the pointer.
     * @return Pointer to the current value.
     */
    T *get_value_unsafe() const;

private:
    /** @brief Holds the state of this wrapper. */
    std::variant<Uninitialized, None, Value> m_state;
};


} // astfri


#include <astfri/impl/tools/OptPtr.inl>


#endif
