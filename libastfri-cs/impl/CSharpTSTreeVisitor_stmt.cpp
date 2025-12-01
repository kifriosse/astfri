#include <libastfri-cs/impl/CSharpTSTreeVisitor.hpp>

namespace astfri::csharp
{

Stmt* CSharpTSTreeVisitor::handle_block_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    return stmt_factory_.mk_compound({});
}

Stmt* CSharpTSTreeVisitor::handle_arrow_expr_clause(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    return stmt_factory_.mk_compound({});
}

} // namespace astfri::csharp
