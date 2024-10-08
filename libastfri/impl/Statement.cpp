#include <libastfri/structures/Statement.hpp>

#include "libastfri/structures/Declaration.hpp"

namespace astfri
{

TranslationUnit::TranslationUnit(std::vector<FunctionDefinition*> functions) :
    functions(std::move(functions))
{
}

CompoundStatement::CompoundStatement(std::vector<Statement*> statements) :
    statements(std::move(statements))
{
}

DeclarationStatement::DeclarationStatement(Declaration* declaration) :
    declaration(declaration)
{
}

DeclarationAndAssigmentStatement::DeclarationAndAssigmentStatement(
    Declaration* declaration,
    Expression* expression
) :
    declaration(declaration),
    expression(expression)
{
}

ReturnStatement::ReturnStatement(Expression* value) : value(value)
{
}

ExpressionStatement::ExpressionStatement(Expression* expression) :
    expression(expression)
{
}

ConditionalStatement::ConditionalStatement(Expression* condition) :
    condition(condition)
{
}

IfStatement::IfStatement(
    Expression* condition,
    CompoundStatement* thenBody,
    CompoundStatement* elseBody
) :
    ConditionalStatement(condition),
    thenBody(thenBody),
    elseBody(elseBody)
{
}

LoopStatement::LoopStatement(Expression* condition, CompoundStatement* body) :
    condition(condition),
    body(body)
{
}

WhileStatement::WhileStatement(Expression* condition, CompoundStatement* body) :
    LoopStatement(condition, body)
{
}

DoWhileStatement::DoWhileStatement(
    Expression* condition,
    CompoundStatement* body
) :
    LoopStatement(condition, body)
{
}

ForStatement::ForStatement(
    Statement* init,
    Expression* condition,
    Expression* step,
    CompoundStatement* body
) :
    LoopStatement(condition, body),
    init(init),
    step(step)
{
}

UnknownStatement::UnknownStatement(std::string message) :
    message(std::move(message))
{
}

} // namespace libastfri::structures