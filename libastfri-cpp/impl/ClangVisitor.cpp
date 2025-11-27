#include <libastfri-cpp/inc/ClangVisitor.hpp>

namespace astfri::astfri_cpp
{
bool ClangVisitor::isInMainFile(clang::SourceLocation Loc) const
{
    if (Loc.isInvalid())
        return false;
    return this->SM->getFileID(this->SM->getExpansionLoc(Loc)) == this->MainFileID;
}

clang::NamespaceDecl* ClangVisitor::get_desired_namespace(clang::TranslationUnitDecl* TU)
{
    for (auto decl : TU->decls())
    {
        if (auto NSD = llvm::dyn_cast<clang::NamespaceDecl>(decl))
        {
            if (this->isInMainFile(NSD->getLocation()))
            {
                return NSD;
            }
        }
    }
    return nullptr;
}

void ClangVisitor::setSM(clang::SourceManager* pSM)
{
    this->SM = pSM;
}

clang::SourceManager* ClangVisitor::getSM()
{
    return this->SM;
}

void ClangVisitor::setMainFileID(clang::FileID MFID)
{
    this->MainFileID = MFID;
}

clang::FileID ClangVisitor::getMFID()
{
    return this->MainFileID;
}

ClangVisitor::ClangVisitor(TranslationUnit& visitedTranslationUnit, clang::SourceManager* pSM) :
    tu_(&visitedTranslationUnit)
{
    this->SM            = pSM;
    this->stmt_factory_ = &StmtFactory::get_instance();
    this->expr_factory_ = &ExprFactory::get_instance();
    this->type_factory_ = &TypeFactory::get_instance();
    this->expr_as_stmt  = false;
}
} // namespace astfri::astfri_cpp