#include <astfri-cpp/impl/visitor-methods/ClangVisitor.hpp>


namespace astfri::cpp {


ClangVisitor::ClangVisitor(TranslationUnit &visitedTranslationUnit, clang::SourceManager *pSM) :
    m_SM(pSM),
    m_tu(&visitedTranslationUnit),
    m_stmt_factory(&StmtFactory::get_instance()),
    m_expr_factory(&ExprFactory::get_instance()),
    m_type_factory(&TypeFactory::get_instance()),
    m_expr_as_stmt(false) {
}

bool ClangVisitor::isInMainFile(clang::SourceLocation Loc) const {
    if (Loc.isInvalid())
        return false;
    return m_SM->getFileID(m_SM->getExpansionLoc(Loc)) == m_MainFileID;
}

clang::NamespaceDecl* ClangVisitor::get_desired_namespace(clang::TranslationUnitDecl* TU) {
    for (auto decl : TU->decls()) {
        if (auto NSD = llvm::dyn_cast<clang::NamespaceDecl>(decl)) {
            if (this->isInMainFile(NSD->getLocation())) {
                return NSD;
            }
        }
    }
    return nullptr;
}

void ClangVisitor::setSM(clang::SourceManager* pSM) {
    m_SM = pSM;
}

clang::SourceManager* ClangVisitor::getSM() {
    return m_SM;
}

void ClangVisitor::setMainFileID(clang::FileID MFID) {
    m_MainFileID = MFID;
}

clang::FileID ClangVisitor::getMFID() {
    return m_MainFileID;
}


} // namespace astfri::cpp
