#ifndef LIBASTFRI_STATEMENT_FACTORY_HPP
#define LIBASTFRI_STATEMENT_FACTORY_HPP

#include <libastfri/ExpressionFactory.hpp>
#include <libastfri/Declaration.hpp>
#include <libastfri/Statement.hpp>
#include <libastfri/Type.hpp>

namespace astfri
{
class StatementFactory
{
public:
    static StatementFactory& getInstance ();

private:
    UsedList<TranslationUnit*> translationUnits;
    UsedList<Statement*> statements;

    StatementFactory() = default;
    ~StatementFactory();
    CompoundStatement* tryGetCompoundStatement (Statement* Statement
    );

public:
    StatementFactory(StatementFactory const&) = delete;
    void operator= (StatementFactory const&)  = delete;

    TranslationUnit* createTranslationUnit (
        std::vector<FunctionDefinition*> functions
    );
    CompoundStatement* createCompoundStatement (
        std::vector<Statement*> statements
    );
    DeclarationStatement* createDeclarationStatement (
        Declaration* declaration
    );
    DeclarationAndAssigmentStatement*
        createDeclarationAndAssigmentStatement (
            Declaration* declaration,
            Expression* expression
        );

    ReturnStatement* createReturnStatement (Expression* value);

    ExpressionStatement* createExpressionStatement (
        Expression* expression
    );

    IfStatement* createIfConditionalStatement (
        Expression* condition,
        CompoundStatement* ifTrue,
        CompoundStatement* ifFalse = nullptr
    );
    IfStatement* createIfConditionalStatement (
        Expression* condition,
        Statement* ifTrue,
        Statement* ifFalse = nullptr
    );

    WhileStatement* createWhileStatement (
        Expression* condition,
        CompoundStatement* body
    );
    WhileStatement* createWhileStatement (
        Expression* condition,
        Statement* statement
    );

    DoWhileStatement* createDoWhileStatement (
        Expression* condition,
        CompoundStatement* body
    );
    ForStatement* createForStatement (
        Statement* init,
        Expression* condition,
        Expression* step,
        Statement* body
    );

    UnknownStatement* createUnknownStatement (std::string message);
};
} // namespace libastfri::factories

#endif