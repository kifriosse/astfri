//include for java input and making of java syntax tree
#include <libastfri-java/inc/ASTBuilder.hpp>

//includes for uml output
#include <libastfri-uml/inc/PlantUMLOutputter.hpp>
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

int main()
{
    //code for java source code input and making of java syntax tree
    astfri::java::ASTBuilder* tb = new astfri::java::ASTBuilder();

    std::string sourceCode = tb->load_directory("/home/adam/projects/astfri/libastfri-java/resources/java_project");
    TSTree* tree = tb->make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = tb->get_translation_unit(tree, sourceCode);

    //code for uml output
    astfri::uml::Config conf;
    conf.innerView_ = true;

    astfri::uml::PlantUMLOutputter op;
    astfri::uml::TypeBeforeConvention tc;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op, tc);
    uml.run(*tu);
}