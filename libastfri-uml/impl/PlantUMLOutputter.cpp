#include <cstddef>
#include <cstring>
#include <libastfri-uml/inc/PlantUMLOutputter.hpp>

namespace astfri::uml {
    void PlantUMLOutputter::open(ClassStruct const& cs) {
        this->outputString_ += cs.name_;
        if (cs.genericParams_.size() > 0) {
            this->outputString_ += "<";
            size_t index = 0;
            for (std::string gp : cs.genericParams_) {
                this->outputString_ += gp;
                if (index != cs.genericParams_.size() - 1) {
                    this->outputString_ += ", ";
                }
                ++index;
            }
            this->outputString_ += ">";
        }
        this->outputString_ += " {\n";
    }

    PlantUMLOutputter::PlantUMLOutputter() {
        this->outputString_ += "@startuml\n";
    }

    void PlantUMLOutputter::write_to_file() {
        this->outputString_ += "@enduml\n";
        UMLOutputter::write_to_file();
    }

    void PlantUMLOutputter::write_to_console() {
        this->outputString_ += "@enduml\n";
        UMLOutputter::write_to_console();
    }

    std::string PlantUMLOutputter::getFileExtension() {
        return ".puml";
    }

    void PlantUMLOutputter::open_class(ClassStruct c) {
        this->outputString_ += "class ";
        this->open(c);
    }

    void PlantUMLOutputter::open_interface(ClassStruct i) {
        this->outputString_ += "interface ";
        this->open(i);
    }

    void PlantUMLOutputter::close_class() {
        this->outputString_ += "}\n";
    }

    void PlantUMLOutputter::add_data_member(VarStruct v) {
        if (v.isIndirect_) v.type_ += this->config_->indirectIndicator_;
        this->outputString_ += 
            this->config_->accessPrefix_[(int)v.accessMod_] + 
            this->config_->typeConvention_->get_string(
                v.type_,
                v.name_,
                this->config_->separator_) + "\n";
    }

    void PlantUMLOutputter::add_function_member(MethodStruct m) {
        if (m.returnIsIndirect_) m.retType_ += this->config_->indirectIndicator_;
        std::string header = m.name_ + "(";
        size_t index = 0;
        for (VarStruct p : m.params_) {
            if (p.isIndirect_) p.type_ += this->config_->indirectIndicator_;
            header += this->config_->typeConvention_->get_string(
                p.type_,
                p.name_,
                this->config_->separator_);
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

    void PlantUMLOutputter::add_relation(RelationStruct r) {
        this->outputString_ +=
            r.to_ + " " +
            this->config_->relationArrows_[(int)r.type_] +
            " " + r.from_ +
            "\n";
    }
} // namespace astfri::uml
