#ifndef CSHARP_AST_BUILDER_HPP
#define CSHARP_AST_BUILDER_HPP

#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <string>
#include <vector>

namespace astfri::csharp
{
class CSharpASTBuilder
{
private:
    const TSLanguage* lang_;
    TSParser* parser_;

public:
    CSharpASTBuilder();

    ~CSharpASTBuilder();
    TranslationUnit* make_ast(const std::string& source_code_dir) const;

private:
    static std::vector<std::string> get_source_codes(
        const std::string& project_dir
    );
};

}; // namespace astfri::csharp

#endif // CSHARP_AST_BUILDER_HPP
