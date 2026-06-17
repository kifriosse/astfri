#include <astfri-common/RapidjsonUtils.hpp>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <cstring>
#include <format>
#include <fstream>
#include <stdexcept>


namespace astfri::common {


std::string as_string(const rapidjson::Value &val) {
    if (! val.IsString()) {
        throw std::invalid_argument("Argument is not a string.");
    }
    return val.GetString();
}


const rapidjson::Value &get_attribute(const rapidjson::Value &node, std::string_view key) {
    if (! node.IsObject()) {
        throw std::invalid_argument("Invalid json node. Not an object.");
    }
    if (! node.HasMember(key.data())) {
        throw std::invalid_argument(std::format("Invalid json node. Missing `{}` attribute.", key));
    }
    return node[key.data()];
}


const rapidjson::Value &get_object(const rapidjson::Value &node, std::string_view key) {
    const rapidjson::Value &obj = get_attribute(node, key);
    if (! obj.IsObject()) {
        throw std::invalid_argument(std::format("Attribute `{}` is not an object.", key));
    }
    return obj;
}


char get_char(const rapidjson::Value &node, std::string_view key) {
    std::string str = get_string(node, key);
    if (str.empty()) {
        throw std::invalid_argument(std::format("Empty string in `{}`.", key));
    }
    return str[0];

}


bool get_bool(const rapidjson::Value &node, std::string_view key) {
    const rapidjson::Value &obj = get_attribute(node, key);
    if (! obj.IsBool()) {
        throw std::invalid_argument(std::format("Attribute `{}` is not bool.", key));
    }
    return obj.GetBool();
}


int get_int(const rapidjson::Value &node, std::string_view key) {
    const rapidjson::Value &obj = get_attribute(node, key);
    if (! obj.IsInt()) {
        throw std::invalid_argument(std::format("Attribute `{}` is not an int.", key));
    }
    return obj.GetInt();
}


std::string get_string(const rapidjson::Value &node, std::string_view key) {
    return as_string(get_attribute(node, key));
}


std::vector<const rapidjson::Value*> get_array(const rapidjson::Value &node, std::string_view key) {
    const rapidjson::Value &obj = get_attribute(node, key);
    if (! obj.IsArray()) {
        throw std::invalid_argument(std::format("Attribute `{}` is not an array.", key));
    }
    std::vector<const rapidjson::Value*> arr;
    arr.reserve(obj.Capacity());
    for (const rapidjson::Value &e : obj.GetArray()) {
        arr.push_back(&e);
    }
    return arr;
}


std::vector<std::filesystem::path> get_array_of_paths(
    const rapidjson::Value &node,
    std::string_view key
) {
    const rapidjson::Value &obj = get_attribute(node, key);
    if (! obj.IsArray()) {
        throw std::invalid_argument(std::format("Attribute `{}` is not an array.", key));
    }
    std::vector<std::filesystem::path> arr;
    arr.reserve(obj.Capacity());
    for (const rapidjson::Value &e : obj.GetArray()) {
        if (! e.IsString()) {
            throw std::invalid_argument(std::format(
                "Array `{}` does not contain strings.",
                key));
        }
        arr.push_back(std::filesystem::path(e.GetString()));
    }
    return arr;
}


rapidjson::Value &add_object(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    rapidjson::Value jsonVal(rapidjson::kObjectType);
    node.AddMember(jsonKey.Move(), jsonVal.Move(), alloc);
    return node[key.data()];
}


void add_string(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    std::string_view val
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    rapidjson::Value jsonValue(val.data(), val.length(), alloc);
    node.AddMember(jsonKey.Move(), jsonValue.Move(), alloc);
}


void add_string(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    char val
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    rapidjson::Value jsonValue(&val, 1, alloc);
    node.AddMember(jsonKey.Move(), jsonValue.Move(), alloc);
}


void add_bool(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    bool val
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    node.AddMember(jsonKey.Move(), val, alloc);
}


void add_int(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    int val
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    node.AddMember(jsonKey.Move(), val, alloc);
}


void add_array_of_strings(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    const std::vector<std::string> &val
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    rapidjson::Value jsonVal(rapidjson::kArrayType);
    jsonVal.Reserve(static_cast<rapidjson::SizeType>(val.size()), alloc);
    for (const std::string &v : val) {
        rapidjson::Value sv;
        sv.SetString(v.c_str(), v.length(), alloc);
        jsonVal.PushBack(sv.Move(), alloc);
    }
    node.AddMember(jsonKey.Move(), jsonVal.Move(), alloc);
}

void add_array_of_paths(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    const std::vector<std::filesystem::path> &val
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    rapidjson::Value jsonVal(rapidjson::kArrayType);
    jsonVal.Reserve(static_cast<rapidjson::SizeType>(val.size()), alloc);
    for (const std::filesystem::path &v : val) {
        rapidjson::Value sv;
        sv.SetString(v.c_str(), std::strlen(v.c_str()), alloc);
        jsonVal.PushBack(sv.Move(), alloc);
    }
    node.AddMember(jsonKey.Move(), jsonVal.Move(), alloc);
}


rapidjson::Document create_document() {
    return rapidjson::Document(rapidjson::kObjectType);
}


rapidjson::Document read_document(const std::filesystem::path &path) {
    std::ifstream ifst(path);
    if (! ifst.is_open()) {
        throw std::invalid_argument(std::format("Failed to open `{}`.", path.string()));
    }
    rapidjson::IStreamWrapper isw(ifst);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    if (doc.HasParseError()) {
        throw std::runtime_error(std::format(
            "Failed to parse `{}`. Error code {}.",
            path.string(),
            static_cast<int>(doc.GetParseError())));
    }
    ifst.close();
    return doc;
}


void write_document(const std::filesystem::path &path, const rapidjson::Document & doc) {
    std::ofstream ofst(path);
    if (! ofst.is_open()) {
        throw std::invalid_argument(std::format("Failed to open `{}`.", path.string()));
    }
    rapidjson::OStreamWrapper osw(ofst);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    doc.Accept(writer);
}


} // namespace astfri::common
