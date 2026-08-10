#ifndef ASTFRI_JAVA_CONFIG_HPP
#define ASTFRI_JAVA_CONFIG_HPP

#include <astfri/impl/Concepts.hpp>

#include <rapidjson/document.h>

#include <filesystem>


namespace astfri::java {


struct Config {


public:
    static Config create_default();
    static Config create_from_args(int argc, char* argv[]);
    static Config create_from_json(const rapidjson::Value &node);
    static Config create_from_json(const std::filesystem::path &path);

public:
    Config(const Config &other) = default;
    Config(Config &&other) noexcept = default;
    Config &operator=(const Config &other) = default;
    Config &operator=(Config &&other) noexcept = default;

public:
    void write_json(const std::filesystem::path &path) const;

private:
    Config() = default;
};

static_assert(astfri::detail::IsConfig<astfri::java::Config, rapidjson::Value>);


} // namespace astfri::java

#endif // ASTFRI_JAVA_CONFIG_HPP
