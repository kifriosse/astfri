#include <libastfri-uml/inc/UMLOutputter.hpp>

namespace uml {
    void UMLOutputter::set_config(Config* config) {
        this->config_ = config;
    }

    void UMLOutputter::write_to_console() {
        std::cout << this->outputString_;
    }

    void UMLOutputter::write_to_file() {
        // TODO
    }

} // namespace uml