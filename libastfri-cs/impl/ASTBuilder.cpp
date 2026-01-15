#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/SymbolTableBuilder.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
    ts_language_delete(lang_);
    ts_parser_delete(parser_);
}

TranslationUnit* ASTBuilder::make_ast(
    const std::filesystem::path& source_code_dir
) const
{
    TranslationUnit* ast = StmtFactory::get_instance().mk_translation_unit();
    std::vector<SourceCode> srcs;

    for (auto& source_file : get_source_codes(source_code_dir))
    {
        TSTree* tree = ts_parser_parse_string(
            parser_,
            nullptr,
            source_file.content.c_str(),
            static_cast<uint32_t>(source_file.content.size())
        );
        srcs.emplace_back(source_file, tree);
        ts_parser_reset(parser_);
    }

    using milli = std::chrono::milliseconds;

    SymbolTable symb_table;
    SymbolTableBuilder symbol_table_builder(srcs, symb_table);

    std::cout << "Phase 1: Symbol Table Building\n"
              << "Discovering user defined types..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    symbol_table_builder.reg_user_types();
    std::cout << "Loading using directives...\n";
    symbol_table_builder.reg_using_directives();
    std::cout << "Discovering members of user defined types...\n";
    symbol_table_builder.reg_members();

    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<milli>(end - start);

    std::cout << "Symbol Table Building took " << duration.count() << " ms"
              << std::endl;

    SemanticContext glob_sem_context(symb_table);
    SrcCodeVisitor source_visitor(srcs, glob_sem_context, symb_table);

    std::cout << "Phase 2: Building of AST" << std::endl;

    start = std::chrono::high_resolution_clock::now();

    source_visitor.visit_comp_unit(*ast);

    end      = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<milli>(end - start);

    std::cout << "AST building completed.\n"
              << "AST Building took " << duration.count() << " ms" << std::endl;

    return ast;
}

std::vector<SourceFile> ASTBuilder::get_source_codes(
    const std::filesystem::path& project_dir
) const
{
    std::vector<SourceFile> source_files;
    std::stack<std::filesystem::path> dirs;
    const std::filesystem::path& root_path{project_dir};
    dirs.emplace(root_path);

    while (! dirs.empty())
    {
        auto dir_it = std::filesystem::directory_iterator(dirs.top());
        dirs.pop();
        for (const auto& dir_entry : dir_it)
        {
            std::filesystem::path entry_path = dir_entry.path();
            const auto file_name             = entry_path.filename().string();

            if (dir_entry.is_directory())
            {
                if (file_name == "bin" || file_name == "obj"
                    || file_name == ".git")
                {
                    continue;
                }
                dirs.push(entry_path);
            }
            else if (entry_path.extension() == ".cs")
            {
                if (file_name.ends_with(".g.cs") || file_name.ends_with(".i.cs")
                    || file_name.ends_with(".AssemblyInfo.cs"))
                {
                    continue;
                }
                std::string source_code;
                std::ifstream file_stream(entry_path, std::ios::binary);
                const auto file_size = std::filesystem::file_size(entry_path);
                source_code.resize(file_size);
                file_stream.read(
                    source_code.data(),
                    static_cast<std::streamsize>(file_size)
                );
                TSTree* tree = ts_parser_parse_string(
                    parser_,
                    nullptr,
                    source_code.c_str(),
                    source_code.length()
                );
                source_files.emplace_back(
                    entry_path,
                    util::remove_comments(
                        ts_tree_root_node(tree),
                        source_code,
                        lang_,
                        entry_path
                    )
                );
                ts_tree_delete(tree);
            }
        }
    }
    return source_files;
}
} // namespace astfri::csharp