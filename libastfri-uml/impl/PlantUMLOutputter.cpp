#include <cstddef>
#include <cstring>
#include <libastfri-uml/inc/PlantUMLOutputter.hpp>
#include "libastfri-uml/inc/ElementStructs.hpp"

namespace uml {
    void PlantUMLOutputter::open_class(ClassStruct c) {
        this->outputString_ += "class " + c.name_;
        if (c.genericParams_.size() > 0) {
            this->outputString_ += "<";
            size_t index = 0;
            for (std::string gp : c.genericParams_) {
                this->outputString_ += gp;
                if (index != c.genericParams_.size() - 1) {
                    this->outputString_ += ", ";
                }
                ++index;
            }
            this->outputString_ += ">";
        }
        this->outputString_ += " {\n";
    }

    void PlantUMLOutputter::close_class() {
        this->outputString_ += "}\n";
    }

    void PlantUMLOutputter::add_data_member(VarStruct v) {
        this->outputString_ += 
            this->config_->accessPrefix_[(int)v.accessMod_] + 
            this->config_->typeConvention_->get_string(v.type_, v.name_, this->config_->separator_) + "\n";
    }

    void PlantUMLOutputter::add_function_member(MethodStruct m) {
        std::string header = m.name_ + "(";
        size_t index = 0;
        for (VarStruct p : m.params_) {
            header += this->config_->typeConvention_->get_string(p.type_, p.name_, this->config_->separator_);
            if (index != m.params_.size() - 1) {
                header += ", ";
            }
            index++;
        }
        header += ")";

        this->outputString_ += 
            this->config_->accessPrefix_[(int)m.accessMod_] + 
            this->config_->typeConvention_->get_string(m.retType_, header, this->config_->separator_) + "\n";
    }
} // namespace uml