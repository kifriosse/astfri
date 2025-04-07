#include <libastfri-uml/inc/UMLOutputter.hpp>

namespace astfri::uml {
    void UMLOutputter::set_config(Config const& config) {
        this->config_ = (Config*)&config;
    }

    void UMLOutputter::write_to_console() {
        std::cout << "UMLOutputter - diagram output:" << std::endl;
        std::cout << this->outputString_;
    }

    void UMLOutputter::write_to_file() {
        std::string path = this->config_->outputFilePath_ + this->getFileExtension();
        FILE* fhandle = fopen(path.c_str(), "wt");
        if (fhandle) {
            fprintf(fhandle, "%s", this->outputString_.c_str());
            std::cout << "UMLOutputter - Successfully written to: " << path << std::endl;
        } else {
            std::cout << "UMLOutputter - Could not write into file" << std::endl;
            // TODO - throw
        }
        fclose(fhandle);
    }

} // namespace astfri::uml
