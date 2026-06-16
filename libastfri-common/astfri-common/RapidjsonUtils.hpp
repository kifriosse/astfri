#ifndef ASTFRI_COMMON_JSON_UTILS_HPP
#define ASTFRI_COMMON_JSON_UTILS_HPP

#include <rapidjson/document.h>

#include <filesystem>
#include <vector>


namespace astfri::common {


/**
 * @brief Converts json value to string.
 * @throw std::invalid_argument if @p val is not string.
 * @param node Json value.
 * @return String value of the attribute.
 */
std::string as_string(const rapidjson::Value &val);

/**
 * @brief Reads attribute @p key of @p node.
 * @throw std::invalid_argument if @p node is not an object or misses @p key attribute.
 * @param node Node to read attribute from.
 * @param key Attribute name.
 * @return Reference to the attribute.
 */
const rapidjson::Value &get_attribute(const rapidjson::Value &node, std::string_view key);

/**
 * @brief Reads object attribute @p key of @p node.
 * @throw std::invalid_argument if @p node is not an object, misses @p key attribute or attribute is not an object.
 * @param node Node to read attribute from.
 * @param key Attribute name.
 * @return Reference to the object attribute.
 */
const rapidjson::Value &get_object(const rapidjson::Value &node, std::string_view key);

/**
 * @brief Reads string attribute @p key of @p node and returns the first character.
 * @throw std::invalid_argument if @p node is not an object, misses @p key attribute or the string is empty.
 * @param node Node to read attribute from.
 * @param key Attribute name.
 * @return First character of the string attribute.
 */
char get_char(const rapidjson::Value &node, std::string_view key);

/**
 * @brief Reads bool attribute @p key of @p node.
 * @throw std::invalid_argument if @p node is not an object or misses @p key attribute.
 * @param node Node to read attribute from.
 * @param key Attribute name.
 * @return Bool value of the attribute.
 */
bool get_bool(const rapidjson::Value &node, std::string_view key);

/**
 * @brief Reads int attribute @p key of @p node.
 * @throw std::invalid_argument if @p node is not an object or misses @p key attribute.
 * @param node Node to read attribute from.
 * @param key Attribute name.
 * @return Int value of the attribute.
 */
int get_int(const rapidjson::Value &node, std::string_view key);

/**
 * @brief Reads string attribute @p key of @p node.
 * @throw std::invalid_argument if @p node is not an object or misses @p key attribute.
 * @param node Node to read attribute from.
 * @param key Attribute name.
 * @return String value of the attribute.
 */
std::string get_string(const rapidjson::Value &node, std::string_view key);

/**
 * @brief Reads array attribute @p key of @p node.
 * @throw std::invalid_argument if @p node is not an object or misses @p key attribute.
 * @param node Node to read attribute from.
 * @param key Attribute name.
 * @return Array value of the attribute.
 */
std::vector<const rapidjson::Value*> get_array(const rapidjson::Value &node, std::string_view key);

/**
 * @brief Creates and adds object attribute @p key to @p node and returns reference to it.
 * @param node Node to add attribute to.
 * @param alloc Document allocator.
 * @param key Attribute name.
 * @return Reference to the new object attribute.
 */
rapidjson::Value &add_object(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key);

/**
 * @brief Adds string attribute @p key to @p node.
 * @param node Node to add attribute to.
 * @param alloc Document allocator.
 * @param key Attribute name.
 * @param val Attribute value.
 */
void add_string(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    std::string_view val);

/**
 * @brief Adds bool attribute @p key to @p node.
 * @param node Node to add attribute to.
 * @param alloc Document allocator.
 * @param key Attribute name.
 * @param val Attribute value.
 */
void add_bool(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    bool val);

/**
 * @brief Adds int attribute @p key to @p node.
 * @param node Node to add attribute to.
 * @param alloc Document allocator.
 * @param key Attribute name.
 * @param val Attribute value.
 */
void add_int(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    int val);

/**
 * @brief Adds array of strings attribute @p key to @p node.
 * @param node Node to add attribute to.
 * @param alloc Document allocator.
 * @param key Attribute name.
 * @param val Attribute value.
 */
void add_array(
    rapidjson::Value &node,
    rapidjson::Document::AllocatorType &alloc,
    std::string_view key,
    const std::vector<std::string> &val);

/**
 * @brief Creates json document object.
 * @return Document json object.
 */
rapidjson::Document create_document();

/**
 * @brief Reads json file as rapidjson document.
 * @throw std::invalid_argument if @p path does not exist or contains invalid json.
 * @param path Path to a json file.
 * @return Json document.
 */
rapidjson::Document read_document(const std::filesystem::path &path);

/**
 * @brief Writes json document @p doc to a file specified by @p path.
 * @throw std::invalid_argument if @p path is not writable.
 * @param path Path where to store the document.
 * @param doc Json document.
 */
void write_document(const std::filesystem::path &path, const rapidjson::Document &doc);


} // namespace astfri::common


#endif
