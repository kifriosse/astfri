#include <libastfri-cs/inc/Config.hpp>

namespace astfri::csharp {

Config::Config(const rapidjson::Value& path) {
    // todo
}

Config::Config(const std::filesystem::path& path) {
    // todo
}

Config::Config(int argc, char* argv[]) {
    // todo
}

Config Config::createFromJson(const rapidjson::Value& json) {
    return Config(json);
}

Config Config::createFromJson(const std::filesystem::path& path) {
    return Config(path);
}

Config Config::createDefault() {
    return Config();
}

Config Config::createFromArgs(const int argc, char* argv[]) {
    return Config(argc, argv);
}

} // namespace astfri::csharp
