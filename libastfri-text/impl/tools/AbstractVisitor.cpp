#include <libastfri-text/inc/tools/AbstractVisitor.hpp>

using namespace astfri::text;

AbstractVisitor::AbstractVisitor(TextBuilder* builder) :
    m_builder(builder)
{
}

void AbstractVisitor::process_condition(Expr* expr)
{
    m_builder->write_space();
    m_builder->write_left_bracket("(");
    accept_node(expr);
    m_builder->write_right_bracket(")");
}

void AbstractVisitor::process_body(Stmt* stmt, bool const& onNewLine)
{
    m_builder->write_opening_curl_bracket(onNewLine);
    accept_node(stmt);
    m_builder->write_closing_curl_bracket();
}