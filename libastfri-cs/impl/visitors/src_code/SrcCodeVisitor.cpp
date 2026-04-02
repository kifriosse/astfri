#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/visitors/src_code/SrcCodeVisitor.hpp>
#include <astfri/Astfri.hpp>

#include <tree_sitter/tree-sitter-c-sharp.h>

#include <vector>

namespace astfri::csharp {

ExprFactory& SrcCodeVisitor::exprFact_ = ExprFactory::get_instance();
StmtFactory& SrcCodeVisitor::stmtFact_ = StmtFactory::get_instance();
TypeFactory& SrcCodeVisitor::typeFact_ = TypeFactory::get_instance();

SrcCodeVisitor::SrcCodeVisitor(SemanticContext& semanticContext, SymbolTable& symbTable) :
    typeTrs_(symbTable),
    semanticContext_(semanticContext),
    lang_(tree_sitter_c_sharp()) {
}

void SrcCodeVisitor::visit_comp_unit(TranslationUnit& trUnit) {
    for (const auto metadata : this->semanticContext_.get_type_metadata()) {
        typeTrs_.set_current_namespace(metadata->type_binding().treeNode);
        bool added = false;
        for (auto& [node, src] : metadata->defs()) {
            if (! src)
                continue;

            currentSrc_ = src;
            typeTrs_.set_current_src(src);
            StmtMapper hStmt = MapManager::get_stmt_mapper(node);
            Stmt* stmt       = hStmt(this, node);
            if (added)
                continue;

            if (is_a<ClassDefStmt>(stmt))
                trUnit.classes.push_back(as_a<ClassDefStmt>(stmt));
            else if (is_a<InterfaceDefStmt>(stmt))
                trUnit.interfaces.push_back(as_a<InterfaceDefStmt>(stmt));

            added = true;
        }
    }
}

} // namespace astfri::csharp