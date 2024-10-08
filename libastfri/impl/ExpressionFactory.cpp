#include <libastfri/factories/ExpressionFactory.hpp>

#include <libastfri/utils/Helper.hpp>

namespace astfri
{

//// LiteralFactory
LiteralFactory& LiteralFactory::getInstance()
{
    static LiteralFactory instance;

    return instance;
}

template<typename K, typename T>
T* LiteralFactory::getLiteralFromMap(K key, UsedMap<K, T>& map)
{
    return &Helper::getValueFromMap(
        key,
        map,
        [] (auto& p_map, K p_key) { return p_map.emplace(p_key, T(p_key)); }
    );
}

IntLiteral* LiteralFactory::getIntLiteral(int literal)
{
    return getLiteralFromMap(literal, this->intLiterals);
}

FloatLiteral* LiteralFactory::getFloatLiteral(float literal)
{
    return getLiteralFromMap(literal, this->floatLiterals);
}

CharLiteral* LiteralFactory::getCharLiteral(char literal)
{
    return getLiteralFromMap(literal, this->charLiterals);
}

StringLiteral* LiteralFactory::getStringLiteral(std::string literal)
{
    return getLiteralFromMap(std::move(literal), this->stringLiterals);
}

BoolLiteral* LiteralFactory::getBoolLiteral(bool literal)
{
    return getLiteralFromMap(literal, this->boolLiterals);
}

//// ExpressionFactory
ExpressionFactory& ExpressionFactory::getInstance()
{
    static ExpressionFactory instance;

    return instance;
}

ExpressionFactory::~ExpressionFactory()
{
    for (auto expr : this->expressions)
    {
        delete expr;
    }
    expressions.clear();
}

UnaryExpression* ExpressionFactory::createUnaryExpression(
    UnaryOperators op,
    Expression* operand
)
{
    auto* expr = new UnaryExpression(op, operand);
    this->expressions.emplace_back(expr);

    return expr;
}

BinaryExpression* ExpressionFactory::createBinaryExpression(
    BinaryOperators op,
    Expression* left,
    Expression* right
)
{
    auto* expr = new BinaryExpression(left, op, right);
    this->expressions.emplace_back(expr);

    return expr;
}

UnknownExpression* ExpressionFactory::createUnknownExpression(
    std::string message
)
{
    auto* expr = new UnknownExpression(std::move(message));

    return expr;
}

//// ReferenceFactory
ReferenceFactory& ReferenceFactory::getInstance()
{
    static ReferenceFactory instance;

    return instance;
}

ReferenceFactory::~ReferenceFactory()
{
    for (auto expr : this->refExpressions)
    {
        delete expr;
    }
    refExpressions.clear();
}

VarRefExpression* ReferenceFactory::createVarRefExpression(
    VariableDefintion* variable
)
{
    VarRefExpression* expr = new VarRefExpression(variable);
    this->refExpressions.push_back(expr);

    return expr;
}

ParamRefExpression* ReferenceFactory::createParamRefExpression(
    ParameterDefinition* parameter
)
{
    ParamRefExpression* expr = new ParamRefExpression(parameter);
    this->refExpressions.push_back(expr);

    return expr;
}

FunctionCallExpression* ReferenceFactory::createFunctionCallExpression(
    std::string functionName,
    std::vector<Expression*> arguments
)
{
    FunctionCallExpression* expr = new FunctionCallExpression(
        std::move(functionName),
        std::move(arguments)
    );
    this->refExpressions.push_back(expr);

    return expr;
}

} // namespace libastfri::factories
