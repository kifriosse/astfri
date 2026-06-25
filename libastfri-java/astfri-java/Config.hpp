#ifndef ASTFRI_JAVA_CONFIG_HPP
#define ASTFRI_JAVA_CONFIG_HPP

#include <rapidjson/document.h>

#include <filesystem>


namespace astfri::java {


struct Config {


public:
    static Config create_default();
    static Config create_from_args(int argc, char* argv[]);
    static Config create_from_json(const rapidjson::Value &node);
    static Config create_from_json(const std::filesystem::path &path);

private:
    Config() = default;
};


} // namespace astfri::java


#endif
