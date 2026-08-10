#include <astfri-cpp/impl/ClangManagement.hpp>


namespace astfri::cpp {


CppASTConsumer::CppASTConsumer(astfri::TranslationUnit &tu) :
    m_visitor(tu, nullptr) {
}

void CppASTConsumer::HandleTranslationUnit(clang::ASTContext& Context) {
    m_visitor.setSM(&Context.getSourceManager());
    m_visitor.setMainFileID(m_visitor.getSM()->getMainFileID());
    // std::cout << "Beginning of filling ASTFRI Translation Unit.\n";
    clang::NamespaceDecl* desired_namespace
        = m_visitor.get_desired_namespace(Context.getTranslationUnitDecl());
    if (desired_namespace) {
        m_visitor.TraverseDecl(desired_namespace);
    }
    else {
        m_visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
    // std::cout << "ASTFRI Translation Unit is filled succesfully.\n";
}


CppFrontendAction::CppFrontendAction(astfri::TranslationUnit & tu) :
    m_tu(&tu) {
}

std::unique_ptr<clang::ASTConsumer> CppFrontendAction::CreateASTConsumer(
    [[maybe_unused]] clang::CompilerInstance &CI,
    [[maybe_unused]] clang::StringRef file
) {
    return std::make_unique<CppASTConsumer>(*m_tu);
}


CppFrontendActionFactory::CppFrontendActionFactory(astfri::TranslationUnit &tu) :
    m_tu(&tu) {
}

std::unique_ptr<clang::FrontendAction> CppFrontendActionFactory::create() {
    return std::make_unique<CppFrontendAction>(*m_tu);
}


} // namespace astfri::cpp
