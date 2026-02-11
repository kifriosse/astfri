#ifndef CSHARP_IDENTIFIERS_INL
#define CSHARP_IDENTIFIERS_INL

template<>
struct std::hash<astfri::csharp::MethodId>
{
    using MethodId = astfri::csharp::MethodId;

    size_t operator()(const MethodId& m) const noexcept
    {
        size_t seed = std::hash<std::string>{}(m.name);
        astfri::csharp::util::hash_combine(seed, m.paramCount);
        astfri::csharp::util::hash_combine(seed, m.isStatic);
        return seed;
    }
};

#endif // CSHARP_IDENTIFIERS_INL