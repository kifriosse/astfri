#ifndef CSHARP_SOURCE_HPP
#define CSHARP_SOURCE_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <string>
#include <variant>
#include <vector>

namespace astfri::csharp
{

struct FileContext
{
    // todo add global aliases
    std::vector<Scope> usings{};
    std::vector<UserTypeDefStmt*> staticUsings{};
    // todo redo this into namespace aware
    IdentifierMap<std::variant<Type*, std::string>> aliases{};
};

struct SourceFile
{
    std::filesystem::path filePath{};
    std::string content{};
};

struct SourceCode
{
    FileContext fileContext{};
    SourceFile file{};
    TSTree* tree{nullptr};

    explicit SourceCode(SourceFile file, TSTree* tree = nullptr);
    ~SourceCode();

    const TSLanguage* lang() const;

    SourceCode(const SourceCode&)            = delete;
    SourceCode& operator=(const SourceCode&) = delete;
    SourceCode(SourceCode&& other) noexcept;
    SourceCode& operator=(SourceCode&& other) noexcept;
};

} // namespace astfri::csharp

#endif // CSHARP_SOURCE_HPP