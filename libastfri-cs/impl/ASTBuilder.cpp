#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/SourceCodeVisitor.hpp>
#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/tree-sitter-c-sharp.h>

#include <filesystem>
#include <fstream>
#include <ranges>
#include <stack>

namespace astfri::csharp
{

ASTBuilder::ASTBuilder() :
    lang_(tree_sitter_c_sharp()),
    parser_(ts_parser_new())
{
    ts_parser_set_language(parser_, lang_);
}

ASTBuilder::~ASTBuilder()
{
    for (const auto& tree : source_trees_ | std::views::values)
    {
        ts_tree_delete(tree);
    }
    ts_language_delete(lang_);
    ts_parser_delete(parser_);
}

TranslationUnit* ASTBuilder::make_ast(const std::string& source_code_dir)
{
    TranslationUnit* ast = StmtFactory::get_instance().mk_translation_unit();
    const std::vector<SourceFile> source_files
        = get_source_codes(source_code_dir);
    for (auto& [file_path, source_code] : source_files)
    {
        TSTree* tree = ts_parser_parse_string(
            parser_,
            nullptr,
            source_code.c_str(),
            static_cast<uint32_t>(source_code.size())
        );
        const TSNode root = ts_tree_root_node(tree);

        source_trees_.emplace(file_path, tree);
        SourceCodeVisitor cs_ts_tree_visitor(source_code, lang_);
        cs_ts_tree_visitor.handle_comp_unit_stmt(*ast, &root);

        ts_parser_reset(parser_);
    }
    return ast;
}

std::vector<SourceFile> ASTBuilder::get_source_codes(
    const std::string& project_dir
) const
{
    std::vector<SourceFile> source_files;
    std::stack<std::string> dirs;
    dirs.push(project_dir);

    while (! dirs.empty())
    {
        auto dir_it = std::filesystem::directory_iterator(dirs.top());
        dirs.pop();
        for (const auto& dir_entry : dir_it)
        {
            std::filesystem::path current_path = dir_entry.path();
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
            else if (current_path.extension() == ".cs")
            {
                const std::filesystem::path& path = dir_entry.path();
                std::ifstream file_stream(path);
                std::string source_code(
                    (std::istreambuf_iterator(file_stream)),
                    std::istreambuf_iterator<char>()
                );
                TSTree* tree = ts_parser_parse_string(
                    parser_,
                    nullptr,
                    source_code.c_str(),
                    source_code.length()
                );
                source_files.emplace_back(
                    current_path,
                    remove_comments(
                        source_code,
                        ts_tree_root_node(tree),
                        lang_,
                        path
                    )
                );
                ts_tree_delete(tree);
            }
        }
    }
    return source_files;
}
} // namespace astfri::csharp