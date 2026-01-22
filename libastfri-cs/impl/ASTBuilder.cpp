#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/SymbolTableBuilder.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
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

TranslationUnit* ASTBuilder::make_ast(const std::filesystem::path& srcDir) const
{
    TranslationUnit* ast = StmtFactory::get_instance().mk_translation_unit();
    std::vector<SourceCode> srcs;

    for (auto& srcFile : get_source_codes(srcDir))
    {
        TSTree* tree = ts_parser_parse_string(
            parser_,
            nullptr,
            srcFile.content.c_str(),
            static_cast<uint32_t>(srcFile.content.size())
        );
        srcs.emplace_back(srcFile, tree);
        ts_parser_reset(parser_);
    }

    using milli = std::chrono::milliseconds;

    SymbolTable symbTable;
    SymbolTableBuilder symbTableBuilder(srcs, symbTable);

    std::cout << "Phase 1: Symbol Table Building\n"
              << "Discovering user defined types..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    symbTableBuilder.reg_user_types();
    std::cout << "Loading using directives...\n";
    symbTableBuilder.reg_using_directives();
    std::cout << "Discovering members of user defined types...\n";
    symbTableBuilder.reg_members();

    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<milli>(end - start);

    std::cout << "Symbol Table Building took " << duration.count() << " ms"
              << std::endl;

    SemanticContext semanticContext(symbTable);
    SrcCodeVisitor srcVisitor(srcs, semanticContext, symbTable);

    std::cout << "Phase 2: Building of AST" << std::endl;

    start = std::chrono::high_resolution_clock::now();

    srcVisitor.visit_comp_unit(*ast);

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
    std::vector<SourceFile> srcFiles;
    std::stack<std::filesystem::path> dirs;
    const std::filesystem::path& rootPath{project_dir};
    dirs.emplace(rootPath);

    while (! dirs.empty())
    {
        auto dirIt = std::filesystem::directory_iterator(dirs.top());
        dirs.pop();
        for (const auto& dirEntry : dirIt)
        {
            const std::filesystem::path& entryPath = dirEntry.path();
            const std::string fileName = entryPath.filename().string();

            if (dirEntry.is_directory())
            {
                if (fileName == "bin" || fileName == "obj"
                    || fileName == ".git")
                {
                    continue;
                }
                dirs.push(entryPath);
            }
            else if (entryPath.extension() == ".cs")
            {
                if (fileName.ends_with(".g.cs") || fileName.ends_with(".i.cs")
                    || fileName.ends_with(".AssemblyInfo.cs"))
                {
                    continue;
                }
                std::string src;
                std::ifstream fileStream(entryPath, std::ios::binary);
                const auto fileSize = std::filesystem::file_size(entryPath);
                src.resize(fileSize);
                fileStream.read(
                    src.data(),
                    static_cast<std::streamsize>(fileSize)
                );
                TSTree* tree = ts_parser_parse_string(
                    parser_,
                    nullptr,
                    src.c_str(),
                    src.length()
                );
                srcFiles.emplace_back(
                    entryPath,
                    util::remove_comments(
                        ts_tree_root_node(tree),
                        src,
                        entryPath
                    )
                );
                ts_tree_delete(tree);
            }
        }
    }
    return srcFiles;
}
} // namespace astfri::csharp