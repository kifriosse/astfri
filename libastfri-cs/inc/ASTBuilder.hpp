#ifndef CSHARP_AST_BUILDER_HPP
#define CSHARP_AST_BUILDER_HPP

#include <astfri/Astfri.hpp>
#include <astfri/impl/Concepts.hpp>

#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri-cs/inc/Config.hpp>
#include <libastfri-cs/inc/SDKProfile.hpp>

#include <tree_sitter/api.h>

#include <external/rapidjson/include/rapidjson/document.h>
#include <vector>

namespace astfri {

namespace csharp {

/**
 * @brief Class responsible for loading C# source code and building AST from it.
 * @note This class isn't copyable or movable
 */
class ASTBuilder {
private:
    using path = std::filesystem::path;
    const TSLanguage* lang_;
    TSParser* parser_;
    std::vector<std::unique_ptr<SourceFile>> srcs_;
    std::vector<std::filesystem::path> externalTypeSources_;

public:
    ASTBuilder();
    ~ASTBuilder();
    ASTBuilder(ASTBuilder&)             = delete;
    ASTBuilder(ASTBuilder&&)            = delete;
    ASTBuilder& operator=(ASTBuilder&)  = delete;
    ASTBuilder& operator=(ASTBuilder&&) = delete;

    /**
     * @brief Loads souce code of a project into AST Builder
     * @param projectDir path to directory that contains C# source code
     */
    void load_src(const path& projectDir);
    /**
     * @brief Loads souce code from a stream into AST Builder
     * @param inputStream stream containing source code
     */
    void load_src(std::istream& inputStream);
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
    TranslationUnit* mk_ast(SDKProfile profile = SDKProfile::Core);

private:
    /**
     * @brief Loads source code from a stream into AST Builder
     * @param inputStream stream containing source code
     * @param path path to source file being loaded. Used for error reporting.
     * Optional parameter, default value is empty path.
     */
    void load_from_stream(std::istream& inputStream, const path& path = {});
};

} // namespace astfri::csharp

class CSharpOutput {
public:
    static TranslationUnit load_file(csharp::Config cfg, std::istream& ist);
    static TranslationUnit load_file(csharp::Config cfg, const std::filesystem::path& path);
    static TranslationUnit load_project(csharp::Config cfg, const std::filesystem::path& path);

private:
    template<typename Source>
    static TranslationUnit load(csharp::Config& cfg, Source& source);
};

} // namespace astfri

static_assert(astfri::IsInputLibInterface<astfri::CSharpOutput, astfri::csharp::Config ,rapidjson::Value>);

#include <libastfri-cs/impl/ASTBuilder.inl>

#endif // CSHARP_AST_BUILDER_HPP
