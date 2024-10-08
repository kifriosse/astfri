#include <libastfri/factories/DeclarationFactory.hpp>

#include <libastfri/structures/Declaration.hpp>

namespace astfri
{
DeclarationFactory& DeclarationFactory::getInstance()
{
    static DeclarationFactory instance;

    return instance;
}

// TODO uptrs and no destructor
DeclarationFactory::~DeclarationFactory()
{
    for (auto* variable : variables)
    {
        delete variable;
    }

    for (auto* function : functions)
    {
        delete function;
    }
}

VariableDefintion* DeclarationFactory::createVariable(
    std::string name,
    Type* type,
    Expression* value
)
{
    auto* variable = new VariableDefintion(name, type, value);
    variables.emplace_back(variable);

    return variable;
}

ParameterDefinition* DeclarationFactory::createParameter(
    std::string name,
    Type* type,
    Expression* value
)
{
    auto* parameter = new ParameterDefinition(name, type, value);
    variables.emplace_back(parameter);

    return parameter;
}

FunctionDefinition* DeclarationFactory::createFunction(
    std::string name,
    std::vector<ParameterDefinition*> parameters,
    CompoundStatement* body,
    Type* returnType
)
{
    auto* function
        = new FunctionDefinition(name, parameters, body, returnType);
    functions.emplace_back(function);

    return function;
}

UknownDeclaration* DeclarationFactory::createUknownDeclaration(
    std::string message
)
{
    auto* declaration = new UknownDeclaration(message);

    return declaration;
}

} // namespace libastfri::factories
