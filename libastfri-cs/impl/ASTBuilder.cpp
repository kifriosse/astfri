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
// #include <iostream>
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

void ASTBuilder::load_src(const path& projectDir)
{
    std::stack<std::filesystem::path> dirs;
    const std::filesystem::path& rootPath{projectDir};
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
                std::ifstream fileStream(entryPath, std::ios::binary);
                load_from_stream(fileStream, entryPath);
            }
        }
    }
}

void ASTBuilder::load_src(std::istream& inputStream)
{
    load_from_stream(inputStream);
}

TranslationUnit* ASTBuilder::mk_ast()
{
    // using milli          = std::chrono::milliseconds;
    TranslationUnit* ast = StmtFactory::get_instance().mk_translation_unit();
    // std::cout << "Preprocessing Phase: \n"
    //           << "Gathering souce files and removing comments..." <<
    //           std::endl;
    // auto start = std::chrono::high_resolution_clock::now();
    // auto end      = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<milli>(end - start);
    // auto total    = duration;
    // std::cout << "Preprocesing complete.\n"
    //           << "Preprocessing took " << duration.count() << " ms"
    //           << std::endl;

    SymbolTable symbTable;
    SymbolTableBuilder symbTableBuilder(srcs_, symbTable);

    // std::cout << "Phase 1: Symbol Table Building\n"
    //           << "Discovering user defined types..." << std::endl;

    // start = std::chrono::high_resolution_clock::now();

    symbTableBuilder.reg_user_types();
    // std::cout << "Loading using directives...\n";
    symbTableBuilder.reg_using_directives();
    // std::cout << "Discovering members of user defined types...\n";
    symbTableBuilder.reg_members();

    // end      = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<milli>(end - start);
    // total += duration;
    // std::cout << "Symbol Table Building took " << duration.count() << " ms"
    //           << std::endl;

    SemanticContext semanticContext(symbTable);
    SrcCodeVisitor srcVisitor(srcs_, semanticContext, symbTable);

    // std::cout << "Phase 2: Building of AST" << std::endl;
    // start = std::chrono::high_resolution_clock::now();

    srcVisitor.visit_comp_unit(*ast);

    // end      = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<milli>(end - start);
    // total += duration;
    // std::cout << "AST building completed.\n"
    //           << "AST Building took " << duration.count() << " ms\n"
    //           << "Total time: " << total.count() << " ms" << std::endl;

    return ast;
}

void ASTBuilder::load_from_stream(std::istream& inputStream, const path& path)
{
    std::string src(
        (std::istreambuf_iterator(inputStream)),
        std::istreambuf_iterator<char>()
    );
    TSTree* tree      = util::make_tree(parser_, src);
    const TSNode root = ts_tree_root_node(tree);
    src               = util::remove_comments(root, std::move(src), path);
    ts_tree_delete(tree);
    ts_parser_reset(parser_);
    tree = util::make_tree(parser_, src);

    TSNode nNms{};
    util::for_each_match(
        ts_tree_root_node(tree),
        regs::QueryType::FileNamespace,
        [&nNms](const TSQueryMatch& match) { nNms = match.captures[0].node; }
    );
    FileContext context;
    if (! ts_node_is_null(nNms))
    {
        const TSNode nNmsName = util::child_by_field_name(nNms, "name");
        context.fileNms       = util::extract_text(nNmsName, src);
    }
    srcs_.emplace_back(
        std::make_unique<SourceFile>(std::move(context), std::move(src), tree)
    );
    ts_parser_reset(parser_);
}
} // namespace astfri::csharp