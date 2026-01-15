#ifndef CSHARP_SOURCE_HPP
#define CSHARP_SOURCE_HPP

#include <libastfri-cs/impl/data/FileContext.hpp>

#include <tree_sitter/api.h>

#include <filesystem>

namespace astfri::csharp
{

struct SourceFile
{
    std::filesystem::path file_path{};
    std::string content{};
};

struct SourceCode
{
    FileContext file_context{};
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