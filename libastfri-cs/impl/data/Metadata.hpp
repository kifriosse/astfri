#ifndef CSHARP_METADATA_HPP
#define CSHARP_METADATA_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/Identifiers.hpp>
#include <libastfri-cs/impl/data/Source.hpp>

#include <tree_sitter/api.h>

#include <unordered_map>

namespace astfri
{

// forward declarations
struct MemberVarDefStmt;
struct ParamVarDefStmt;
struct FunctionDefStmt;
struct MethodDefStmt;
struct UserTypeDefStmt;

} // namespace astfri

namespace astfri::csharp
{

/**
 * @brief Metadata about member variable
 */
struct MemberVarMetadata
{
    MemberVarDefStmt* varDef{nullptr};
    TSNode nVar{};
    TSNode nInit{}; // right side of assignment
    // todo maybe add flag for static variable
    bool processed{false};
};

/**
 * @brief Metadata about method parameter
 */
struct ParamMetadata
{
    ParamVarDefStmt* paramDef{nullptr};
    TSNode nParam{};
    TSNode nInit{}; // right side of assignment
};

/**
 * @brief Metadata about function
 */
struct FuncMetadata
{
    std::vector<ParamMetadata> params{};
    FunctionDefStmt* funcDef{nullptr};
    TSNode nFunc{};
};

/**
 * @brief Metadata about method
 */
struct MethodMetadata
{
    std::vector<ParamMetadata> params{};
    MethodDefStmt* methodDef{nullptr};
    TSNode nMethod{};
};

/**
 * @brief Metadata about property
 */
struct PropertyNode
{
    MemberVarMetadata backingField;
    MethodMetadata getter;
    MethodMetadata setter;
};

/**
 * @brief Location of type definition in source code
 */
struct TypeDefLoc
{
    TSNode nType{};
    SourceFile* src{nullptr};
};

/**
 * @brief Metadata about user defined type
 */
struct TypeMetadata
{
    UserTypeDefStmt* userType{nullptr};
    std::unordered_map<MethodId, MethodMetadata> methods{};
    IdentifierMap<MemberVarMetadata> memberVars{};
    IdentifierMap<PropertyNode> properties{};
    std::vector<TypeDefLoc> defs{};
    bool processed{false};
};
} // namespace astfri::csharp

#endif // #define CSHARP_METADATA_HPP
