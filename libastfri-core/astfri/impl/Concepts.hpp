#ifndef ASTFRI_IMPL_CONCEPTS_HPP
#define ASTFRI_IMPL_CONCEPTS_HPP

#include <astfri/impl/StmtFwd.hpp>

#include <concepts>
#include <filesystem>
#include <string_view>


namespace astfri {


/**
 * @brief Defines interface for a config class.
 */
template<typename Cfg, typename JsonNode>
concept IsConfigClass = requires(
    Cfg cfg,
    const JsonNode &node,
    const std::filesystem::path& path,
    int argc,
    char* argv[]
) {
    { Cfg::create_from_json(node) } -> std::same_as<Cfg>;
    { Cfg::create_from_json(path) } -> std::same_as<Cfg>;
    { Cfg::create_default() } -> std::same_as<Cfg>;
    { Cfg::create_from_args(argc, argv) } -> std::same_as<Cfg>;
    cfg.write_json(path);
};


/**
 * @brief Defines interface for each library type.
 */
template<typename Lib>
concept IsLibInterface = requires() {
    { Lib::version() } -> std::same_as<std::string_view>;
};


/**
 * @brief Defines interface for an input library.
 */
template<typename Lib, typename Cfg, typename JsonNode>
concept IsInputLibInterface
    = requires(std::istream& ist, const std::filesystem::path& path) {
          requires IsConfigClass<Cfg, JsonNode>;
          requires IsLibInterface<Lib>;
          { Lib::load_file(Cfg::create_default(), ist) } -> std::same_as<astfri::TranslationUnit>;
          { Lib::load_file(Cfg::create_default(), path) } -> std::same_as<astfri::TranslationUnit>;
          { Lib::load_project(Cfg::create_default(), path) } -> std::same_as<astfri::TranslationUnit>;
      };


/**
 * @brief Defines interface for an output library.
 */
template<typename Lib, typename Cfg, typename JsonNode>
concept IsOutputLibInterface
    = requires(std::ostream& ost, const astfri::TranslationUnit& root) {
          requires IsConfigClass<Cfg, JsonNode>;
          requires IsLibInterface<Lib>;
          { Lib::process_ast(Cfg::create_default(), root) } -> std::same_as<void>;
          { Lib::process_ast(Cfg::create_default(), root, ost) } -> std::same_as<void>;
      };


} // namespace astfri

#endif
