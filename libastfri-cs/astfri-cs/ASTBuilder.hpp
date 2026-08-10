#ifndef ASTFRI_CSHARP_AST_BUILDER_HPP
#define ASTFRI_CSHARP_AST_BUILDER_HPP

#include <astfri/Astfri.hpp>
#include <astfri/impl/Concepts.hpp>

#include <astfri-cs/Config.hpp>
#include <astfri-cs/SDKProfile.hpp>
#include <astfri-cs/impl/data/SourceFile.hpp>

#include <tree_sitter/api.h>

#include <rapidjson/document.h>
#include <vector>


namespace astfri {


namespace csharp {


/**
 * @brief Class responsible for loading C# source code and building AST from it.
 * @note This class isn't copyable or movable
 */
class ASTBuilder {
public:
    static ASTBuilder create(astfri::csharp::Config config);

    static std::string_view version();

private:
    using path = std::filesystem::path;
    const TSLanguage* m_lang;
    TSParser* m_parser;
    std::vector<std::unique_ptr<SourceFile>> m_srcs;
    std::vector<std::filesystem::path> m_externalTypeSources;
    astfri::csharp::Config m_config;

public:
    ASTBuilder(astfri::csharp::Config config);
    ~ASTBuilder();
    ASTBuilder(const ASTBuilder& other)      = delete;
    ASTBuilder(ASTBuilder&& other)           = delete;
    ASTBuilder& operator=(ASTBuilder& other) = delete;
    ASTBuilder& operator=(ASTBuilder&&other) = delete;

    /**
     * @brief Loads souce code of a project into AST Builder
     * @param projectDir path to directory that contains C# source code
     */
    std::vector<astfri::TranslationUnit> load_project(const path& projectDir);

    /**
     * @brief Loads souce code from a file
     * @param path Path to the file with C# source code
     */
    astfri::TranslationUnit load_file(const path &path);

    /**
     * @brief Loads souce code from a stream into AST Builder
     * @param inputStream stream containing source code
     */
    astfri::TranslationUnit load_file(std::istream& inputStream);

    /**
     * @brief Loads a path to JSON with external types into AST Builder
     * @param jsonPath path to JSON file with external types
     */
    void load_source_of_external_types(const path& jsonPath);

    /**
     * @brief Makes AST from loaded source code
     * @param profile SDK profile to determine with standard implicit using to
     * use. Default value is \c SDKProfile::NET which corresponds to .NET SDK
     * profile.
     * @return AST from loaded source code
     */
    TranslationUnit mk_ast(SDKProfile profile = SDKProfile::Core);

private:
    /**
     * @brief Loads source code from a stream into AST Builder
     * @param inputStream stream containing source code
     * @param path path to source file being loaded. Used for error reporting.
     * Optional parameter, default value is empty path.
     */
    void load_from_stream(std::istream& inputStream, const path& path = {});
};


static_assert(
    astfri::detail::IsInputLib<
        astfri::csharp::ASTBuilder,
        astfri::csharp::Config,
        rapidjson::Value>);


} // namespace astfri::csharp


} // namespace astfri

#endif // CSHARP_AST_BUILDER_HPP
