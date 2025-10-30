#include <libastfri-uml/impl/ElementStructs.hpp>

namespace astfri::uml
{
void ClassStruct::reset()
{
    this->name_ = "";
    this->genericParams_.clear();
    this->type_ = UserDefinedType::CLASS;
}

void VarStruct::reset()
{
    this->name_       = "";
    this->type_       = "";
    this->init_       = "";
    this->isIndirect_ = false;
    this->accessMod_  = astfri::AccessModifier::Internal;
}

void MethodStruct::reset()
{
    this->name_             = "";
    this->retType_          = "";
    this->returnIsIndirect_ = false;
    this->params_.clear();
    this->accessMod_ = astfri::AccessModifier::Internal;
}

void ConstructorStruct::reset()
{
    this->class_ = "";
    this->accessMod_ = astfri::AccessModifier::Internal;
    this->params_.clear();
}

void DestructorStruct::reset()
{
    this->class_ = "";
}
} // namespace astfri::uml
