#include <astfri-vector/Config.hpp>
#include <astfri-common/RapidjsonUtils.hpp>

namespace astfri::vector {

Config Config::create_default() {
    Config cfg;
    cfg.minVectorSize = 5;
    cfg.windowStep = 1;
    cfg.writeToFile = false;
    cfg.filePath = "vectors.json";
    return cfg;
}

Config Config::create_from_json(const rapidjson::Value &node) {
    Config cfg;
    cfg.read_file(common::get_object(node, "file"));
    cfg.read_algorithm(common::get_object(node, "algorithm"));
    return cfg;
}

Config Config::create_from_json(const std::filesystem::path &path) {
    return Config::create_from_json(common::read_document(path));
}

Config Config::create_from_args(int args, char **argv) {
    (void)args;
    (void)argv;
    throw std::runtime_error("Not implemented yet.");
}

void Config::write_json(rapidjson::Value &out, rapidjson::Document::AllocatorType &alloc) const {
    rapidjson::Value &file = common::add_object(out, alloc, "file");
    common::add_bool(file, alloc, "writeToFile", this->writeToFile);
    common::add_string(file, alloc, "filePath", this->filePath);

    rapidjson::Value &algorithm = common::add_object(out, alloc, "algorithm");
    common::add_int(algorithm, alloc, "minVectorSize", this->minVectorSize);
    common::add_int(algorithm, alloc, "windowStep", this->windowStep);
}

void Config::write_json(const std::filesystem::path &path) const {
    rapidjson::Document doc = common::create_document();
    this->write_json(doc, doc.GetAllocator());
    common::write_document(path, doc);
}



void Config::read_file(const rapidjson::Value& val) {
    this->writeToFile = common::get_bool(val, "writeToFile");
    this->filePath = common::get_string(val, "filePath");
}

void Config::read_algorithm(const rapidjson::Value& val) {
    this->minVectorSize = common::get_int(val, "minVectorSize");
    this->windowStep = common::get_int(val, "windowStep");
}

}