#ifndef CSHARP_AST_BUILDER_HPP
#define CSHARP_AST_BUILDER_HPP

#include <libastfri-cs/impl/Source.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <string>
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
        const std::string& source_code_dir
    ) const;

    ASTBuilder(ASTBuilder&)             = delete;
    ASTBuilder(ASTBuilder&&)            = delete;
    ASTBuilder& operator=(ASTBuilder&)  = delete;
    ASTBuilder& operator=(ASTBuilder&&) = delete;

private:
    [[nodiscard]] std::vector<SourceFile> get_source_codes(
        const std::string& project_dir
    ) const;
};

} // namespace astfri::csharp

#endif // CSHARP_AST_BUILDER_HPP
