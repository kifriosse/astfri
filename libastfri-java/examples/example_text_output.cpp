//include for java input and making of java syntax tree
#include <libastfri-java/inc/TreeMaker.hpp>

//include for text output
#include <libastfri-text/inc/ASTVisitor.hpp>

int main()
{
    TreeMaker* tm = new TreeMaker();
    
    std::string sourceCode = tm->load_file("/home/adam/projects/astfri/libastfri-java/resources/Dummy.txt");
    TSTree* tree = tm->make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = tm->get_translation_unit(tree, sourceCode);

    ASTVisitor* visitor = new ASTVisitor();
    visitor->visit(*tu);
    visitor->write_file();

    return 0;
}