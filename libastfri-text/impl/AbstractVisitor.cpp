#include <libastfri-text/inc/AbstractVisitor.hpp>

using namespace astfri::text;

AbstractVisitor::AbstractVisitor(AbstractBuilder* const& builder) :
    builder_(builder)
{
}

void AbstractVisitor::write_condition(Expr* const&)
{
}