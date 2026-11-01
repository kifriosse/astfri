#ifndef LIBASTFRI_OUT_VECTOR_CONFIG_HPP
#define LIBASTFRI_OUT_VECTOR_CONFIG_HPP

#include <filesystem>

#include <rapidjson/document.h>
#include <astfri/impl/Concepts.hpp>

namespace astfri::vector {

struct Config {

    int minVectorSize;
    int windowStep;

    bool writeToFile;
    std::string filePath;

private:
    Config() = default;
    
public: 
    Config(const Config &ther) = default;
    
public:
    static Config create_default();
    static Config create_from_json(const std::filesystem::path &path);
    static Config create_from_json(const rapidjson::Value &node);
    static Config create_from_args(int args, char *argv[]);

    void write_json(rapidjson::Value& out, rapidjson::Document::AllocatorType& alloc) const;
    void write_json(const std::filesystem::path& path) const;

private: 
    void read_file(const rapidjson::Value& val);
    void read_algorithm(const rapidjson::Value& val);    
};

static_assert(astfri::detail::IsConfig<Config, rapidjson::Value>);

}

#endif