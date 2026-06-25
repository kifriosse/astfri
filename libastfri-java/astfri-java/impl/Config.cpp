#include <astfri-java/Config.hpp>


namespace astfri::java {


Config Config::create_default() {
    return Config();
}

Config Config::create_from_args(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return Config::create_default();
}

Config Config::create_from_json(const rapidjson::Value &node) {
    (void)node;
    return Config::create_default();
}

Config Config::create_from_json(const std::filesystem::path &path) {
    (void)path;
    return Config::create_default();
}


} // namespace astfri::java
