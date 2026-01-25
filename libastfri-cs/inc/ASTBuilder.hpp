#ifndef CSHARP_AST_BUILDER_HPP
#define CSHARP_AST_BUILDER_HPP

#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <vector>

namespace astfri::csharp
{

class ASTBuilder
{
private:
    const TSLanguage* lang_;
    TSParser* parser_;

public:
    ASTBuilder();
    ~ASTBuilder();
    [[nodiscard]] TranslationUnit* make_ast(
        const std::filesystem::path& srcDir
    ) const;

    ASTBuilder(ASTBuilder&)             = delete;
    ASTBuilder(ASTBuilder&&)            = delete;
    ASTBuilder& operator=(ASTBuilder&)  = delete;
    ASTBuilder& operator=(ASTBuilder&&) = delete;

private:
    [[nodiscard]] std::vector<SourceFile> get_source_codes(
        const std::filesystem::path& project_dir
    ) const;
};

} // namespace astfri::csharp

#endif // CSHARP_AST_BUILDER_HPP
