#include <libastfri-text/inc/AbstractVisitor.hpp>

using namespace astfri::text;

AbstractVisitor::AbstractVisitor(AbstractBuilder*& builder) :
    builder_(builder)
{
}

void AbstractVisitor::write_condition(Expr* const&)
{
}