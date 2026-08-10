#ifndef ASTFRI_IMPL_CONCEPTS_HPP
#define ASTFRI_IMPL_CONCEPTS_HPP

#include <astfri/impl/StmtFwd.hpp>

#include <concepts>
#include <filesystem>
#include <string_view>
#include <vector>


namespace astfri::detail {


/**
 * @brief Defines interface for a config class.
 *
 * Forces the factory function usage for construcion, though, technically does not prevent
 * constructors with parameters.
 */
template<typename Cfg, typename JsonNode>
concept IsConfig =
    std::copyable<Cfg> &&
    !std::default_initializable<Cfg> &&
    requires(
        const JsonNode &node,
        const std::filesystem::path &path,
        int argc,
        char *argv[]
    ) {
        { Cfg::create_from_json(node) } -> std::same_as<Cfg>;
        { Cfg::create_from_json(path) } -> std::same_as<Cfg>;
        { Cfg::create_default() } -> std::same_as<Cfg>;
        { Cfg::create_from_args(argc, argv) } -> std::same_as<Cfg>;
        { Cfg::create_default().write_json(path) } -> std::same_as<void>;
    };


/**
 * @brief Defines interface for a library.
 */
template<typename Lib, typename Cfg>
concept IsLib =
    !std::copyable<Lib> &&
    !std::movable<Lib> &&
    !std::default_initializable<Lib> &&
    requires() {
        { Lib::version() } -> std::same_as<std::string_view>;
        { Lib::create(Cfg::create_default()) } -> std::same_as<Lib>;
    };


/**
 * @brief Defines interface for an input library.
 */
template<typename Lib, typename Cfg, typename JsonNode>
concept IsInputLib =
    IsConfig<Cfg, JsonNode> &&
    IsLib<Lib, Cfg> &&
    requires(Lib lib, std::istream &ist, const std::filesystem::path &path) {
        { lib.load_file(ist) } -> std::same_as<astfri::TranslationUnit>;
        { lib.load_file(path) } -> std::same_as<astfri::TranslationUnit>;
        { lib.load_project(path) } -> std::same_as<std::vector<astfri::TranslationUnit>>;
    };


/**
 * @brief Defines interface for an output library.
 */
template<typename Lib, typename Cfg, typename JsonNode>
concept IsOutputLib =
    IsConfig<Cfg, JsonNode> &&
    IsLib<Lib, Cfg> &&
    requires(std::ostream &ost, const astfri::TranslationUnit &root) {
        { Lib::process_ast(Cfg::create_default(), root) } -> std::same_as<void>;
        { Lib::process_ast(Cfg::create_default(), root, ost) } -> std::same_as<void>;
    };


} // namespace astfri::detail

#endif
