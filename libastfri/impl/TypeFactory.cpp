#include <libastfri/factories/TypeFactory.hpp>

#include <libastfri/utils/Helper.hpp>

namespace astfri
{

TypeFactory& TypeFactory::getInstance()
{
    static TypeFactory instance;

    return instance;
}

IntType* TypeFactory::getIntType()
{
    return &this->intType;
}

FloatType* TypeFactory::getFloatType()
{
    return &this->floatType;
}

CharType* TypeFactory::getCharType()
{
    return &this->charType;
}

BoolType* TypeFactory::getBoolType()
{
    return &this->boolType;
}

VoidType* TypeFactory::getVoidType()
{
    return &this->voidType;
}

UserType* TypeFactory::getUserType(std::string name)
{
    return &Helper::getValueFromMap(
        name,
        userTypes,
        [] (auto& p_map, auto p_key)
        { return p_map.emplace(p_key, UserType {{p_key}}); }
    );
}
} // namespace astfri
