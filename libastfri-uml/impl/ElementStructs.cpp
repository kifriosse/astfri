#include <libastfri-uml/inc/ElementStructs.hpp>

namespace astfri::uml {
    void ClassStruct::reset() {
        this->name_ = "";
        this->genericParams_.clear();
    }

    void VarStruct::reset() {
        this->name_ = "";
        this->type_ = "";
        this->init_ = "";
        this->isIndirect_ = false;
        this->accessMod_ = astfri::AccessModifier::Internal;
    }

    void MethodStruct::reset() {
        this->name_ = "";
        this->retType_ = "";
        this->returnIsIndirect_ = false;
        this->params_.clear();
        this->accessMod_ = astfri::AccessModifier::Internal;
    }
} // namespace astfri::uml