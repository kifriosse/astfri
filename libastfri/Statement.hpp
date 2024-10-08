#ifndef LIBASTFRI_STATEMENT_HPP
#define LIBASTFRI_STATEMENT_HPP

#include <libastfri/Declaration.hpp>
#include <libastfri/Expression.hpp>
#include <libastfri/Visitor.hpp>
#include <libastfri/impl/Utils.hpp>

#include <string>
#include <vector>

namespace astfri
{
/**
 * @brief pre reprazentaciu celeho vstupneho bloku kodu / suboru
 *
 */
struct TranslationUnit : details::MakeVisitable<TranslationUnit>
{
    std::vector<FunctionDefinition*> functions;

    TranslationUnit(std::vector<FunctionDefinition*> functions = {});
};

// "riadok" v kode
struct Statement : virtual IVisitable
{
    Statement() = default;
};

struct CompoundStatement : Statement, details::MakeVisitable<CompoundStatement>
{
    std::vector<Statement*> statements;

    CompoundStatement(std::vector<Statement*> statements = {});
};

struct DeclarationStatement :
    Statement,
    details::MakeVisitable<DeclarationStatement>
{
    Declaration* declaration;

    DeclarationStatement(Declaration* declaration);
};

struct DeclarationAndAssigmentStatement :
    Statement,
    details::MakeVisitable<DeclarationAndAssigmentStatement>
{
    Declaration* declaration;
    Expression* expression;

    DeclarationAndAssigmentStatement(
        Declaration* declaration,
        Expression* expression
    );
};

struct ReturnStatement : Statement, details::MakeVisitable<ReturnStatement>
{
    Expression* value;

    ReturnStatement(Expression* value);
};

struct ExpressionStatement :
    Statement,
    details::MakeVisitable<ExpressionStatement>
{
    Expression* expression;

    ExpressionStatement(Expression* expression);
};

// Conditional statement
struct ConditionalStatement : Statement
{
    Expression* condition;

    ConditionalStatement(Expression* condition);
};

struct IfStatement : ConditionalStatement, details::MakeVisitable<IfStatement>
{
    CompoundStatement* thenBody;
    CompoundStatement* elseBody;

    IfStatement(
        Expression* condition,
        CompoundStatement* thenBody,
        CompoundStatement* elseBody = nullptr
    );
};

// Loop statement
struct LoopStatement : Statement
{
    Expression* condition;
    CompoundStatement* body;

    LoopStatement(Expression* condition, CompoundStatement* body);
};

struct WhileStatement : LoopStatement, details::MakeVisitable<WhileStatement>
{
    WhileStatement(Expression* condition, CompoundStatement* body);
};

struct DoWhileStatement :
    LoopStatement,
    details::MakeVisitable<DoWhileStatement>
{
    DoWhileStatement(Expression* condition, CompoundStatement* body);
};

struct ForStatement : LoopStatement, details::MakeVisitable<ForStatement>
{
    Statement* init;
    Expression* step;

    ForStatement(
        Statement* init,
        Expression* condition,
        Expression* step,
        CompoundStatement* body
    );
};

struct UnknownStatement : Statement, details::MakeVisitable<UnknownStatement>
{
    std::string message;

    UnknownStatement(std::string message);
};
} // namespace libastfri::structures

#endif