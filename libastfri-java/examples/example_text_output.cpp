//include for java input and making of java syntax tree
#include <libastfri-java/inc/JavaSyntaxTreeBuilder.hpp>

//include for text output
#include <libastfri-text/inc/ASTVisitor.hpp>

int main()
{
    //code for java source code input and making of java syntax tree
    JavaSyntaxTreeBuilder* tb = new JavaSyntaxTreeBuilder();
    
    std::string sourceCode = tb->load_file("/home/adam/projects/astfri/libastfri-java/resources/Dummy.txt");
    TSTree* tree = tb->make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = tb->get_translation_unit(tree, sourceCode);

    //code for text output
    ASTVisitor* visitor = new ASTVisitor();
    visitor->visit(*tu);
    visitor->write_file();

    return 0;
}