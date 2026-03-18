#ifndef CSHARP_METADATA_HPP
#define CSHARP_METADATA_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/data/Identifiers.hpp>
#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>

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
    TSNode nType;
    SourceFile* src;
};

/**
 * @brief Metadata about user defined type
 */
class TypeMetadata
{
private:
    std::unordered_map<MethodId, std::vector<MethodMetadata>> methods_{};
    IdentifierMap<MemberVarMetadata> memberVars_{};
    IdentifierMap<PropertyNode> properties_{};
    std::vector<TypeDefLoc> defs_{};
    TypeBinding tb_;

public:
    explicit TypeMetadata(const TypeBinding& tb);
    void add_def(const TSNode& node, SourceFile* src);
    void add_def(TypeDefLoc defLocation);
    void add_method(MethodId id, MethodMetadata methodMetadata);
    bool add_property(std::string name, PropertyNode prop);
    bool add_memb_var(std::string name, const MemberVarMetadata& membVar);

    /**
     * @brief Returns span of locations of type definitions in source code.
     * @return span of type defintion locations for this type
     */
    std::span<const TypeDefLoc> defs();
    /**
     * @brief Finds method with given id. If there are multiple methods with the
     * same id, it will return nullptr.
     * @param id method id to search for
     * @return pointer to method metadata if there is exactly one method with
     * given id, nullptr otherwise
     */
    MethodMetadata* find_method(const MethodId& id);
    /**
     * @brief Finds member variable with given name.
     * @param name name of member variable to search for
     * @return pointer to member variable metadata if there is member variable
     * with given name, nullptr otherwise
     */
    MemberVarMetadata* find_memb_var(std::string_view name);
    /**
     * @brief Finds property with given name.
     * @param name name of property to search for
     * @return pointer to property node if there is property with given name,
     * nullptr otherwise
     */
    PropertyNode* find_property(std::string_view name);
    TypeBinding& type_binding();
};

} // namespace astfri::csharp

#endif // #define CSHARP_METADATA_HPP
