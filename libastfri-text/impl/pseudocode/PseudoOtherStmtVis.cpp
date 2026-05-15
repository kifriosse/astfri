#include <libastfri-text/inc/pseudocode/PseudocodeVisitor.hpp>

using namespace astfri::text;

void PseudocodeVisitor::visit(const CompoundStmt& stmt)
{
    for (size_t i = 0; i < stmt.stmts.size(); ++i)
    {
        accept_node(stmt.stmts.at(i));
        if (i < stmt.stmts.size() - 1)
        {
            m_builder->write_new_line();
        }
    }
}

void PseudocodeVisitor::visit(const ReturnStmt& stmt)
{
    m_builder->write_return_word();
    if (stmt.val)
    {
        m_builder->write_space();
        accept_node(stmt.val);
    }
}

void PseudocodeVisitor::visit(const ExprStmt& stmt)
{
    accept_node(stmt.expr);
}

void PseudocodeVisitor::visit(const IfStmt& stmt)
{
    m_builder->write_if_word();
    process_condition(stmt.cond);
    m_builder->write_space();
    process_body(stmt.iftrue, m_config->conditionBlockBracketNewLine);
    if (stmt.iffalse)
    {
        m_builder->write_opening_else_word();
        process_body(stmt.iffalse, m_config->conditionBlockBracketNewLine);
    }
}

void PseudocodeVisitor::visit(const CaseStmt& stmt)
{
}

void PseudocodeVisitor::visit(const DefaultCaseStmt& stmt)
{
    m_builder->write_default_word();
    m_builder->write_space();
    m_builder->write_do_word();
    process_body(stmt.body, m_config->switchBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const SwitchStmt& stmt)
{
}

void PseudocodeVisitor::visit(const WhileStmt& stmt)
{
    m_builder->write_while_word();
    process_condition(stmt.cond);
    m_builder->write_space();
    m_builder->write_repeat_word();
    process_body(stmt.body, m_config->loopBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const DoWhileStmt& stmt)
{
    m_builder->write_repeat_word();
    process_body(stmt.body, m_config->loopBlockBracketNewLine);
    if (m_config->dowhileConditionNewLine)
    {
        m_builder->write_new_line();
    }
    else
    {
        m_builder->write_space();
    }
    m_builder->write_while_word();
    process_condition(stmt.cond);
}

void PseudocodeVisitor::visit(const ForStmt& stmt)
{
    m_builder->write_for_word();
    m_builder->write_space();
    m_builder->write_left_bracket("(");
    accept_node(stmt.init);
    m_builder->write_separator(";");
    m_builder->write_space();
    accept_node(stmt.cond);
    m_builder->write_separator(";");
    m_builder->write_space();
    accept_node(stmt.step);
    m_builder->write_right_bracket(")");
    m_builder->write_space();
    m_builder->write_repeat_word();
    process_body(stmt.body, m_config->loopBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const ForEachStmt& stmt)
{
    m_builder->write_foreach_word();
    m_builder->write_space();
    m_builder->write_left_bracket("(");
    accept_node(stmt.var);
    m_builder->write_text(" from container ");
    accept_node(stmt.container);
    m_builder->write_right_bracket(")");
    m_builder->write_space();
    m_builder->write_repeat_word();
    process_body(stmt.body, m_config->loopBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const ThrowStmt& stmt)
{
    m_builder->write_throw_word();
    accept_node(stmt.val);
}

void PseudocodeVisitor::visit(const CatchStmt& stmt)
{
    m_builder->write_catch_word();
    process_params_or_args(stmt.param, false);
    process_body(stmt.body, m_config->trycatchBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const TryStmt& stmt)
{
}

void PseudocodeVisitor::visit(const UnknownStmt& /*stmt*/)
{
    m_builder->write_unknown_stmt();
}

void PseudocodeVisitor::visit(const ContinueStmt& /*stmt*/)
{
    m_builder->write_continue_word();
}

void PseudocodeVisitor::visit(const BreakStmt& /*stmt*/)
{
    m_builder->write_break_word();
}