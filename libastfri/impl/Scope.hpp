#ifndef LIBASTFRI_SCOPE_HPP
#define LIBASTFRI_SCOPE_HPP

#include <string>
#include <string_view>
#include <vector>

namespace astfri {

/**
 * @brief TODO
 */
struct Scope
{
    std::vector<std::string> names_;
};

/**
 * @brief TODO
 */
template<typename... String>
Scope mk_scope(String... parts);

/**
 * @brief Makes _unique_ fully qualified name.
 * The name is used for an internal bookeeping.
 * It is not intended for display purposes.
 */
std::string mk_fqn(const Scope &scope, std::string_view name);

} // namespace astfri

#include <libastfri/impl/Scope.inl>

#endif
