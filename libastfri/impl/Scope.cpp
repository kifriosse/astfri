#include <libastfri/impl/Scope.hpp>

namespace astfri {

std::string mk_fqn(const Scope &scope, std::string_view name) {
    std::string fqn;
    for (const std::string &s : scope.names_) {
      fqn += s;
      fqn += "::";
    }
    fqn += name;
    return fqn;
}

} // namespace astfri
