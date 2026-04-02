#ifndef ASTFRI_IMPL_CONCEPTS_HPP
#define ASTFRI_IMPL_CONCEPTS_HPP

#include <astfri/impl/StmtFwd.hpp>

#include <concepts>
#include <filesystem>


namespace astfri {

/**
 * @brief TODO
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
 * @brief TODO
 */
template<typename Lib, typename Cfg, typename JsonNode>
concept IsInputLibInterface
    = requires(Cfg cfg, std::istream& ist, const std::filesystem::path& path) {
          IsConfigClass<Cfg, JsonNode>;
          { load_file(cfg, ist) } -> std::same_as<astfri::TranslationUnit>;
          { load_file(cfg, path) } -> std::same_as<astfri::TranslationUnit>;
          { load_project(cfg, path) } -> std::same_as<astfri::TranslationUnit>;
      };

/**
 * @brief TODO
 */
template<typename Lib, typename Cfg, typename JsonNode>
concept IsOutputLibInterface
    = requires(Cfg cfg, std::ostream& ost, const astfri::TranslationUnit& root) {
          IsConfigClass<Cfg, JsonNode>;
          { process_ast(cfg, root) } -> std::same_as<void>;
          { process_ast(cfg, root, ost) } -> std::same_as<void>;
      };

} // namespace astfri

#endif
