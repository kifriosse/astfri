#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/Source.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/tree-sitter-c-sharp.h>

#include <vector>

namespace astfri::csharp
{

ExprFactory& SrcCodeVisitor::exprFact_ = ExprFactory::get_instance();
StmtFactory& SrcCodeVisitor::stmtFact_ = StmtFactory::get_instance();
TypeFactory& SrcCodeVisitor::typeFact_ = TypeFactory::get_instance();

SrcCodeVisitor::SrcCodeVisitor(
    std::vector<SourceFile>& srcCodes,
    SemanticContext& semanticContext,
    SymbolTable& symbTable
) :
    typeTrs_(symbTable),
    srcCodes_(srcCodes),
    semanticContext_(semanticContext),
    lang_(tree_sitter_c_sharp())
{
}

void SrcCodeVisitor::visit_comp_unit(TranslationUnit& trUnit)
{
    for (auto& defStmt : this->semanticContext_.get_user_types())
    {
        const auto typeMetaOpt = semanticContext_.get_type_metadata(defStmt);
        if (! typeMetaOpt)
            continue;

        bool added = false;
        for (auto& [node, src] : typeMetaOpt->defs)
        {
            if (! src)
                continue;

            currentSrc_ = src;
            typeTrs_.set_current_src(src);
            StmtHandler hStmt = RegManager::get_stmt_handler(node);
            Stmt* stmt        = hStmt(this, node);
            if (added)
                continue;

            if (is_a<ClassDefStmt>(stmt))
                trUnit.classes_.push_back(as_a<ClassDefStmt>(stmt));
            else if (is_a<InterfaceDefStmt>(stmt))
                trUnit.interfaces_.push_back(as_a<InterfaceDefStmt>(stmt));

            added = true;
        }
    }
}

} // namespace astfri::csharp