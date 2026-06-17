#include <astfri-cs/Config.hpp>
#include <astfri-common/RapidjsonUtils.hpp>

#include <stdexcept>


namespace astfri::csharp {


Config Config::create_from_json(const rapidjson::Value& json) {
    Config config;
    config.profile = sdk_profile_from_string(common::get_string(json, "profile"));
    config.externalTypeSources = common::get_array_of_paths(json, "externalTypeSources");
    return config;
}

Config Config::create_from_json(const std::filesystem::path& path) {
    return Config::create_from_json(common::read_document(path));
}

Config Config::create_default() {
    Config config;
    config.profile = SDKProfile::Core;
    config.externalTypeSources = {};
    return config;
}

Config Config::create_from_args(const int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    throw std::runtime_error("Not implemented yet.");
}

void Config::write_json(rapidjson::Value &out, rapidjson::Document::AllocatorType &alloc) const {
    common::add_string(out, alloc, "profile", sdk_profile_to_string(profile));
    common::add_array_of_paths(out, alloc, "externalTypeSources", externalTypeSources);
}

void Config::write_json(const std::filesystem::path &path) const {
    rapidjson::Document doc = common::create_document();
    this->write_json(doc, doc.GetAllocator());
    common::write_document(path, doc);
}


} // namespace astfri::csharp
