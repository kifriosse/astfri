// include for java input and making of java syntax tree
#include <libastfri-java/inc/ASTBuilder.hpp>

// include for text output
#include <libastfri-text/inc/TextLibManager.hpp>

int main()
{
    // code for java source code input and making of java syntax tree
    astfri::java::ASTBuilder* tb = new astfri::java::ASTBuilder();

    std::string sourceCode
        = tb->load_file("/home/adam/projects/astfri/libastfri-java/resources/Dummy.txt");
    TSTree* tree                = tb->make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = tb->get_translation_unit(tree, sourceCode);

    // code for text output
    astfri::text::TextLibManager& visitor = astfri::text::TextLibManager::get_instance();
    visitor.visit(*tu);
    // visitor->write_file();

    // freeing memory
    ts_tree_delete(tree);
    delete (tb);
    // delete (visitor);
    return 0;
}