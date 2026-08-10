#include <astfri/Astfri.hpp>

#include <astfri-cs/impl/data/SymbolTable.hpp>
#include <astfri-cs/impl/regs/Maps.hpp>
#include <astfri-cs/impl/util/AstfriUtil.hpp>
#include <astfri-cs/impl/util/TSUtil.hpp>
#include <astfri-cs/impl/visitors/src_code/SrcCodeTransformer.hpp>
#include <astfri-cs/impl/visitors/SymbTableBuilder.hpp>
#include <astfri-cs/ASTBuilder.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <rapidjson/document.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <vector>


namespace astfri {


namespace csharp {


namespace fs = std::filesystem;


namespace {


const fs::path extTypesRoot                               = ASTFRI_CS_RESOURCES;
const fs::path core                                       = extTypesRoot / "core.json";
const fs::path winDesktop                                 = extTypesRoot / "win-desktop.json";

const std::unordered_map<SDKProfile, fs::path> profileMap = {
    {SDKProfile::None,     core                        },
    {SDKProfile::Core,     core                        },
    {SDKProfile::Worker,   core                        },
    {SDKProfile::Web,      extTypesRoot / "aspnet.json"},
    {SDKProfile::WinForms, winDesktop                  },
    {SDKProfile::WPF,      winDesktop                  }
};


} // namespace


ASTBuilder::ASTBuilder(astfri::csharp::Config config) :
    m_lang(tree_sitter_c_sharp()),
    m_parser(ts_parser_new()),
    m_config(std::move(config)) {
    ts_parser_set_language(m_parser, m_lang);
}

ASTBuilder::~ASTBuilder() {
    ts_language_delete(m_lang);
    ts_parser_delete(m_parser);
}

std::vector<astfri::TranslationUnit> ASTBuilder::load_project(const path& projectDir) {
    if (is_regular_file(projectDir)) {
        if (projectDir.extension() == ".cs") {
            std::ifstream fileStream(projectDir, std::ios::binary);
            this->load_from_stream(fileStream, projectDir);
            return {this->mk_ast()};
        }
        return {};
    }

    std::vector<std::filesystem::path> dirs;
    const std::filesystem::path& rootPath{projectDir};
    dirs.emplace_back(rootPath);

    while (! dirs.empty()) {
        auto dirIt = std::filesystem::directory_iterator(dirs.back());
        dirs.pop_back();
        for (auto& dirEntry : dirIt) {
            auto& entryPath            = dirEntry.path();
            const std::string fileName = entryPath.filename().string();

            if (dirEntry.is_directory()) {
                if (fileName == "bin" || fileName == "obj" || fileName == ".git") {
                    continue;
                }
                dirs.push_back(entryPath);
            }
            else if (entryPath.extension() == ".cs") {
                if (fileName.ends_with(".g.cs") || fileName.ends_with(".i.cs")
                    || fileName.ends_with(".AssemblyInfo.cs")) {
                    continue;
                    }
                std::ifstream fileStream(entryPath, std::ios::binary);
                load_from_stream(fileStream, entryPath);
            }
        }
    }

    return {this->mk_ast()};
}

astfri::TranslationUnit ASTBuilder::load_file(const std::filesystem::path &path) {
    std::ifstream file(path);
    if (! file.is_open()) {
        throw std::runtime_error(std::format("Failed to open `{}`.", path.string()));
    }
    return this->load_file(file);
}

astfri::TranslationUnit ASTBuilder::load_file(std::istream& inputStream) {
    this->load_from_stream(inputStream);
    return this->mk_ast();
}

void ASTBuilder::load_source_of_external_types(const path& jsonPath) {
    m_externalTypeSources.push_back(jsonPath);
}

TranslationUnit ASTBuilder::mk_ast(SDKProfile profile) {
    // using milli          = std::chrono::milliseconds;
    const auto it = profileMap.find(profile);
    if (it == profileMap.end())
        throw std::runtime_error(
            "Profile " + std::to_string(static_cast<size_t>(profile))
            + "doesn't have implemented path"
        );
    load_source_of_external_types(profileMap.at(profile));
    if (profile != SDKProfile::None && profile != SDKProfile::Core
        && profile != SDKProfile::Worker) {
        load_source_of_external_types(profileMap.at(SDKProfile::Core));
        }

    SymbolTable symbTable;
    SymbTableBuilder symbTableBuilder(m_srcs, symbTable);

    for (auto& extTypeSource : m_externalTypeSources) {
        symbTableBuilder.load_external_types(extTypeSource);
    }

    // std::cout << "Phase 1: Symbol Table Building\n"
    //           << "Discovering user defined types..." << std::endl;

    // start = std::chrono::high_resolution_clock::now();

    symbTableBuilder.reg_user_types();
    // symbTableBuilder.collect_types();
    // std::cout << "Loading using directives...\n";
    symbTableBuilder.load_implicit_usings(profile);
    symbTableBuilder.reg_using_directives();
    // std::cout << "Discovering members of user defined types...\n";
    symbTableBuilder.reg_members();

    // end      = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<milli>(end - start);
    // total += duration;
    // std::cout << "Symbol Table Building took " << duration.count() << " ms"
    //           << std::endl;

    SrcCodeTransformer srcVisitor(symbTable);

    // std::cout << "Phase 2: Building of AST" << std::endl;
    // start = std::chrono::high_resolution_clock::now();

    TranslationUnit ast = srcVisitor.visit_comp_unit();

    // end      = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<milli>(end - start);
    // total += duration;
    // std::cout << "AST building completed.\n"
    //           << "AST Building took " << duration.count() << " ms\n"
    //           << "Total time: " << total.count() << " ms" << std::endl;

    return ast;
}

void ASTBuilder::load_from_stream(std::istream& inputStream, const path& path) {
    std::string src((std::istreambuf_iterator(inputStream)), std::istreambuf_iterator<char>());
    TSTree* tree      = util::make_tree(m_parser, src);
    const TSNode root = ts_tree_root_node(tree);
    src               = util::remove_comments(root, std::move(src), path);
    ts_tree_delete(tree);
    ts_parser_reset(m_parser);
    tree = util::make_tree(m_parser, src);

    TSNode nNms{};
    util::for_each_match(
        ts_tree_root_node(tree),
        maps::QueryType::FileNamespace,
        [&nNms](const TSQueryMatch& match) { nNms = match.captures[0].node; }
    );
    Scope fileNms{};
    if (! ts_node_is_null(nNms)) {
        const TSNode nNmsName       = util::child_by_field_name(nNms, "name");
        const std::string nmsQualif = util::extract_text(nNmsName, src);
        fileNms                     = util::mk_scope(nmsQualif);
    }
    m_srcs.emplace_back(std::make_unique<SourceFile>(std::move(src), tree, std::move(fileNms)));
    ts_parser_reset(m_parser);
}

ASTBuilder ASTBuilder::create(astfri::csharp::Config config) {
    return ASTBuilder(std::move(config));
}

std::string_view ASTBuilder::version() {
    return ASTFRI_CS_VERSION;
}


} // namespace astfri::csharp


} // namespace astfri

