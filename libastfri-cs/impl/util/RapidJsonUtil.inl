#ifndef CSHARP_RAPID_JSON_UTIL_INL
#define CSHARP_RAPID_JSON_UTIL_INL

namespace astfri::csharp::util
{

template<typename... Args>
bool has_all(const rapidjson::Value& val, Args... args)
{
    return (... && val.HasMember(args));
}

} // namespace astfri::csharp::util

#endif // CSHARP_RAPID_JSON_UTIL_INL
