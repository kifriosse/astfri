#include <libastfri-cs/impl/CSharpTSTreeVisitor.hpp>

namespace astfri::csharp
{

Stmt* CSharpTSTreeVisitor::handle_decl_list_stmt(CSharpTSTreeVisitor* self, TSNode const* node)
{
    return stmt_factory_.mk_compound({});
}

Stmt* CSharpTSTreeVisitor::handle_arrow_expr_clause(CSharpTSTreeVisitor* self, TSNode const* node)
{
    return stmt_factory_.mk_compound({});
}

} // namespace astfri::csharp
