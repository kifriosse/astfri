#ifndef ASTFRI_JAVA_ASTBUILDER_HPP
#define ASTFRI_JAVA_ASTBUILDER_HPP

#include <astfri/impl/Concepts.hpp>
#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/StmtFactory.hpp>
#include <astfri/impl/TypeFwd.hpp>

#include <astfri-java/Config.hpp>
#include <astfri-java/impl/ExpressionTransformer.hpp>
#include <astfri-java/impl/NodeMapper.hpp>
#include <astfri-java/impl/StatementTransformer.hpp>

#include <filesystem>
#include <iosfwd>
#include <string_view>
#include <vector>


namespace astfri::java {


class ASTBuilder {
public:
    static ASTBuilder create(astfri::java::Config config);

    static std::string_view version();

public:
    astfri::TranslationUnit load_file(std::istream &ist);

    astfri::TranslationUnit load_file(const std::filesystem::path &path);

    std::vector<astfri::TranslationUnit> load_project(const std::filesystem::path &path);

private:
    ASTBuilder(astfri::java::Config config);
    ASTBuilder() = delete;
    ASTBuilder(const ASTBuilder &other) = delete;
    ASTBuilder(ASTBuilder &&other) = delete;
    ASTBuilder &operator=(const ASTBuilder &other) = delete;
    ASTBuilder &operator=(ASTBuilder &&other) = delete;

private:
    astfri::java::Config m_config;

    astfri::ExprFactory *m_exprFactory;
    astfri::StmtFactory *m_stmtFactory;
    astfri::TypeFactory *m_typeFactory;

    NodeMapper m_nodeMapper;
    ExpressionTransformer m_exprTransformer;
    StatementTransformer m_stmtTransformer;
};

static_assert(
    astfri::detail::IsInputLib<
        ASTBuilder,
        astfri::java::Config,
        rapidjson::Value>);


} // namespace astfri::java

#endif // ASTFRI_JAVA_ASTBUILDER_HPP
