#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/Source.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <optional>
#include <vector>

namespace astfri::csharp
{

ExprFactory& SrcCodeVisitor::expr_factory_ = ExprFactory::get_instance();
StmtFactory& SrcCodeVisitor::stmt_factory_ = StmtFactory::get_instance();
TypeFactory& SrcCodeVisitor::type_factory_ = TypeFactory::get_instance();

SrcCodeVisitor::SrcCodeVisitor(
    std::vector<SourceCode>& source_codes,
    SemanticContext& semantic_context
) :
    src_codes(source_codes),
    semantic_context_(semantic_context)
{
}

void SrcCodeVisitor::handle_comp_unit_stmt(TranslationUnit& tr_unit)
{
    for (auto& def_stmt : this->semantic_context_.get_user_types())
    {
        std::optional<TypeMetadata> type_metadata
            = this->semantic_context_.get_type_metadata(def_stmt);
        if (! type_metadata)
            continue;

        bool added = false;
        for (auto& [node, src] : type_metadata->defs)
        {
            if (! src)
                continue;
            this->current_src   = src;
            StmtHandler handler = RegManager::get_stmt_handler(node);
            Stmt* stmt          = handler(this, &node);
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