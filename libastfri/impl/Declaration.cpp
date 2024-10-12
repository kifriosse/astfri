#include <libastfri/structures/Declaration.hpp>

namespace astfri
{

BaseVariableDefintion::BaseVariableDefintion(
    std::string name,
    Type* type,
    Expression* value
) :
    name(std::move(name)),
    type(type),
    value(value)
{
}

VariableDefintion::VariableDefintion(
    std::string name,
    Type* type,
    Expression* value
) :
    BaseVariableDefintion(std::move(name), type, value)
{
}

ParameterDefinition::ParameterDefinition(
    std::string name,
    Type* type,
    Expression* value
) :
    BaseVariableDefintion(std::move(name), type, value)
{
}

FunctionDefinition::FunctionDefinition(
    std::string name,
    std::vector<ParameterDefinition*> parameters,
    CompoundStatement* body,
    Type* returnType
) :
    name(std::move(name)),
    parameters(std::move(parameters)),
    body(body),
    returnType(returnType)
{
}

UknownDeclaration::UknownDeclaration(std::string message) :
    message(std::move(message))
{
}

} // namespace astfri