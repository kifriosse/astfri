#include <astfri-uml/impl/PlantUMLOutputter.hpp>

#include <cstddef>
#include <cstring>

namespace astfri::uml {
std::string PlantUMLOutputter::assemble_param(VarStruct p) {
    std::string result;

    if (p.isIndirect_)
        p.type_ += this->config_->indirectIndicator;
    result += TypeConvention::get_string(
        p.type_,
        p.name_,
        this->config_->separator,
        this->config_->typeConvention
    );

    return result;
}

void PlantUMLOutputter::open(const ClassStruct& cs) {
    if (this->config_->handleNamespaces && ! cs.namespace_.empty()) {
        this->outputString_ += cs.namespace_;
    }
    else {
        this->outputString_ += cs.name_;
    }
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

void PlantUMLOutputter::apply_style_from_config() {
    std::string style = "<style>\n";
    style += "classDiagram {\nBackGroundColor " + this->config_->bgDiagram + "\n}\n";
    style += "class {\nBackGroundColor " + this->config_->bgElement + "\n";
    style += "LineColor " + this->config_->elementBorder + "\n";
    style += "FontColor " + this->config_->fontColor + "\n" + "}\n";
    style += "arrow {\nLineColor " + this->config_->arrowColor + "\n}\n";
    style += "</style>\n";
    if (this->config_->handleNamespaces) {
        style += "set separator " + this->config_->namespaceSeparator + "\n";
    }
    if (! this->config_->drawIcons)
        style += "skinparam classAttributeIconSize 0\n";
    this->outputString_ = style + this->outputString_;
}

void PlantUMLOutputter::add_tags_and_style() {
    this->outputString_ += "@enduml\n";
    this->apply_style_from_config();
    this->outputString_ = "@startuml\n" + this->outputString_;
}

void PlantUMLOutputter::write_to_file() {
    this->add_tags_and_style();
    UMLOutputter::write_to_file();
}

void PlantUMLOutputter::write_to_console() {
    this->add_tags_and_style();
    UMLOutputter::write_to_console();
}

std::string PlantUMLOutputter::getFileExtension() {
    return ".puml";
}

void PlantUMLOutputter::open_user_type(ClassStruct c) {
    switch (c.type_) {
    case UserDefinedType::CLASS:
        this->outputString_ += "class ";
        break;
    case UserDefinedType::STRUCT:
        this->outputString_ += "struct ";
        break;
    case UserDefinedType::INTERFACE:
        this->outputString_ += "interface ";
        break;
    case UserDefinedType::ENUM:
        this->outputString_ += "enum ";
        break;
    default:
        break;
    }
    this->open(c);
}

void PlantUMLOutputter::close_user_type() {
    this->outputString_ += "}\n";
}

void PlantUMLOutputter::add_data_member(VarStruct v) {
    if (v.isIndirect_)
        v.type_ += this->config_->indirectIndicator;
    this->outputString_ += this->access_mod_to_char(v.accessMod_)
                         + TypeConvention::get_string(
                               v.type_,
                               v.name_,
                               this->config_->separator,
                               this->config_->typeConvention
                         )
                         + "\n";
}

void PlantUMLOutputter::add_function_member(MethodStruct m) {
    if (m.returnIsIndirect_)
        m.retType_ += this->config_->indirectIndicator;
    std::string header = m.name_ + "(";
    size_t index       = 0;
    for (VarStruct p : m.params_) {
        header += this->assemble_param(p);
        if (index != m.params_.size() - 1) {
            header += ", ";
        }
        index++;
    }
    header += ")";

    this->outputString_ += this->access_mod_to_char(m.accessMod_)
                         + TypeConvention::get_string(
                               m.retType_,
                               header,
                               this->config_->separator,
                               this->config_->typeConvention
                         )
                         + "\n";
}

void PlantUMLOutputter::add_constructor(ConstructorStruct c) {
    std::string header;
    if (config_->innerView) {
        header += c.class_ + "(";
    }
    else {
        if (c.accessMod_ != AccessModifier::Public)
            return;
        header += "{static}<<constructor>> new(";
    }
    size_t index = 0;
    for (VarStruct p : c.params_) {
        header += this->assemble_param(p);
        if (index != c.params_.size() - 1)
            header += ", ";
        ++index;
    }
    header += ")";

    this->outputString_ += this->access_mod_to_char(c.accessMod_)
                         + TypeConvention::get_string(
                               c.class_,
                               header,
                               this->config_->separator,
                               this->config_->typeConvention
                         )
                         + "\n";
}

void PlantUMLOutputter::add_destructor(DestructorStruct d) {
    std::string header = "";
    header += this->access_mod_to_char(AccessModifier::Public);
    header += this->config_->destructorIndicator;
    header += d.class_;
    header += "()\n";
    this->outputString_ += header;
}

void PlantUMLOutputter::add_relation(RelationStruct r) {
    this->outputString_
        += r.to_ + " " + this->relation_to_arrow(r.type_) + " " + r.from_ + "\n";
}

char PlantUMLOutputter::access_mod_to_char(AccessModifier am) const {
    switch (am) {
        case AccessModifier::Public:
            return config_->publicPrefix;
        case AccessModifier::Private:
            return config_->privatePrefix;
        case AccessModifier::Protected:
            return config_->protectedPrefix;
        default:
            throw std::runtime_error(
                "Unhandled enum value in `PlantUMLOutputter::access_mod_to_char`.");
    }
}

std::string PlantUMLOutputter::relation_to_arrow(RelationType rt) const {
    switch (rt) {
        case RelationType::ASSOCIATION:
            return config_->association;
        case RelationType::COMPOSITION:
            return config_->composition;
        case RelationType::EXTENSION:
            return config_->extension;
        case RelationType::IMPLEMENTATION:
            return config_->implementation;
        default:
            throw std::runtime_error(
                "Unhandled enum value in `PlantUMLOutputter::relation_to_arrow`.");
    }
}


} // namespace astfri::uml
