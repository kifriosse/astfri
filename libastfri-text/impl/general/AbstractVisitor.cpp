#include <libastfri-text/inc/general/AbstractVisitor.hpp>

using namespace astfri::text;

AbstractVisitor::AbstractVisitor(AbstractBuilder& builder) :
    m_builder(&builder)
{
}

//
// -----
//

void AbstractVisitor::process_condition(Expr* expr)
{
    m_builder->write_space();
    m_builder->write_left_bracket("(");
    accept_node(expr);
    m_builder->write_right_bracket(")");
}

void AbstractVisitor::process_body(Stmt* stmt)
{
    m_builder->write_opening_curl_bracket();
    accept_node(stmt);
    m_builder->write_new_line();
    m_builder->decrease_indentation();
    m_builder->write_right_bracket("}");
}