#ifndef CSHARP_COMMON_INL
#define CSHARP_COMMON_INL

namespace astfri::csharp::util
{

template<typename T>
void hash_combine(size_t& seed, const T& v)
{
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

} // namespace astfri::csharp::util

#endif // CSHARP_COMMON_INL