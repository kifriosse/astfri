#ifndef CSHARP_SOURCE_HPP
#define CSHARP_SOURCE_HPP

#include <libastfri/impl/Scope.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <string>
#include <vector>

namespace astfri::csharp {
class ScopeNode;

/**
 * @brief Represents a source file
 * @note SourceFile is not copyable
 */
struct SourceFile {
    std::vector<ScopeNode*> usings{};
    Scope fileNms{};
    std::string srcStr;
    TSTree* tree{nullptr};

    /**
     * @brief Constructs SourceFile
     * @param src source code of current file
     * @param tree tree-sitter syntax tree for current file
     * @param fileNms file scoped namespace in current file - default value is
     * empty
     */
    explicit SourceFile(std::string src, TSTree* tree, Scope fileNms = {});
    ~SourceFile();

    SourceFile(const SourceFile&)            = delete;
    SourceFile& operator=(const SourceFile&) = delete;
    SourceFile(SourceFile&& other) noexcept;
    SourceFile& operator=(SourceFile&& other) noexcept;
};

} // namespace astfri::csharp

#endif // CSHARP_SOURCE_HPP