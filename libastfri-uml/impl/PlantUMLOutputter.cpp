#include <libastfri-uml/impl/PlantUMLOutputter.hpp>

#include <cstddef>
#include <cstring>

namespace astfri::uml
{
std::string PlantUMLOutputter::assemble_param(VarStruct p)
{
    std::string result;

    if (p.isIndirect_)
        p.type_ += this->config_->indirectIndicator_;
    result += TypeConvention::get_string(
            p.type_,
            p.name_,
            this->config_->separator_,
            this->config_->typeConvention_);

    return result;
}

void PlantUMLOutputter::open(ClassStruct const& cs)
{
    if (this->config_->handleNamespaces_ && !cs.namespace_.empty())
    {
        this->outputString_ += cs.namespace_;
    }
    else
    {
        this->outputString_ += cs.name_;
    }
    if (cs.genericParams_.size() > 0)
    {
        this->outputString_ += "<";
        size_t index = 0;
        for (std::string gp : cs.genericParams_)
        {
            this->outputString_ += gp;
            if (index != cs.genericParams_.size() - 1)
            {
                this->outputString_ += ", ";
            }
            ++index;
        }
        this->outputString_ += ">";
    }
    this->outputString_ += " {\n";
}

void PlantUMLOutputter::apply_style_from_config()
{
    std::string style = "<style>\n";
    style += "classDiagram {\nBackGroundColor " + this->config_->diagramBG_ + "\n}\n";
    style += "class {\nBackGroundColor " + this->config_->elementBG_ + "\n";
    style += "LineColor " + this->config_->elementBorder_ + "\n";
    style += "FontColor " + this->config_->fontColor_ + "\n" + "}\n";
    style += "arrow {\nLineColor " + this->config_->arrowColor_ + "\n}\n";
    style += "</style>\n";
    if (this->config_->handleNamespaces_)
    {
        style += "set separator " + this->config_->namespaceSeparator_ + "\n";
    }
    if (! this->config_->drawAccessModIcons_)
        style += "skinparam classAttributeIconSize 0\n";
    this->outputString_ = style + this->outputString_;
}

void PlantUMLOutputter::add_tags_and_style()
{
    this->outputString_ += "@enduml\n";
    this->apply_style_from_config();
    this->outputString_ = "@startuml\n" + this->outputString_;
}

void PlantUMLOutputter::write_to_file()
{
    this->add_tags_and_style();
    UMLOutputter::write_to_file();
}

void PlantUMLOutputter::write_to_console()
{
    this->add_tags_and_style();
    UMLOutputter::write_to_console();
}

std::string PlantUMLOutputter::getFileExtension()
{
    return ".puml";
}

void PlantUMLOutputter::open_user_type(ClassStruct c)
{
    switch (c.type_)
    {
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

void PlantUMLOutputter::close_user_type()
{
    this->outputString_ += "}\n";
}

void PlantUMLOutputter::add_data_member(VarStruct v)
{
    if (v.isIndirect_)
        v.type_ += this->config_->indirectIndicator_;
    this->outputString_
        += this->config_->accessPrefix_[(int)v.accessMod_]
         + TypeConvention::get_string(
                 v.type_,
                 v.name_,
                 this->config_->separator_,
                 this->config_->typeConvention_)
         + "\n";
}

void PlantUMLOutputter::add_function_member(MethodStruct m)
{
    if (m.returnIsIndirect_)
        m.retType_ += this->config_->indirectIndicator_;
    std::string header = m.name_ + "(";
    size_t index       = 0;
    for (VarStruct p : m.params_)
    {
        header += this->assemble_param(p);
        if (index != m.params_.size() - 1)
        {
            header += ", ";
        }
        index++;
    }
    header += ")";

    this->outputString_
        += this->config_->accessPrefix_[(int)m.accessMod_]
         + TypeConvention::get_string(
                 m.retType_,
                 header,
                 this->config_->separator_,
                 this->config_->typeConvention_)
         + "\n";
}

void PlantUMLOutputter::add_constructor(ConstructorStruct c)
{
    std::string header;
    if (config_->innerView_)
    {
        header += c.class_ + "(";
    }
    else
    {
        if (c.accessMod_ != AccessModifier::Public) return;
        header += "{static}<<constructor>> new(";
    }
    size_t index = 0;
    for (VarStruct p : c.params_)
    {
        header += this->assemble_param(p);
        if (index != c.params_.size() - 1) header += ", ";
        ++index;
    }
    header += ")";

    this->outputString_ +=
        this->config_->accessPrefix_[(int)c.accessMod_] +
        TypeConvention::get_string(
                c.class_,
                header,
                this->config_->separator_,
                this->config_->typeConvention_)
        + "\n";
}

void PlantUMLOutputter::add_destructor(DestructorStruct d)
{
    std::string header = "";
    header += this->config_->accessPrefix_[(int)AccessModifier::Public];
    header += this->config_->destructorIndicator_;
    header += d.class_;
    header += "()\n";
    this->outputString_ += header;
}

void PlantUMLOutputter::add_relation(RelationStruct r)
{
    this->outputString_
        += r.to_ + " " + this->config_->relationArrows_[(int)r.type_] + " " + r.from_ + "\n";
}
} // namespace astfri::uml
