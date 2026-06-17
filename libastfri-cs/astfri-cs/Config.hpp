#ifndef CSHARP_CONFIG_HPP
#define CSHARP_CONFIG_HPP

#include <astfri/impl/Concepts.hpp>
#include <astfri-cs/SDKProfile.hpp>

#include <rapidjson/document.h>

#include <vector>


namespace astfri::csharp {


struct Config {
    SDKProfile profile;
    std::vector<std::filesystem::path> externalTypeSources;

private:
    Config() = default;

public:
    Config(const Config &other) = default;

public:
    static Config create_from_json(const rapidjson::Value& json);
    static Config create_from_json(const std::filesystem::path& path);
    static Config create_default();
    static Config create_from_args(int argc, char* argv[]);

public:
    void write_json(rapidjson::Value &out, rapidjson::Document::AllocatorType &alloc) const;
    void write_json(const std::filesystem::path &path) const;
};

static_assert(astfri::IsConfigClass<astfri::csharp::Config, rapidjson::Value>);


} // namespace astfri::csharp

#endif // CSHARP_CONFIG_HPP
