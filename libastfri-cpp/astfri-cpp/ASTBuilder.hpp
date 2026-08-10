#ifndef ASTFRI_CPP_ASTBUILDER_HPP
#define ASTFRI_CPP_ASTBUILDER_HPP

#include <astfri/impl/Concepts.hpp>
#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/StmtFactory.hpp>
#include <astfri/impl/TypeFwd.hpp>

#include <astfri-cpp/Config.hpp>

#include <filesystem>
#include <iosfwd>
#include <string_view>
#include <vector>


namespace astfri::cpp {


class ASTBuilder {
public:
    static ASTBuilder create(astfri::cpp::Config config);

    static std::string_view version();

public:
    astfri::TranslationUnit load_file(std::istream &ist);

    astfri::TranslationUnit load_file(const std::filesystem::path &path);

    std::vector<astfri::TranslationUnit> load_project(const std::filesystem::path &path);

private:
    ASTBuilder(astfri::cpp::Config config);
    ASTBuilder() = delete;
    ASTBuilder(const ASTBuilder &other) = delete;
    ASTBuilder(ASTBuilder &&other) = delete;
    ASTBuilder &operator=(const ASTBuilder &other) = delete;
    ASTBuilder &operator=(ASTBuilder &&other) = delete;

private:
    astfri::cpp::Config m_config;

    astfri::ExprFactory *m_exprFactory;
    astfri::StmtFactory *m_stmtFactory;
    astfri::TypeFactory *m_typeFactory;
};

static_assert(
    astfri::detail::IsInputLib<
        ASTBuilder,
        astfri::cpp::Config,
        rapidjson::Value>);


} // namespace astfri::cpp

#endif // ASTFRI_CPP_ASTBUILDER_HPP
