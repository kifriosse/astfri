// include for java input and making of java syntax tree
#include <astfri-java/ASTBuilder.hpp>

// includes for uml output
#include <astfri-uml/UMLLibWrapper.hpp>

int main() {
    // code for java source code input and making of java syntax tree
    astfri::java::ASTBuilder* tb = new astfri::java::ASTBuilder();

    std::string sourceCode
        = tb->load_file("/home/adamk/projects/astfri/resources/samples/java/Main.java");
    TSTree* tree                = tb->make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = tb->get_translation_unit(tree, sourceCode);

    // code for uml output
    astfri::uml::Config conf = astfri::uml::Config::createDefault();
    conf.innerView = true;

    astfri::uml::PlantUMLOutputter op;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op);
    uml.run(*tu);

    // freeing memory
    ts_tree_delete(tree);
    delete (tb);
    return 0;
}