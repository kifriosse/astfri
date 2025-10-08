#ifndef LIBASTFRIUML_TYPE_CONVENTION_HPP
#define LIBASTFRIUML_TYPE_CONVENTION_HPP

#include <string>

namespace astfri::uml
{
class TypeConvention
{
public:
    virtual std::string get_string(std::string type, std::string val, char separator) = 0;
};

class TypeBeforeConvention : public TypeConvention
{
public:
    std::string get_string(std::string type, std::string val, char separator) override;
};

class TypeAfterConvention : public TypeConvention
{
public:
    std::string get_string(std::string type, std::string val, char separator) override;
};
} // namespace astfri::uml

#endif
