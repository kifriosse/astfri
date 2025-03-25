//include for java input and making of java syntax tree
#include <libastfri-java/inc/TreeMaker.hpp>

//includes for uml output
#include <libastfri-uml/inc/PlantUMLOutputter.hpp>
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

int main()
{
    TreeMaker* tm = new TreeMaker();

    std::string sourceCode = tm->load_directory("/home/adam/projects/astfri/libastfri-java/resources/java_project");
    TSTree* tree = tm->make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = tm->get_translation_unit(tree, sourceCode);

    astfri::uml::Config conf;
    conf.innerView_ = true;

    astfri::uml::PlantUMLOutputter op;
    astfri::uml::TypeBeforeConvention tc;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op, tc);
    uml.run(*tu);
}