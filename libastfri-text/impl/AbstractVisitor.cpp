#include <libastfri-text/inc/AbstractVisitor.hpp>

using namespace astfri::text;

AbstractVisitor::AbstractVisitor(AbstractBuilder& builder) :
    builder_(&builder)
{
}

//
// -----
//

void AbstractVisitor::process_condition(Expr const* const& expr)
{
    builder_->append_space();
    builder_->write_left_bracket("(");
    if (!try_accept_node(expr))
    {
        builder_->write_invalid_expr();
    }
    builder_->write_right_bracket(")");
}