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
    { Cfg::createFromJson(node) } -> std::same_as<Cfg>;
    { Cfg::createFromJson(path) } -> std::same_as<Cfg>;
    { Cfg::createDefault() } -> std::same_as<Cfg>;
    { Cfg::createFromArgs(argc, argv) } -> std::same_as<Cfg>;
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
    = requires(Cfg cfg, std::istream& ist, const std::filesystem::path& path) {
          requires IsConfigClass<Cfg, JsonNode>;
          requires IsLibInterface<Lib>;
          { Lib::load_file(cfg, ist) } -> std::same_as<astfri::TranslationUnit>;
          { Lib::load_file(cfg, path) } -> std::same_as<astfri::TranslationUnit>;
          { Lib::load_project(cfg, path) } -> std::same_as<astfri::TranslationUnit>;
      };


/**
 * @brief Defines interface for an output library.
 */
template<typename Lib, typename Cfg, typename JsonNode>
concept IsOutputLibInterface
    = requires(Cfg cfg, std::ostream& ost, const astfri::TranslationUnit& root) {
          requires IsConfigClass<Cfg, JsonNode>;
          requires IsLibInterface<Lib>;
          { Lib::process_ast(cfg, root) } -> std::same_as<void>;
          { Lib::process_ast(cfg, root, ost) } -> std::same_as<void>;
      };


} // namespace astfri

#endif
