#ifndef CSHARP_SOURCE_HPP
#define CSHARP_SOURCE_HPP

#include <tree_sitter/api.h>

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace astfri::csharp
{
class ScopeNode;

struct FileContext
{
    std::vector<ScopeNode*> usings{};
    std::optional<std::string> fileNms{}; // use ScopeNode here
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