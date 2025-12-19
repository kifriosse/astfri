#ifndef CSHARP_AST_BUILDER_HPP
#define CSHARP_AST_BUILDER_HPP

#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <string>
#include <vector>

namespace astfri::csharp
{

struct SourceFile
{
    std::filesystem::path file_path;
    std::string content;
};

class ASTBuilder
{
private:
    const TSLanguage* lang_;
    TSParser* parser_;
    std::unordered_map<std::string, TSTree*> source_trees_;

public:
    ASTBuilder();
    ~ASTBuilder();
    [[nodiscard]] TranslationUnit* make_ast(const std::string& source_code_dir);

private:
    [[nodiscard]] std::vector<SourceFile> get_source_codes(
        const std::string& project_dir
    ) const;
};

} // namespace astfri::csharp

#endif // CSHARP_AST_BUILDER_HPP
