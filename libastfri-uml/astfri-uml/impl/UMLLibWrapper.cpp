#include <astfri-uml/UMLLibWrapper.hpp>

namespace astfri::uml {
void UMLLibWrapper::init(Config& config, UMLOutputter& output) {
    output.set_config(config);
    this->classVisitor_.set_config(config);
    this->classVisitor_.set_outputter(output);
}

void UMLLibWrapper::run(astfri::TranslationUnit& translationUnit) {
    translationUnit.accept(this->classVisitor_);
}
} // namespace astfri::uml


namespace astfri {


std::string_view astfri::uml_out::version() {
    return ASTFRI_UML_VERSION;
}


} // namespace astfri
