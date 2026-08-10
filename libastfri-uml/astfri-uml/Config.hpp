#ifndef LIBASTFRIUML_CONFIG_HPP
#define LIBASTFRIUML_CONFIG_HPP

#include <astfri/impl/Concepts.hpp>
#include <astfri-uml/impl/TypeConvention.hpp>

#include <rapidjson/document.h>


namespace astfri::uml {


struct Config {
    bool writeToFile;
    std::string filePath;

    std::string intTypeName;
    std::string floatTypeName;
    std::string charTypeName;
    std::string boolTypeName;
    std::string voidTypeName;
    char indirectIndicator;
    std::string separator;
    TypeConventions typeConvention;

    bool innerView;
    bool drawIcons;
    char publicPrefix;
    char privatePrefix;
    char protectedPrefix;
    char packagePrivatePrefix;

    std::string bgDiagram;
    std::string bgElement;
    std::string elementBorder;
    std::string fontColor;
    std::string arrowColor;

    std::string association;
    std::string composition;
    std::string extension;
    std::string implementation;

    char destructorIndicator;

    bool handleNamespaces;
    std::string namespaceSeparator;

private:
    Config() = default;

public:
    Config(const Config &other) = default;

public:
    static Config create_from_json(const rapidjson::Value &node);
    static Config create_from_json(const std::filesystem::path &path);
    static Config create_default();
    static Config create_from_args(int argc, char* argv[]);

public:
    void write_json(rapidjson::Value &out, rapidjson::Document::AllocatorType &alloc) const;
    void write_json(const std::filesystem::path &path) const;

private:
    void read_file(const rapidjson::Value &val);
    void read_types(const rapidjson::Value &val);
    void read_access(const rapidjson::Value &val);
    void read_colors(const rapidjson::Value &val);
    void read_relations(const rapidjson::Value &val);
    void read_destructor(const rapidjson::Value &val);
    void read_namespaces(const rapidjson::Value &val);
};

static_assert(astfri::detail::IsConfig<Config, rapidjson::Value>);


} // namespace astfri::uml

#endif
