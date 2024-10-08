#ifndef LIBASTFRI_DECLARATION_FACTORY_HPP
#define LIBASTFRI_DECLARATION_FACTORY_HPP

#include <libastfri/ExpressionFactory.hpp>
#include <libastfri/Declaration.hpp>
#include <string>

namespace astfri
{
class DeclarationFactory
{
public:
    static DeclarationFactory& getInstance ();

private:
    UsedList<BaseVariableDefintion*> variables;
    UsedList<FunctionDefinition*> functions;

    DeclarationFactory() = default;
    ~DeclarationFactory();

public:
    VariableDefintion* createVariable (
        std::string name,
        Type* type,
        Expression* value = nullptr
    );
    ParameterDefinition* createParameter (
        std::string name,
        Type* type,
        Expression* value = nullptr
    );

    FunctionDefinition* createFunction (
        std::string name,
        std::vector<ParameterDefinition*> parameters,
        CompoundStatement* body,
        Type* returnType
    );

    UknownDeclaration* createUknownDeclaration (std::string message);

    DeclarationFactory(DeclarationFactory const&) = delete;
    void operator= (DeclarationFactory const&)    = delete;
};
} // namespace libastfri::factories

#endif