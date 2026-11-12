#ifndef ASTFRI_TED_CONFIG_HPP
#define ASTFRI_TED_CONFIG_HPP

#include <astfri-ted/Costs.hpp>
#include <astfri/impl/Concepts.hpp>
#include <rapidjson/document.h>
#include <astfri-ted/impl/AttributeConfig.hpp>

#include <filesystem>

namespace astfri::ted {

struct Config {
    Costs costs;
    AttributeConfig attributes;

public:
    static Config create_default();
    static Config create_from_json(const rapidjson::Value& node);
    static Config create_from_json(const std::filesystem::path& path);
    static Config create_from_args(int argc, char* argv[]);

    Config(const Config& other) = default;
    void write_json(const std::filesystem::path& path) const;

private:
    Config() = default;

    static AttributeConfig read_attributes(const rapidjson::Value& node);
};

static_assert(astfri::detail::IsConfig<astfri::ted::Config, rapidjson::Value>);

} // namespace astfri::ted

#endif
