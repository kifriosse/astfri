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
        this->outputString_ += this->config_->typeConvention_->get_string(v.type_, v.name_, this->config_->separator_) + "\n";
    }

    void PlantUMLOutputter::add_function_member(MethodStruct m) {
        // TODO - access modifier (also in visitor)
        std::string header = 
            m.name_ +
            "(" +
            this->config_->typeConvention_->get_string(m.param_.type_, m.param_.name_, this->config_->separator_) +
            ")"
        ;
        this->outputString_ += this->config_->typeConvention_->get_string(m.retType_, header, this->config_->separator_) + "\n";
    }
} // namespace uml