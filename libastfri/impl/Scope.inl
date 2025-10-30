#include <libastfri/impl/Scope.hpp>

namespace astfri {

template<typename... String>
Scope mk_scope(String... parts) {
    Scope s;
    (s.names_.push_back(std::string(std::move(parts))), ...);
    return s;
}

} // namespace astfri
