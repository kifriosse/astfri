#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/CSharpTSTreeVisitor.hpp>
#include <libastfri-cs/inc/CSharpASTBuilder.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/tree-sitter-c-sharp.h>

#include <filesystem>
#include <fstream>
#include <stack>

namespace astfri::csharp
{

CSharpASTBuilder::CSharpASTBuilder() :
    lang_(tree_sitter_c_sharp()),
    parser_(ts_parser_new())
{
    ts_parser_set_language(parser_, lang_);
}

CSharpASTBuilder::~CSharpASTBuilder()
{
    ts_language_delete(lang_);
    ts_parser_delete(parser_);
}

TranslationUnit* CSharpASTBuilder::make_ast(
    const std::string& source_code_dir
) const
{
    TranslationUnit* ast = StmtFactory::get_instance().mk_translation_unit();
    const std::vector<std::string> source_codes
        = get_source_codes(source_code_dir);
    for (auto& source_code : source_codes)
    {
        TSTree* const tree = ts_parser_parse_string(
            parser_,
            nullptr,
            source_code.c_str(),
            static_cast<uint32_t>(source_code.size())
        );
        const TSNode root = ts_tree_root_node(tree);

        CSharpTSTreeVisitor cs_ts_tree_visitor(source_code, lang_);
        cs_ts_tree_visitor.handle_comp_unit_stmt(*ast, &root);

        ts_tree_delete(tree);
        ts_parser_reset(parser_);
    }
    return ast;
}

std::vector<std::string> CSharpASTBuilder::get_source_codes(
    const std::string& project_dir
) const
{
    std::vector<std::string> source_codes;
    std::stack<std::string> dirs;
    dirs.push(project_dir);

    while (! dirs.empty())
    {
        auto dir_it = std::filesystem::directory_iterator(dirs.top());
        dirs.pop();
        for (const auto& dir_entry : dir_it)
        {
            if (dir_entry.is_directory())
            {
                const auto& path = dir_entry.path();
                if (path == project_dir + "/bin"
                    || path == project_dir + "/obj")
                {
                    continue;
                }
                dirs.push(path);
            }
            else if (dir_entry.path().extension() == ".cs")
            {
                const std::filesystem::path& path = dir_entry.path();
                std::ifstream file_stream(path);
                std::string source_code(
                    (std::istreambuf_iterator<char>(file_stream)),
                    std::istreambuf_iterator<char>()
                );
                TSTree* tree = ts_parser_parse_string(
                    parser_,
                    nullptr,
                    source_code.c_str(),
                    source_code.length()
                );
                source_codes.emplace_back(remove_comments(
                    source_code,
                    ts_tree_root_node(tree),
                    lang_,
                    path
                ));
                ts_tree_delete(tree);
            }
        }
    }
    return source_codes;
}
} // namespace astfri::csharp