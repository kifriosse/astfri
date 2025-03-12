
#include <libastfri-uml/inc/UMLLibWrapper.hpp>
#include "libastfri-uml/inc/TypeConvention.hpp"

namespace astfri::uml {
    void UMLLibWrapper::init(Config& config, UMLOutputter& output, TypeConvention const& typeConvention) {
        config.typeConvention_ = (TypeConvention*)&typeConvention;
        output.set_config(config);
        this->classVisitor_.set_config(config);
        this->classVisitor_.set_outputter(output);
    }
    void UMLLibWrapper::run(astfri::TranslationUnit& translationUnit) {
        translationUnit.accept(this->classVisitor_);
        this->classVisitor_.finish();
    }
} // namespace astfri::uml