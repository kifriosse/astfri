#ifndef LIBASTFRIUML_UML_LIB_WRAPPER_HPP
#define LIBASTFRIUML_UML_LIB_WRAPPER_HPP

#include <libastfri-uml/impl/ClassVisitor.hpp>
#include <libastfri-uml/impl/PlantUMLOutputter.hpp>

namespace astfri::uml {
class UMLLibWrapper {
private:
    ClassVisitor classVisitor_;

public:
    void init(Config& config, UMLOutputter& output);
    void run(astfri::TranslationUnit& translationUnit);
};


} // namespace astfri::uml

//namespace astfri
//{
//    struct uml_out
//    {
//        void process_ast(uml::Config cfg, const astfri::TranslationUnit& root);
//        void process_ast(uml::Config cfg, const astfri::TranslationUnit& root, std::ostream& ost);
//    };
//
//    static_assert(IsOutputLibInterface<uml_out, uml::Config, rapidjson::Value>);
//}
#endif
