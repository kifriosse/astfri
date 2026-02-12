#ifndef CSHARP_AST_BUILDER_HPP
#define CSHARP_AST_BUILDER_HPP

#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <vector>

namespace astfri::csharp
{

class ASTBuilder
{
private:
    using path = std::filesystem::path;
    const TSLanguage* lang_;
    TSParser* parser_;
    std::vector<std::unique_ptr<SourceFile>> srcs_;

public:
    ASTBuilder();
    ~ASTBuilder();
    ASTBuilder(ASTBuilder&)             = delete;
    ASTBuilder(ASTBuilder&&)            = delete;
    ASTBuilder& operator=(ASTBuilder&)  = delete;
    ASTBuilder& operator=(ASTBuilder&&) = delete;

    /**
     * Loads souce code of a project into AST Builder
     * @param projectDir path to directory that contains C# source code
     */
    void load_src(const path& projectDir);
    /**
     * Loads souce code from a stream
     * @param inputStream stream containing source code
     */
    void load_src(std::istream& inputStream);
    /**
     * Makes AST from loaded source code
     * @return AST from loaded source code
     */
    TranslationUnit* mk_ast();

private:
    void load_from_stream(std::istream& inputStream, const path& path = {});
};

} // namespace astfri::csharp

#endif // CSHARP_AST_BUILDER_HPP
