#include "version.hpp"


namespace astfri {


void print_version(std::ostream &ost) {
    ost << "astfri " << ASTFRI_VERSION << "\n"
        << "  libastfri-core      " << ASTFRI_CORE_VERSION      << "\n"
        << "  libastfri-cpp       " << ASTFRI_CPP_VERSION       << "\n"
        << "  libastfri-cs        " << ASTFRI_CS_VERSION        << "\n"
        << "  libastfri-java      " << ASTFRI_JAVA_VERSION      << "\n"
        << "  libastfri-serialize " << ASTFRI_SERIALIZE_VERSION << "\n"
        << "  libastfri-text      " << ASTFRI_TEXT_VERSION      << "\n"
        << "  libastfri-uml       " << ASTFRI_UML_VERSION       << "\n";
}


} // namespace astfri
