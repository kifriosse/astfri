#ifndef LIBASTFRIUML_ELEMENT_STRUCTS_HPP
#define LIBASTFRIUML_ELEMENT_STRUCTS_HPP

#include <libastfri/inc/Astfri.hpp>

#include <string>
#include <vector>

namespace astfri::uml
{
enum class RelationType
{
    ASSOCIATION = 0,
    COMPOSITION,
    EXTENSION,
    IMPLEMENTATION
};

enum class UserDefinedType
{
    CLASS = 0,
    STRUCT,
    INTERFACE,
    ENUM
};

struct ClassStruct
{
    std::string name_;
    UserDefinedType type_;
    std::vector<std::string> genericParams_;

    void reset();
};

struct VarStruct
{
    std::string name_;
    std::string type_;
    std::string init_;
    bool isIndirect_ = false;
    astfri::AccessModifier accessMod_;

    void reset();
};

struct MethodStruct
{
    std::string name_;
    std::string retType_;
    bool returnIsIndirect_ = false;
    std::vector<VarStruct> params_;
    astfri::AccessModifier accessMod_;

    void reset();
};

struct ConstructorStruct
{
    std::string class_;
    std::vector<VarStruct> params_;
    astfri::AccessModifier accessMod_;

    void reset();
};

struct DestructorStruct
{
    std::string class_;

    void reset();
};

struct RelationStruct
{
    std::string from_;
    std::string to_;
    RelationType type_;
};
} // namespace astfri::uml

#endif
