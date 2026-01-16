#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/Source.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/tree-sitter-c-sharp.h>

#include <optional>
#include <vector>

namespace astfri::csharp
{

ExprFactory& SrcCodeVisitor::expr_f_ = ExprFactory::get_instance();
StmtFactory& SrcCodeVisitor::stmt_f_ = StmtFactory::get_instance();
TypeFactory& SrcCodeVisitor::type_f_ = TypeFactory::get_instance();

SrcCodeVisitor::SrcCodeVisitor(
    std::vector<SourceCode>& source_codes,
    SemanticContext& semantic_context,
    SymbolTable& symbol_table
) :
    type_tr_(symbol_table),
    src_codes_(source_codes),
    semantic_context_(semantic_context),
    lang_(tree_sitter_c_sharp())
{
}

void SrcCodeVisitor::visit_comp_unit(TranslationUnit& tr_unit)
{
    for (auto& def_stmt : this->semantic_context_.get_user_types())
    {
        const auto type_meta_opt
            = semantic_context_.get_type_metadata(def_stmt);
        if (! type_meta_opt)
            continue;

        bool added = false;
        for (auto& [node, src] : type_meta_opt->defs)
        {
            if (! src)
                continue;

            current_src_ = src;
            type_tr_.set_current_src(src);
            StmtHandler handler = RegManager::get_stmt_handler(node);
            Stmt* stmt          = handler(this, node);
            if (added)
                continue;

            if (is_a<ClassDefStmt>(stmt))
                tr_unit.classes_.push_back(as_a<ClassDefStmt>(stmt));
            else if (is_a<InterfaceDefStmt>(stmt))
                tr_unit.interfaces_.push_back(as_a<InterfaceDefStmt>(stmt));

            added = true;
        }
    }
}

} // namespace astfri::csharp