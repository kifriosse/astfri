#include <cstring>
#include <libastfri-uml/inc/PlantUMLOutputter.hpp>
#include "libastfri-uml/inc/Config.hpp"
#include "libastfri-uml/inc/ElementStructs.hpp"

namespace uml {
    void PlantUMLOutputter::open_class(ClassStruct c) {
        this->outputString_ += "class " + c.name_;
        if (strcmp(c.genericParam_.c_str(), "") != 0) this->outputString_ += "<" + c.genericParam_ + ">";
        this->outputString_ += " {\n";
    }

    void PlantUMLOutputter::close_class() {
        this->outputString_ += "}\n";
    }

    void PlantUMLOutputter::add_data_member(VarStruct v) {
        // TODO - access modifier (also in visitor)
        if (this->config_->typeConvention_ == TypeConvention::TYPE_AFTER_NAME) {
            this->outputString_ += v.name_ + " : " + v.type_+ "\n";
        } else {
            this->outputString_ += v.type_ + " " + v.name_ + "\n";
        }
    }
} // namespace uml