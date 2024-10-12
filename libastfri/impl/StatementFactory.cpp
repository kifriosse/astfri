#include <libastfri/factories/StatementFactory.hpp>

#include <libastfri/structures/Declaration.hpp>
#include <libastfri/structures/Statement.hpp>

namespace astfri
{

StatementFactory& StatementFactory::getInstance()
{
    static StatementFactory instance;

    return instance;
}

StatementFactory::~StatementFactory()
{
    for (auto* statement : statements)
    {
        delete statement;
    }
    statements.clear();

    for (auto* translationUnit : translationUnits)
    {
        delete translationUnit;
    }
    translationUnits.clear();
}

CompoundStatement* StatementFactory::tryGetCompoundStatement(
    Statement* Statement
)
{
    if (Statement == nullptr)
    {
        return nullptr;
    }

    auto* compoundStatement = dynamic_cast<CompoundStatement*>(Statement);
    if (compoundStatement == nullptr)
    {
        return createCompoundStatement({Statement});
    }

    return compoundStatement;
}

TranslationUnit* StatementFactory::createTranslationUnit(
    std::vector<FunctionDefinition*> functions
)
{
    auto* translationUnitStatement = new TranslationUnit {std::move(functions)};
    translationUnits.emplace_back(translationUnitStatement);

    return translationUnitStatement;
}

CompoundStatement* StatementFactory::createCompoundStatement(
    std::vector<Statement*> statements
)
{
    auto* compoundStatement = new CompoundStatement(std::move(statements));
    statements.emplace_back(compoundStatement);

    return compoundStatement;
}

DeclarationStatement* StatementFactory::createDeclarationStatement(
    Declaration* declaration
)
{
    auto* declarationStatement = new DeclarationStatement(declaration);
    statements.emplace_back(declarationStatement);

    return declarationStatement;
}

DeclarationAndAssigmentStatement* StatementFactory::
    createDeclarationAndAssigmentStatement(
        Declaration* variable,
        Expression* expression
    )
{
    auto* declarationAndAssigmentStatement
        = new DeclarationAndAssigmentStatement(variable, expression);
    statements.emplace_back(declarationAndAssigmentStatement);

    return declarationAndAssigmentStatement;
}

ReturnStatement* StatementFactory::createReturnStatement(Expression* value)
{
    auto* returnStatement = new ReturnStatement(value);
    statements.emplace_back(returnStatement);

    return returnStatement;
}

ExpressionStatement* StatementFactory::createExpressionStatement(
    Expression* expression
)
{
    auto* expressionStatement = new ExpressionStatement(expression);
    statements.emplace_back(expressionStatement);

    return expressionStatement;
}

IfStatement* StatementFactory::createIfConditionalStatement(
    Expression* condition,
    CompoundStatement* thenBody,
    CompoundStatement* elseBody
)
{
    auto* ifStatement = new IfStatement(condition, thenBody, elseBody);
    statements.emplace_back(ifStatement);

    return ifStatement;
}

IfStatement* StatementFactory::createIfConditionalStatement(
    Expression* condition,
    Statement* thenStatement,
    Statement* elseStatement
)
{
    // try parse statements to compound statements
    auto* thenCompoundStatement = tryGetCompoundStatement(thenStatement);
    auto* elseCompoundStatement = tryGetCompoundStatement(elseStatement);

    return createIfConditionalStatement(
        condition,
        thenCompoundStatement,
        elseCompoundStatement
    );
}

WhileStatement* StatementFactory::createWhileStatement(
    Expression* condition,
    CompoundStatement* body
)
{
    auto* whileLoopStatement = new WhileStatement(condition, body);
    statements.emplace_back(whileLoopStatement);

    return whileLoopStatement;
}

WhileStatement* StatementFactory::createWhileStatement(
    Expression* condition,
    Statement* statement
)
{
    auto* body = tryGetCompoundStatement(statement);

    return createWhileStatement(condition, body);
}

DoWhileStatement* StatementFactory::createDoWhileStatement(
    Expression* condition,
    CompoundStatement* body
)
{
    auto* doWhileStatement = new DoWhileStatement(condition, body);
    statements.emplace_back(doWhileStatement);

    return doWhileStatement;
}

ForStatement* StatementFactory::createForStatement(
    Statement* init,
    Expression* condition,
    Expression* step,
    Statement* body
)
{
    ForStatement* forStatement = new ForStatement(
        init,
        condition,
        step,
        tryGetCompoundStatement(body)
    );
    statements.emplace_back(forStatement);

    return forStatement;
}

UnknownStatement* StatementFactory::createUnknownStatement(std::string message)
{
    auto* unknownStatement = new UnknownStatement(message);
    statements.emplace_back(unknownStatement);

    return unknownStatement;
}

} // namespace astfri
