#include <libastfri-cpp/inc/ClangManagement.hpp>

namespace astfri::astfri_cpp
{
// ClangConsumer::ClangConsumer(
//     clang::ASTContext& context,
//     astfri::TranslationUnit& visitedTranslationUnit
// ) :
//     context(&context),
//     visitedTranslationUnit(&visitedTranslationUnit)
// {
// }

// void ClangConsumer::HandleTranslationUnit(clang::ASTContext& p_context)
// {
//     ClangVisitor visitor(*visitedTranslationUnit);

//     visitor.TraverseDecl(p_context.getTranslationUnitDecl());
// }

// ClangTraverseAction::ClangTraverseAction(
//     astfri::TranslationUnit& visitedTranslationUnit
// ) :
//     visitedTranslationUnit(&visitedTranslationUnit)
// {
// }

// std::unique_ptr<clang::ASTConsumer> ClangTraverseAction::CreateASTConsumer(
//     clang::CompilerInstance& compiler,
//     llvm::StringRef
// )
// {
//     return std::make_unique<ClangConsumer>(
//         compiler.getASTContext(),
//         *visitedTranslationUnit
//     );
// }


CppASTConsumer::CppASTConsumer(astfri::TranslationUnit& _tu) : Visitor(_tu) {}
void CppASTConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
    this->Visitor.TraverseDecl(Context.getTranslationUnitDecl());
}    
    
CppFrontendAction::CppFrontendAction(astfri::TranslationUnit& _tu) : tu(_tu) {}
// pridane [[maybe_unused]] aby kompilator neplakal
std::unique_ptr<clang::ASTConsumer> CppFrontendAction::CreateASTConsumer([[maybe_unused]]clang::CompilerInstance &CI, [[maybe_unused]]clang::StringRef file) {
    return std::make_unique<CppASTConsumer>(tu);
}

CppFrontendActionFactory::CppFrontendActionFactory(astfri::TranslationUnit& _tu) : tu(_tu) {}
std::unique_ptr<clang::FrontendAction> CppFrontendActionFactory::create() {
    return std::make_unique<CppFrontendAction>(tu);
}


// int fill_translation_unit(TranslationUnit& tu, int argc, const char** argv) {
    
//     // Main function
//     static llvm::cl::OptionCategory MyToolCategory("my-tool options");

//     auto OptionsParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory);
//     if (!OptionsParser) {
//         llvm::errs() << OptionsParser.takeError();
//         return 1;
//     }
//     clang::tooling::ClangTool Tool(OptionsParser->getCompilations(), OptionsParser->getSourcePathList());
//     Tool.run(std::make_unique<CppFrontendActionFactory>(tu).get());
//     return 0;
// }

int fill_translation_unit(astfri::TranslationUnit& tu, const std::string& file_path) {
    // kompilacne argumenty
    std::vector<std::string> compilations = {};
    // fixna kompilacna databaza
    clang::tooling::FixedCompilationDatabase Compilations(".", compilations);
    // spustenie ClangTool
    clang::tooling::ClangTool Tool(Compilations, {file_path});
    return Tool.run(std::make_unique<CppFrontendActionFactory>(tu).get());
}

} // namespace libastfri::astfri_cpp