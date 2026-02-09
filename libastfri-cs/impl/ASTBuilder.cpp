#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/visitors/src_code/SrcCodeVisitor.hpp>
#include <libastfri-cs/impl/visitors/SymbolTableBuilder.hpp>
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
    using milli          = std::chrono::milliseconds;
    TranslationUnit* ast = StmtFactory::get_instance().mk_translation_unit();
    // std::cout << "Preprocessing Phase: \n"
    //           << "Gathering souce files and removing comments..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::unique_ptr<SourceFile>> srcs = get_source_codes(srcDir);

    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<milli>(end - start);
    auto total    = duration;
    // std::cout << "Preprocesing complete.\n"
    //           << "Preprocessing took " << duration.count() << " ms"
    //           << std::endl;

    SymbolTable symbTable;
    SymbolTableBuilder symbTableBuilder(srcs, symbTable);

    // std::cout << "Phase 1: Symbol Table Building\n"
    //           << "Discovering user defined types..." << std::endl;

    start = std::chrono::high_resolution_clock::now();

    symbTableBuilder.reg_user_types();
    // std::cout << "Loading using directives...\n";
    symbTableBuilder.reg_using_directives();
    // std::cout << "Discovering members of user defined types...\n";
    symbTableBuilder.reg_members();

    end      = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<milli>(end - start);
    total += duration;

    // std::cout << "Symbol Table Building took " << duration.count() << " ms"
    //           << std::endl;

    SemanticContext semanticContext(symbTable);
    SrcCodeVisitor srcVisitor(srcs, semanticContext, symbTable);

    // std::cout << "Phase 2: Building of AST" << std::endl;

    start = std::chrono::high_resolution_clock::now();

    srcVisitor.visit_comp_unit(*ast);

    end      = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<milli>(end - start);
    total += duration;

    // std::cout << "AST building completed.\n"
    //           << "AST Building took " << duration.count() << " ms\n"
    //           << "Total time: " << total.count() << " ms" << std::endl;

    return ast;
}

std::vector<std::unique_ptr<SourceFile>> ASTBuilder::get_source_codes(
    const std::filesystem::path& project_dir
) const
{
    std::vector<std::unique_ptr<SourceFile>> srcs;
    std::stack<std::filesystem::path> dirs;
    const std::filesystem::path& rootPath{project_dir};
    dirs.emplace(rootPath);

    while (! dirs.empty())
    {
        auto dirIt = std::filesystem::directory_iterator(dirs.top());
        dirs.pop();
        for (auto& dirEntry : dirIt)
        {
            auto& entryPath            = dirEntry.path();
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
                const auto fileSize = file_size(entryPath);
                src.resize(fileSize);
                fileStream.read(
                    src.data(),
                    static_cast<std::streamsize>(fileSize)
                );
                TSTree* tree = util::make_tree(parser_, src);
                TSNode root  = ts_tree_root_node(tree);
                src = util::remove_comments(root, std::move(src), entryPath);
                ts_tree_delete(tree);
                ts_parser_reset(parser_);
                tree = util::make_tree(parser_, src);

                TSNode nNms{};
                util::for_each_match(
                    ts_tree_root_node(tree),
                    regs::QueryType::FileNamespace,
                    [&nNms](const TSQueryMatch& match)
                    { nNms = match.captures[0].node; }
                );
                FileContext context;
                if (! ts_node_is_null(nNms))
                {
                    TSNode nNmsName = util::child_by_field_name(nNms, "name");
                    context.fileNms = util::extract_text(nNmsName, src);
                }
                srcs.emplace_back(
                    std::make_unique<SourceFile>(
                        std::move(context),
                        std::move(src),
                        tree
                    )
                );
                ts_parser_reset(parser_);
            }
        }
    }
    return srcs;
}
} // namespace astfri::csharp