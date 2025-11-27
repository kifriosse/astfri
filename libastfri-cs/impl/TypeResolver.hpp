#ifndef CSHARP_TYPE_RESOLVER_HPP
#define CSHARP_TYPE_RESOLVER_HPP

#include <string>
#include <unordered_map>

#include <libastfri/inc/Stmt.hpp>

namespace astfri::csharp
{

enum class DeclarationType
{
    Class,
    Interface,
    Struct,
    Enum,
    Delegate,
    Unknown
};

struct TypeInfo
{
    ScopedType* type_;
    DeclarationType decl_type_;
};

struct NamespaceNode
{
    std::unordered_map<std::string, NamespaceNode> namespaces_;//
    std::unordered_map<std::string, TypeInfo> types_;
};


class TypeRegistry
{
private:
    NamespaceNode global_namespace_;
public:
    void add_type(TypeInfo const& info);
    NamespaceNode& root();
};

struct FileContext
{
    std::unordered_map<std::string, Scope> using_namespaces_;
    std::unordered_map<std::string, TypeInfo*> aliases_;
};

class TypeResolver
{
private:
    static std::unordered_map<std::string, DeclarationType> decl_type_map_;
    FileContext file_context_;
    TypeRegistry type_registry_;
    Scope current_namespace;
public:
    /**
     * Finds all defined types inside a source file;
     * @param file path to file with source code
     */
    void find_types_file(std::string const& file);
    /**
     * Finds all defined types inside a source code;
     * @param source_code source code
     */
    void find_types_src(std::string const& source_code);
    /**
     * Finds all defined types inside C# project
     * @param project_dir path to directory with C# project
     */
    void find_types_project(std::string const& project_dir);
    /**
     * Used for importing .NET/C# types or types from Nuget packages
     * (types that don't have definitions inside the project)
     * @param json_file path to the json file
     */
    void load_external_types(std::string const& json_file);
};

}

#endif // CSHARP_TYPE_RESOLVER_HPP