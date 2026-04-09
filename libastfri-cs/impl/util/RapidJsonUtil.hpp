#ifndef CSHARP_RAPID_JSON_UTIL_HPP
#define CSHARP_RAPID_JSON_UTIL_HPP

#include <rapidjson/document.h>

namespace astfri::csharp::util {
template<typename... Args>
bool has_all(const rapidjson::Value& val, Args... args);

} // namespace astfri::csharp::util

#include <libastfri-cs/impl/util/RapidJsonUtil.inl>

#endif // CSHARP_RAPID_JSON_UTIL_HPP
