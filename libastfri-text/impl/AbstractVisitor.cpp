#include <libastfri-text/inc/AbstractVisitor.hpp>

using namespace astfri::text;

AbstractVisitor::AbstractVisitor(AbstractBuilder& builder) :
    builder_(&builder)
{
}

//
// -----
//

void AbstractVisitor::process_body(Stmt const* const& stmt)
{
    builder_->write_opening_curl_bracket();
    if (!try_accept_node(stmt))
    {
        builder_->write_invalid_stmt();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->write_right_bracket("}");
}

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

// TODO tmp
std::stringstream* AbstractVisitor::get_builded_text()
{
    return builder_->get_builded_text();
}