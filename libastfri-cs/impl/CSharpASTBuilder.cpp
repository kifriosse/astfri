
#include "CSharpASTBuilder.hpp"

#include <filesystem>
#include <fstream>
#include <stack>

namespace astfri::csharp
{

CSharpASTBuilder::~CSharpASTBuilder()
{
    ts_language_delete(lang_);
    ts_parser_delete(parser_);
}

TranslationUnit* CSharpASTBuilder::make_ast(std::string const& source_code_dir) const
{
    TranslationUnit* ast                        = StmtFactory::get_instance().mk_translation_unit();
    std::vector<std::string> const source_codes = get_source_codes(source_code_dir);
    for (auto& source_code : source_codes)
    {
        // TSParser* local_parser = ts_parser_new();
        // ts_parser_set_language(local_parser, lang_);
        TSTree* const tree = ts_parser_parse_string(
            // local_parser,
            parser_,
            nullptr,
            source_code.c_str(),
            static_cast<uint32_t>(source_code.size())
        );
        TSNode const root = ts_tree_root_node(tree);

        CSharpTSTreeVisitor cs_ts_tree_visitor(source_code, lang_);
        cs_ts_tree_visitor.handle_comp_unit_stmt(*ast, &root);

        ts_tree_delete(tree);
        // ts_parser_delete(local_parser);
        ts_parser_reset(parser_);
    }
    return ast;
}

std::vector<std::string> CSharpASTBuilder::get_source_codes(std::string const& source_code_dir)
{
    std::vector<std::string> source_codes;
    std::stack<std::string> dirs;
    dirs.push(source_code_dir);

    while (! dirs.empty())
    {
        auto dir_it = std::filesystem::directory_iterator(dirs.top());
        dirs.pop();
        for (auto const& dir_entry : dir_it)
        {
            if (dir_entry.is_directory())
            {
                auto const& path = dir_entry.path();
                if (path == source_code_dir + "/bin" || path == source_code_dir + "/obj")
                {
                    continue;
                }
                dirs.push(path);
            }
            else if (dir_entry.path().extension() == ".cs")
            {
                std::ifstream file_stream(dir_entry.path());
                std::string content(
                    (std::istreambuf_iterator<char>(file_stream)),
                    std::istreambuf_iterator<char>()
                );
                source_codes.emplace_back(std::move(content));
            }
        }
    }
    return source_codes;
}
} // namespace astfri::csharp