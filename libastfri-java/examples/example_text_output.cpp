// include for java input and making of java syntax tree
#include <libastfri-java/inc/ASTBuilder.hpp>

// include for text output
#include <libastfri-text/inc/TextLibManager.hpp>
#include <iostream>

int main()
{
    // code for java source code input and making of java syntax tree
    astfri::java::ASTBuilder* tb = new astfri::java::ASTBuilder();

    std::string sourceCode
        = tb->load_directory("/home/adam/projects/astfri/libastfri-java/resources/Structures");
    TSTree* tree                = tb->make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = tb->get_translation_unit(tree, sourceCode);

    // code for text output
    astfri::text::TextLibManager& visitor = astfri::text::TextLibManager::get_instance();

    visitor.visit_and_export(*tu);

    for (astfri::ClassDefStmt* classDef : tu->classes_)
    {
        std::cout << "Class: " << classDef->name_ << std::endl;
        for (astfri::GenericParam* tparam : classDef->tparams_)
        {
            std::cout << "  TParam: " << tparam->name_ << " " << tparam->constraint_ << std::endl;
        }
        for (astfri::ClassDefStmt* base : classDef->bases_)
        {
            std::cout << "  Base: " << base->name_ << std::endl;
        }
        for (astfri::MemberVarDefStmt* attr : classDef->vars_)
        {
            std::cout << "  Attribute: " << attr->name_ << std::endl;
        }
        for (astfri::MethodDefStmt* method : classDef->methods_)
        {
            std::cout << "  Method: " << method->func_->name_ << std::endl;
        }
        for (astfri::ConstructorDefStmt* constructor : classDef->constructors_)
        {
            std::cout << "  Constructor: " << constructor->owner_->name_ << std::endl;
        }
    }

    // freeing memory
    ts_tree_delete(tree);
    delete (tb);
    // delete (visitor);
    return 0;
}