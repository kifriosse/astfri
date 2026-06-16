#include <astfri-uml/impl/Config.hpp>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <format>
#include <fstream>
#include <stdexcept>


namespace astfri::uml {


Config Config::createFromJson(const rapidjson::Value &node) {
    Config config;
    config.read_file(safe_get(node, "file"));
    config.read_types(safe_get(node, "types"));
    config.read_access(safe_get(node, "accessMod"));
    config.read_colors(safe_get(node, "colors"));
    config.read_relations(safe_get(node, "relations"));
    config.read_destructor(safe_get(node, "destructor"));
    config.read_namespaces(safe_get(node, "namespaces"));
    return config;
}

Config Config::createFromJson(const std::filesystem::path& path) {
    std::ifstream ifst(path);
    if (! ifst.is_open()) {
        throw std::invalid_argument(std::format("Failed to open `{}`.", path.string()));
    }
    rapidjson::IStreamWrapper isw(ifst);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    ifst.close();
    return Config::createFromJson(doc);
}

Config Config::createDefault() {
    Config config;

    config.writeToFile          = false;
    config.filePath             = "class_diagram.puml";

    config.intTypeName          = "int";
    config.floatTypeName        = "float";
    config.charTypeName         = "char";
    config.boolTypeName         = "bool";
    config.voidTypeName         = "void";
    config.indirectIndicator    = '*';
    config.separator            = " : ";
    config.typeConvention       = TypeConventions::TypeAfterIdentifier;

    config.innerView            = true;
    config.drawIcons            = true;
    config.publicPrefix         = '+';
    config.privatePrefix        = '-';
    config.protectedPrefix      = '#';
    config.packagePrivatePrefix = '~';

    config.bgDiagram            = "#FFFFFF";
    config.bgElement            = "#FFDDDD";
    config.elementBorder        = "#000000";
    config.fontColor            = "#000000";
    config.arrowColor           = "#000000";

    config.association          = "<--";
    config.composition          = "*--";
    config.extension            = "<|--";
    config.implementation       = "<|..";

    config.destructorIndicator  = '~';

    config.handleNamespaces     = true;
    config.namespaceSeparator   = "::";

    return config;
}

Config Config::createFromArgs(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    throw std::runtime_error("Not implemented yet.");
}

void Config::write_json(rapidjson::Value &out, rapidjson::Document::AllocatorType &alloc) const {
    rapidjson::Value file;
    file.SetObject();
    add_bool_member(file, alloc, "writeToFile", this->writeToFile);
    add_string_member(file, alloc, "filePath", this->filePath);
    out.AddMember("file", file, alloc);

    rapidjson::Value types;
    types.SetObject();
    add_string_member(types, alloc, "intTypeName", this->intTypeName);
    add_string_member(types, alloc, "floatTypeName", this->floatTypeName);
    add_string_member(types, alloc, "charTypeName", this->charTypeName);
    add_string_member(types, alloc, "boolTypeName", this->boolTypeName);
    add_string_member(types, alloc, "voidTypeName", this->voidTypeName);
    add_string_member(types, alloc, "indirectIndicator", this->indirectIndicator);
    add_string_member(types, alloc, "separator", this->separator);
    add_string_member(types, alloc, "typeConvention", to_string(this->typeConvention));
    out.AddMember("types", types, alloc);

    rapidjson::Value accessMod;
    accessMod.SetObject();
    add_bool_member(accessMod, alloc, "innerView", this->innerView);
    add_bool_member(accessMod, alloc, "drawIcons", this->drawIcons);
    add_string_member(accessMod, alloc, "publicPrefix", this->publicPrefix);
    add_string_member(accessMod, alloc, "privatePrefix", this->privatePrefix);
    add_string_member(accessMod, alloc, "protectedPrefix", this->protectedPrefix);
    add_string_member(accessMod, alloc, "packagePrivatePrefix", this->packagePrivatePrefix);
    out.AddMember("accessMod", accessMod, alloc);

    rapidjson::Value colors;
    colors.SetObject();
    add_string_member(colors, alloc, "bgDiagram", this->bgDiagram);
    add_string_member(colors, alloc, "bgElement", this->bgElement);
    add_string_member(colors, alloc, "elementBorder", this->elementBorder);
    add_string_member(colors, alloc, "fontColor", this->fontColor);
    add_string_member(colors, alloc, "arrowColor", this->arrowColor);
    out.AddMember("colors", colors, alloc);

    rapidjson::Value relations;
    relations.SetObject();
    add_string_member(relations, alloc, "association", this->association);
    add_string_member(relations, alloc, "composition", this->composition);
    add_string_member(relations, alloc, "extension", this->extension);
    add_string_member(relations, alloc, "implementation", this->implementation);
    out.AddMember("relations", relations, alloc);

    rapidjson::Value destructor;
    destructor.SetObject();
    add_string_member(destructor, alloc, "destructorIndicator", this->destructorIndicator);
    out.AddMember("destructor", destructor, alloc);

    rapidjson::Value namespaces;
    namespaces.SetObject();
    add_bool_member(namespaces, alloc, "handleNamespaces", this->handleNamespaces);
    add_string_member(namespaces, alloc, "namespaceSeparator", this->namespaceSeparator);
    out.AddMember("namespaces", namespaces, alloc);
}

void Config::write_json_file(const std::filesystem::path &path) const {
    std::ofstream ofst(path);
    if (! ofst.is_open()) {
        throw std::runtime_error(std::format("Failed to open `{}`.", path.string()));
    }
    rapidjson::Document doc;
    doc.SetObject();
    this->write_json(doc, doc.GetAllocator());
    rapidjson::OStreamWrapper osw(ofst);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    doc.Accept(writer);
}

void Config::read_file(const rapidjson::Value& val) {
    this->writeToFile = safe_get(val, "writeToFile").GetBool();
    this->filePath = safe_get(val, "filePath").GetString();
}

void Config::read_types(const rapidjson::Value& val) {
    this->intTypeName = safe_get(val, "intTypeName").GetString();
    this->floatTypeName = safe_get(val, "floatTypeName").GetString();
    this->charTypeName = safe_get(val, "charTypeName").GetString();
    this->boolTypeName = safe_get(val, "boolTypeName").GetString();
    this->voidTypeName = safe_get(val, "voidTypeName").GetString();
    this->indirectIndicator = safe_get_char(val, "indirectIndicator");
    this->separator = safe_get(val, "separator").GetString();
    this->typeConvention = from_string(safe_get(val, "typeConvention").GetString());
}

void Config::read_access(const rapidjson::Value& val) {
    this->innerView = safe_get(val, "innerView").GetBool();
    this->drawIcons = safe_get(val, "drawIcons").GetBool();
    this->publicPrefix = safe_get_char(val, "publicPrefix");
    this->privatePrefix = safe_get_char(val, "privatePrefix");
    this->protectedPrefix = safe_get_char(val, "protectedPrefix");
    this->packagePrivatePrefix = safe_get_char(val, "packagePrivatePrefix");
}

void Config::read_colors(const rapidjson::Value& val) {
    this->bgDiagram = safe_get(val, "bgDiagram").GetString();
    this->bgElement = safe_get(val, "bgElement").GetString();
    this->elementBorder = safe_get(val, "elementBorder").GetString();
    this->fontColor = safe_get(val, "fontColor").GetString();
    this->arrowColor = safe_get(val, "arrowColor").GetString();
}

void Config::read_relations(const rapidjson::Value& val) {
    this->association = safe_get(val, "association").GetString();
    this->composition = safe_get(val, "composition").GetString();
    this->extension = safe_get(val, "extension").GetString();
    this->implementation = safe_get(val, "implementation").GetString();
}

void Config::read_destructor(const rapidjson::Value& val) {
    this->destructorIndicator = safe_get_char(val, "destructorIndicator");
}

void Config::read_namespaces(const rapidjson::Value& val) {
    this->handleNamespaces = safe_get(val, "handleNamespaces").GetBool();
    this->namespaceSeparator = safe_get(val, "namespaceSeparator").GetString();
}


const rapidjson::Value &Config::safe_get(const rapidjson::Value &node, std::string_view key) {
    if (! node.HasMember(key.data())) {
        throw std::invalid_argument(std::format("Invalid json node. Missing `{}` member.", key));
    }
    return node[key.data()];
}

char Config::safe_get_char(const rapidjson::Value &node, std::string_view key) {
    std::string str = safe_get(node, key).GetString();
    if (str.empty()) {
        throw std::invalid_argument(std::format("Empty string in `{}`.", key));
    }
    return str[0];
}

void Config::add_string_member(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    std::string_view str
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    rapidjson::Value jsonValue(str.data(), str.length(), alloc);
    node.AddMember(jsonKey.Move(), jsonValue.Move(), alloc);
}

void Config::add_string_member(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    char c
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    rapidjson::Value jsonValue(&c, 1, alloc);
    node.AddMember(jsonKey.Move(), jsonValue.Move(), alloc);
}

void Config::add_bool_member(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    bool val
) {
    rapidjson::Value jsonKey(key.data(), key.length(), alloc);
    node.AddMember(jsonKey.Move(), val, alloc);
}



} // namespace astfri::uml
