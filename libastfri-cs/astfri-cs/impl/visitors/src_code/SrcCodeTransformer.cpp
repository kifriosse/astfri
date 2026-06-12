#include <astfri/Astfri.hpp>

#include <astfri-cs/impl/CSFwd.hpp>
#include <astfri-cs/impl/data/Metadata.hpp>
#include <astfri-cs/impl/data/SourceFile.hpp>
#include <astfri-cs/impl/regs/Maps.hpp>
#include <astfri-cs/impl/SemanticContext.hpp>
#include <astfri-cs/impl/visitors/src_code/SrcCodeTransformer.hpp>

#include <tree_sitter/tree-sitter-c-sharp.h>

#include <vector>

namespace astfri::csharp {

ExprFactory& SrcCodeTransformer::exprFact_        = ExprFactory::get_instance();
StmtFactory& SrcCodeTransformer::stmtFact_        = StmtFactory::get_instance();
TypeFactory& SrcCodeTransformer::typeFact_        = TypeFactory::get_instance();
maps::MapManager& SrcCodeTransformer::mapManager_ = maps::MapManager::get();

SrcCodeTransformer::SrcCodeTransformer(SymbolTable& symbTable) :
    typeTrs_(symbTable),
    semContext_(symbTable),
    lang_(tree_sitter_c_sharp()) {
}

TranslationUnit* SrcCodeTransformer::visit_comp_unit() {
    TranslationUnit* trUnit = stmtFact_.mk_translation_unit();
    for (const auto metadata : this->semContext_.get_type_metadata()) {
        typeTrs_.set_current_namespace(metadata->type_binding().treeNode);
        bool added = false;
        for (auto& [node, src] : metadata->defs()) {
            if (! src)
                continue;

            currentSrc_ = src;
            typeTrs_.set_current_src(src);
            StmtMapper mStmt = mapManager_.get_stmt_mapper(node);
            Stmt* stmt       = mStmt(this, node);
            if (added)
                continue;

            if (is<ClassDefStmt>(stmt))
                trUnit->classes.push_back(as<ClassDefStmt>(stmt));
            else if (is<InterfaceDefStmt>(stmt))
                trUnit->interfaces.push_back(as<InterfaceDefStmt>(stmt));

            added = true;
        }
    }
    return trUnit;
}

} // namespace astfri::csharp