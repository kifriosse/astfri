#ifndef CSHARP_SOURCE_HPP
#define CSHARP_SOURCE_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace astfri::csharp
{

struct FileContext
{
    using Alias = std::variant<Type*, Scope, std::string>;
    // todo add global aliases
    // todo redo this into namespace aware
    IdentifierMap<Alias> aliases{};
    std::vector<Scope> usings{};
    std::vector<UserTypeDefStmt*> staticUsings{};
    std::optional<std::string> fileNms{};
};

struct SourceFile
{
    FileContext fileContext{};
    std::string srcStr;
    TSTree* tree{nullptr};

    explicit SourceFile(FileContext context, std::string src, TSTree* tree);
    ~SourceFile();

    SourceFile(const SourceFile&)            = delete;
    SourceFile& operator=(const SourceFile&) = delete;
    SourceFile(SourceFile&& other) noexcept;
    SourceFile& operator=(SourceFile&& other) noexcept;
};

} // namespace astfri::csharp

#endif // CSHARP_SOURCE_HPP