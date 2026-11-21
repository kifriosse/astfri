#ifndef CSHARP_RAPID_JSON_UTIL_HPP
#define CSHARP_RAPID_JSON_UTIL_HPP

#include <rapidjson/document.h>

namespace astfri::csharp::util {

/**
 * @brief Checks if the given rapidjson::Value has all the specified members.
 * @tparam Args string literals (@code const char *@endcode) representing the JSON object member
 * names to check for.
 * @param val the rapidjson::Value to check. Should be a JSON object.
 * @param args the member names to check for.
 * @return true if the JSON object has all the specified members, false othrwise or
 * if JSON value is not an object.
 */
template<typename... Args>
bool has_all(const rapidjson::Value& val, Args... args);

} // namespace astfri::csharp::util

#include <astfri-cs/impl/util/RapidJsonUtil.inl>

#endif // CSHARP_RAPID_JSON_UTIL_HPP
