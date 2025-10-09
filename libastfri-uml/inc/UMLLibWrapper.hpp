#ifndef LIBASTFRIUML_UML_LIB_WRAPPER_HPP
#define LIBASTFRIUML_UML_LIB_WRAPPER_HPP

#include <libastfri-uml/impl/ClassVisitor.hpp>
#include <libastfri-uml/impl/PlantUMLOutputter.hpp>

namespace astfri::uml
{
class UMLLibWrapper
{
private:
    ClassVisitor classVisitor_;

public:
    void init(Config& config, UMLOutputter& output, TypeConvention const& typeConvention);
    void run(astfri::TranslationUnit& translationUnit);
};
} // namespace astfri::uml

#endif
