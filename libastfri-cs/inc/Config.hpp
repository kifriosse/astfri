
#ifndef CSHARP_CONFIG_HPP
#define CSHARP_CONFIG_HPP
#include <astfri/impl/Concepts.hpp>

#include <libastfri-cs/inc/SDKProfile.hpp>

#include <external/rapidjson/include/rapidjson/document.h>
#include <vector>

namespace astfri::csharp {

struct Config {

    SDKProfile profile = SDKProfile::Core;
    std::vector<std::filesystem::path> external_type_sources;

    Config() = default;
    explicit Config(const rapidjson::Value& path);
    explicit Config(const std::filesystem::path& path);
    Config(int argc, char* argv[]);

    static Config createFromJson(const rapidjson::Value& json);
    static Config createFromJson(const std::filesystem::path& path);
    static Config createDefault();
    static Config createFromArgs(int argc, char* argv[]);
};

} // namespace astfri::csharp

static_assert(astfri::IsConfigClass<astfri::csharp::Config, rapidjson::Value>);

#endif // CSHARP_CONFIG_HPP
