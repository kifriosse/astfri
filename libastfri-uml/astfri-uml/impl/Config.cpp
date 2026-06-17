#include <astfri-uml/Config.hpp>
#include <astfri-common/RapidjsonUtils.hpp>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <stdexcept>


namespace astfri::uml {


Config Config::create_from_json(const rapidjson::Value &node) {
    Config config;
    config.read_file(common::get_object(node, "file"));
    config.read_types(common::get_object(node, "types"));
    config.read_access(common::get_object(node, "accessMod"));
    config.read_colors(common::get_object(node, "colors"));
    config.read_relations(common::get_object(node, "relations"));
    config.read_destructor(common::get_object(node, "destructor"));
    config.read_namespaces(common::get_object(node, "namespaces"));
    return config;
}

Config Config::create_from_json(const std::filesystem::path& path) {
    return Config::create_from_json(common::read_document(path));
}

Config Config::create_default() {
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

Config Config::create_from_args(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    throw std::runtime_error("Not implemented yet.");
}

void Config::write_json(rapidjson::Value &out, rapidjson::Document::AllocatorType &alloc) const {
    rapidjson::Value &file = common::add_object(out, alloc, "file");
    common::add_bool(file, alloc, "writeToFile", this->writeToFile);
    common::add_string(file, alloc, "filePath", this->filePath);

    rapidjson::Value &types = common::add_object(out, alloc, "types");
    common::add_string(types, alloc, "intTypeName", this->intTypeName);
    common::add_string(types, alloc, "floatTypeName", this->floatTypeName);
    common::add_string(types, alloc, "charTypeName", this->charTypeName);
    common::add_string(types, alloc, "boolTypeName", this->boolTypeName);
    common::add_string(types, alloc, "voidTypeName", this->voidTypeName);
    common::add_string(types, alloc, "indirectIndicator", this->indirectIndicator);
    common::add_string(types, alloc, "separator", this->separator);
    common::add_string(types, alloc, "typeConvention", type_conventions_to_string(this->typeConvention));

    rapidjson::Value &accessMod = common::add_object(out, alloc, "accessMod");
    common::add_bool(accessMod, alloc, "innerView", this->innerView);
    common::add_bool(accessMod, alloc, "drawIcons", this->drawIcons);
    common::add_string(accessMod, alloc, "publicPrefix", this->publicPrefix);
    common::add_string(accessMod, alloc, "privatePrefix", this->privatePrefix);
    common::add_string(accessMod, alloc, "protectedPrefix", this->protectedPrefix);
    common::add_string(accessMod, alloc, "packagePrivatePrefix", this->packagePrivatePrefix);

    rapidjson::Value &colors = common::add_object(out, alloc, "colors");
    common::add_string(colors, alloc, "bgDiagram", this->bgDiagram);
    common::add_string(colors, alloc, "bgElement", this->bgElement);
    common::add_string(colors, alloc, "elementBorder", this->elementBorder);
    common::add_string(colors, alloc, "fontColor", this->fontColor);
    common::add_string(colors, alloc, "arrowColor", this->arrowColor);

    rapidjson::Value &relations = common::add_object(out, alloc, "relations");
    common::add_string(relations, alloc, "association", this->association);
    common::add_string(relations, alloc, "composition", this->composition);
    common::add_string(relations, alloc, "extension", this->extension);
    common::add_string(relations, alloc, "implementation", this->implementation);

    rapidjson::Value &destructor = common::add_object(out, alloc, "destructor");
    common::add_string(destructor, alloc, "destructorIndicator", this->destructorIndicator);

    rapidjson::Value &namespaces = common::add_object(out, alloc, "namespaces");
    common::add_bool(namespaces, alloc, "handleNamespaces", this->handleNamespaces);
    common::add_string(namespaces, alloc, "namespaceSeparator", this->namespaceSeparator);
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

void Config::read_types(const rapidjson::Value& val) {
    this->intTypeName = common::get_string(val, "intTypeName");
    this->floatTypeName = common::get_string(val, "floatTypeName");
    this->charTypeName = common::get_string(val, "charTypeName");
    this->boolTypeName = common::get_string(val, "boolTypeName");
    this->voidTypeName = common::get_string(val, "voidTypeName");
    this->indirectIndicator = common::get_char(val, "indirectIndicator");
    this->separator = common::get_string(val, "separator");
    this->typeConvention = type_conventions_from_string(common::get_string(val, "typeConvention"));
}

void Config::read_access(const rapidjson::Value& val) {
    this->innerView = common::get_bool(val, "innerView");
    this->drawIcons = common::get_bool(val, "drawIcons");
    this->publicPrefix = common::get_char(val, "publicPrefix");
    this->privatePrefix = common::get_char(val, "privatePrefix");
    this->protectedPrefix = common::get_char(val, "protectedPrefix");
    this->packagePrivatePrefix = common::get_char(val, "packagePrivatePrefix");
}

void Config::read_colors(const rapidjson::Value& val) {
    this->bgDiagram = common::get_string(val, "bgDiagram");
    this->bgElement = common::get_string(val, "bgElement");
    this->elementBorder = common::get_string(val, "elementBorder");
    this->fontColor = common::get_string(val, "fontColor");
    this->arrowColor = common::get_string(val, "arrowColor");
}

void Config::read_relations(const rapidjson::Value& val) {
    this->association = common::get_string(val, "association");
    this->composition = common::get_string(val, "composition");
    this->extension = common::get_string(val, "extension");
    this->implementation = common::get_string(val, "implementation");
}

void Config::read_destructor(const rapidjson::Value& val) {
    this->destructorIndicator = common::get_char(val, "destructorIndicator");
}

void Config::read_namespaces(const rapidjson::Value& val) {
    this->handleNamespaces = common::get_bool(val, "handleNamespaces");
    this->namespaceSeparator = common::get_string(val, "namespaceSeparator");
}


} // namespace astfri::uml
