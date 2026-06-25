#ifndef ASTFRI_JAVA_ASTBUILDER_HPP
#define ASTFRI_JAVA_ASTBUILDER_HPP

#include <astfri/impl/ExprFactory.hpp>
#include <astfri/impl/StmtFactory.hpp>
#include <astfri/impl/TypeFactory.hpp>

#include <astfri-java/Config.hpp>
#include <astfri-java/impl/ExpressionTransformer.hpp>
#include <astfri-java/impl/NodeMapper.hpp>
#include <astfri-java/impl/StatementTransformer.hpp>

#include <filesystem>
#include <iosfwd>
#include <string>
#include <vector>


namespace astfri::java {


class ASTBuilder {
public:
    static ASTBuilder create(astfri::java::Config config);

public:
    astfri::TranslationUnit load_file(std::istream &ist);

    astfri::TranslationUnit load_file(const std::filesystem::path &path);

    std::vector<astfri::TranslationUnit> load_project(const std::filesystem::path &path);

private:
    ASTBuilder(astfri::java::Config config);

private:
    astfri::java::Config m_config;

    astfri::ExprFactory *m_exprFactory;
    astfri::StmtFactory *m_stmtFactory;
    astfri::TypeFactory *m_typeFactory;

    NodeMapper m_nodeMapper;
    ExpressionTransformer m_exprTransformer;
    StatementTransformer m_stmtTransformer;
};


} // namespace astfri::java

#endif // ASTFRI_JAVA_ASTBUILDER_HPP
