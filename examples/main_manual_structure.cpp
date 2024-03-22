#include <libastfri/structures/Expression.hpp>
#include <libastfri/structures/Statement.hpp>

#include <libastfri/factories/DeclarationFactory.hpp>
#include <libastfri/factories/ExpressionFactory.hpp>
#include <libastfri/factories/StatementFactory.hpp>
#include <libastfri/factories/TypeFactory.hpp>

using namespace libastfri::structures;
using namespace libastfri::factories;

/**
 * @brief example, kde vyuzivam manualne vytvorene struktury z libastfri pre
 * reprezentovanie kodu funcki na konci suboru
 */
int main() {
    auto &typeFac = TypeFactory::getInstance();
    auto &literalFac = LiteralFactory::getInstance();
    auto &statementFac = StatementFactory::getInstance();
    auto &expressionFac = ExpressionFactory::getInstance();
    auto &referenceFac = ReferenceFactory::getInstance();
    auto &declarationFac = DeclarationFactory::getInstance();

    // int simpleAddition(int a, int b)
    std::vector<ParameterDefinition *> paramsSimpleAddition;
    paramsSimpleAddition.push_back(
        declarationFac.createParameter("a", typeFac.getIntType()));
    paramsSimpleAddition.push_back(
        declarationFac.createParameter("b", typeFac.getIntType()));

    auto bodySimpleAddition = statementFac.createCompoundStatement(
        {statementFac.createReturnStatement(
            expressionFac.createBinaryExpression(
                BinaryOperators::Add,
                referenceFac.createParamRefExpression(paramsSimpleAddition[0]),
                referenceFac.createParamRefExpression(
                    paramsSimpleAddition[1])))});
    auto retTypeSimpleAddition = typeFac.getIntType();

    auto functionSimpleAddition = declarationFac.createFunction(
        "simpleAddition", paramsSimpleAddition, bodySimpleAddition,
        retTypeSimpleAddition);

    // int brutalAddition(int a, int b)
    std::vector<ParameterDefinition *> params;
    params.push_back(declarationFac.createParameter("a", typeFac.getIntType()));
    params.push_back(declarationFac.createParameter("b", typeFac.getIntType()));
    auto variableC = declarationFac.createVariable("c", typeFac.getIntType());
    auto variableRepMultiplier =
        declarationFac.createVariable("repMultiplier", typeFac.getIntType());
    auto variableRepCount =
        declarationFac.createVariable("repCount", typeFac.getIntType());

    auto body = statementFac.createCompoundStatement(
        {statementFac.createDeclarationAndAssigmentStatement(
             variableC, expressionFac.createBinaryExpression(
                            BinaryOperators::Add,
                            referenceFac.createParamRefExpression(params[0]),
                            referenceFac.createParamRefExpression(params[1]))),
         statementFac.createDeclarationAndAssigmentStatement(
             variableRepMultiplier, literalFac.getIntLiteral(1)),
         statementFac.createIfConditionalStatement(
             expressionFac.createBinaryExpression(
                 BinaryOperators::Less,
                 referenceFac.createVarRefExpression(variableC),
                 literalFac.getIntLiteral(0)),
             statementFac.createCompoundStatement(
                 {statementFac.createExpressionStatement(
                     expressionFac.createBinaryExpression(
                         BinaryOperators::Assign,
                         referenceFac.createVarRefExpression(
                             variableRepMultiplier),
                         literalFac.getIntLiteral(-1)))})),
         statementFac.createDeclarationAndAssigmentStatement(
             variableRepCount, literalFac.getIntLiteral(0)),
         statementFac.createWhileLoopStatement(
             expressionFac.createBinaryExpression(
                 BinaryOperators::Less,
                 expressionFac.createBinaryExpression(
                     BinaryOperators::Add,
                     referenceFac.createVarRefExpression(variableC),
                     referenceFac.createVarRefExpression(variableRepCount)),
                 literalFac.getIntLiteral(0)),
             statementFac.createCompoundStatement(
                 {statementFac.createExpressionStatement(
                     expressionFac.createBinaryExpression(
                         BinaryOperators::Assign,
                         referenceFac.createVarRefExpression(variableRepCount),
                         expressionFac.createBinaryExpression(
                             BinaryOperators::Add,
                             referenceFac.createVarRefExpression(
                                 variableRepCount),
                             referenceFac.createVarRefExpression(
                                 variableRepMultiplier))))})),
         statementFac.createExpressionStatement(
             referenceFac.createFunctionCallExpression(
                 functionSimpleAddition->name,
                 {referenceFac.createParamRefExpression(params[0]),
                  referenceFac.createParamRefExpression(params[1])})),
         statementFac.createReturnStatement(
             referenceFac.createFunctionCallExpression(
                 functionSimpleAddition->name,
                 {referenceFac.createParamRefExpression(params[0]),
                  referenceFac.createParamRefExpression(params[1])}))});

    auto retType = typeFac.getIntType();

    auto functionBrutalAddition =
        declarationFac.createFunction("brutalAddition", params, body, retType);

    return 0;
}

int simpleAddition(int a, int b) { return a + b; }

int brutalAddition(int a, int b) {
    int c = a + b;
    int repMultiplier = 1;

    if (c < 0) {
        repMultiplier = -1;
    }

    int repCount = 0;
    while (c + repCount < 0) {
        repCount += repMultiplier;
    }

    return simpleAddition(a, b);
}