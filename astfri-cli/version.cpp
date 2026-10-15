#include "version.hpp"


namespace astfri {


void print_version(std::ostream &ost) {
    ost << "astfri " << ASTFRI_VERSION << "\n";
}


} // namespace astfri
