#ifndef LIBASTFRI_IMPL_CONCEPTS_FWD_HPP
#define LIBASTFRI_IMPL_CONCEPTS_FWD_HPP

#include <concepts>
#include <filesystem>

#include "StmtFwd.hpp"
#include "libastfri/inc/Stmt.hpp"

namespace rapidjson
{

class Value;

} // namespace rapidjson


namespace astfri
{


/**
 * @brief TODO
 */
template<typename C>
concept IsConfigClass = requires(
    C cfg,
    const rapidjson::Value &node,
    const std::filesystem::path &path,
    int argc,
    char *argv[]
)
{
    { C::createFromJson(node)       } -> std::same_as<C>;
    { C::createFromJson(path)       } -> std::same_as<C>;
    { C::createDefault()            } -> std::same_as<C>;
    { C::createFromArgs(argc, argv) } -> std::same_as<C>;
};


/**
 * @brief TODO
 */
template<typename Lib, typename Cfg>
concept IsInputLibInterface = requires(
    Cfg cfg,
    std::istream &ist,
    const std::filesystem::path &path
)
{
    IsConfigClass<Cfg>;
    { load_file(cfg, ist) } -> std::same_as<astfri::TranslationUnit>;
    { load_file(cfg, path) } -> std::same_as<astfri::TranslationUnit>;
    { load_project(cfg, path) } -> std::same_as<astfri::TranslationUnit>;
};


/**
 * @brief TODO
 */
template<typename Lib, typename Cfg>
concept IsOutputLibInterface = requires(
    Cfg cfg,
    std::ostream &ost,
    const astfri::TranslationUnit &root
)
{
    IsConfigClass<Cfg>;
    { process_ast(cfg, root) } -> std::same_as<void>;
    { process_ast(cfg, root, ost) } -> std::same_as<void>;
};


} // namespace astfri

#endif
