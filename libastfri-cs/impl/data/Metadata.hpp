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
    MemberVarDefStmt* var_def{nullptr};
    TSNode var_node{};
    TSNode initializer{}; // right side of assignment
    // todo maybe add flag for static variable
    bool processed{false};
};

/**
 * @brief Metadata about method parameter
 */
struct ParamMetadata
{
    ParamVarDefStmt* param_def{nullptr};
    TSNode param_node{};
    TSNode initializer{}; // right side of assignment
};

/**
 * @brief Metadata about function
 */
struct FunctionMetadata
{
    std::vector<ParamMetadata> params{};
    FunctionDefStmt* func_def{nullptr};
    TSNode function_node{};
};

/**
 * @brief Metadata about method
 */
struct MethodMetadata
{
    std::vector<ParamMetadata> params{};
    MethodDefStmt* method_def{nullptr};
    TSNode method_node{};
};

/**
 * @brief Metadata about property
 */
struct PropertyNode
{
    MemberVarMetadata backing_field;
    MethodMetadata getter;
    MethodMetadata setter;
};

/**
 * @brief Location of type definition in source code
 */
struct TypeDefLoc
{
    TSNode type_node{};
    SourceCode* src_code{nullptr};
};

/**
 * @brief Metadata about user defined type
 */
struct TypeMetadata
{
    UserTypeDefStmt* user_type{nullptr};
    std::unordered_map<MethodId, MethodMetadata> methods{};
    IdentifierMap<MemberVarMetadata> member_vars{};
    IdentifierMap<PropertyNode> properties{};
    std::vector<TypeDefLoc> defs{};
    bool processed{false};
};
} // namespace astfri::csharp

#endif // #define CSHARP_METADATA_HPP
