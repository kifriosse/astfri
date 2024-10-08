#ifndef LIBASTFRI_DECLARATION_HPP
#define LIBASTFRI_DECLARATION_HPP

#include <libastfri/Visitor.hpp>
#include <libastfri/impl/Utils.hpp>

#include <string>
#include <vector>

namespace astfri
{
struct Type;
struct Expression;
struct CompoundStatement;

struct Declaration : virtual IVisitable
{
};

// zaklad, ktory sa pouziva pri definiciach premennych, parametrov, atributov,
// atd.
struct BaseVariableDefintion : Declaration
{
    std::string name;
    Type* type;
    Expression* value;

    BaseVariableDefintion(
        std::string name,
        Type* type,
        Expression* value = nullptr
    );
};

// premenna
struct VariableDefintion :
    BaseVariableDefintion,
    details::MakeVisitable<VariableDefintion>
{
    VariableDefintion(
        std::string name,
        Type* type,
        Expression* value = nullptr
    );
};

// parameter
struct ParameterDefinition :
    BaseVariableDefintion,
    details::MakeVisitable<ParameterDefinition>
{
    ParameterDefinition(
        std::string name,
        Type* type,
        Expression* value = nullptr
    );
};

// funkcia
struct FunctionDefinition :
    Declaration,
    details::MakeVisitable<FunctionDefinition>
{
    std::string name;
    std::vector<ParameterDefinition*> parameters;
    CompoundStatement* body;
    Type* returnType;

    FunctionDefinition(
        std::string name,
        std::vector<ParameterDefinition*> parameters,
        CompoundStatement* body,
        Type* returnType
    );
};

struct UknownDeclaration :
    Declaration,
    details::MakeVisitable<UknownDeclaration>
{
    std::string message;

    UknownDeclaration(std::string message);
};

} // namespace libastfri::structures

#endif