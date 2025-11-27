#ifndef CSHARP_AST_BUILDER_HPP
#define CSHARP_AST_BUILDER_HPP

#include <libastfri/inc/Astfri.hpp>

#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>
#include <vector>

namespace astfri::csharp
{
class CSharpASTBuilder
{
private:
    TSLanguage const* lang_;
    TSParser* parser_;

public:
    CSharpASTBuilder() :
        lang_(tree_sitter_c_sharp()),
        parser_(ts_parser_new())
    {
        ts_parser_set_language(parser_, lang_);
    };

    ~CSharpASTBuilder();
    TranslationUnit* make_ast(std::string const& source_code_dir) const;

private:
    static std::vector<std::string> get_source_codes(std::string const& project_dir);
};

}; // namespace astfri::csharp

#endif // CSHARP_AST_BUILDER_HPP
