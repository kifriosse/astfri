#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

using namespace astfri::text;

AbstractCodeVisitor::AbstractCodeVisitor(AbstractCodeBuilder* const& builder) :
    AbstractVisitor(builder)
{
}