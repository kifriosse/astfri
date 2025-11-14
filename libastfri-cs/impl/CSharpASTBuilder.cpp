
#include "CSharpASTBuilder.hpp"

#include <filesystem>
#include <fstream>
#include <stack>

namespace astfri::csharp
{
CSharpASTBuilder::~CSharpASTBuilder()
{
    ts_parser_delete(parser_);
    ts_language_delete(lang_);
}

std::vector<TranslationUnit*> CSharpASTBuilder::make_ast(std::string source_code_dir)
{

}

TranslationUnit* CSharpASTBuilder::make_translation_unit(std::string const& source_code)
{
    TSTree* const tree = ts_parser_parse_string(
        parser_,
        nullptr,
        source_code.c_str(),
        source_code.length()
    );
    CSharpTSTreeVisitor cs_ts_tree_visitor(source_code, lang_);
}

std::vector<std::string> CSharpASTBuilder::get_source_codes(std::string const& source_code_dir)
{
    std::vector<std::string> source_codes;
    std::stack<std::string> dirs;
    dirs.push(source_code_dir);

    while (!dirs.empty())
    {
        auto dir_it = std::filesystem::directory_iterator(dirs.top());
        dirs.pop();
        for (auto const& dir_entry : dir_it)
        {
            if (dir_entry.is_directory())
            {
                dirs.push(dir_entry.path());
            }
            else if (dir_entry.path().extension() == ".cs")
            {
                std::ifstream file_stream(dir_entry.path());
                std::string content((std::istreambuf_iterator<char>(file_stream)),
                                     std::istreambuf_iterator<char>());
                source_codes.emplace_back(std::move(content));
            }
        }
    }
    return source_codes;
}
}