#include <libastfri-cpp/inc/ClangManagement.hpp>

#include <iostream>

namespace astfri::astfri_cpp
{
CppASTConsumer::CppASTConsumer(astfri::TranslationUnit& _tu) :
    Visitor(_tu, nullptr)
{
}

void CppASTConsumer::HandleTranslationUnit(clang::ASTContext& Context)
{
    this->Visitor.setSM(&Context.getSourceManager());
    this->Visitor.setMainFileID(this->Visitor.getSM()->getMainFileID());
    // std::cout << "Beginning of filling ASTFRI Translation Unit.\n";
    clang::NamespaceDecl* desired_namespace
        = this->Visitor.get_desired_namespace(Context.getTranslationUnitDecl());
    if (desired_namespace)
    {
        this->Visitor.TraverseDecl(desired_namespace);
    }
    else
    {
        this->Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
    // std::cout << "ASTFRI Translation Unit is filled succesfully.\n";
}

CppFrontendAction::CppFrontendAction(astfri::TranslationUnit& _tu) :
    tu(_tu)
{
}

// pridane [[maybe_unused]] aby kompilator neplakal
std::unique_ptr<clang::ASTConsumer> CppFrontendAction::CreateASTConsumer(
    [[maybe_unused]] clang::CompilerInstance& CI,
    [[maybe_unused]] clang::StringRef file
)
{
    return std::make_unique<CppASTConsumer>(tu);
}

CppFrontendActionFactory::CppFrontendActionFactory(astfri::TranslationUnit& _tu) :
    tu(_tu)
{
}

std::unique_ptr<clang::FrontendAction> CppFrontendActionFactory::create()
{
    return std::make_unique<CppFrontendAction>(tu);
}

int fill_translation_unit(astfri::TranslationUnit& tu, std::string const& file_path)
{
    // kompilacne argumenty
    std::vector<std::string> compilations = {}; // {"-nostdinc", "-nostdinc++"};
    // fixna kompilacna databaza
    clang::tooling::FixedCompilationDatabase Compilations(".", compilations);
    // spustenie ClangTool
    clang::tooling::ClangTool Tool(Compilations, {file_path});
    return Tool.run(std::make_unique<CppFrontendActionFactory>(tu).get());

    return 0;
}

} // namespace astfri::astfri_cpp