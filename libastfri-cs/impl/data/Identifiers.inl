#ifndef CSHARP_IDENTIFIERS_INL
#define CSHARP_IDENTIFIERS_INL

template<>
struct std::hash<astfri::csharp::FuncId>
{
    using FuncId = astfri::csharp::FuncId;

    size_t operator()(const FuncId& f) const noexcept
    {
        size_t seed = std::hash<std::string>{}(f.name);
        astfri::csharp::util::hash_combine(seed, f.param_count);
        return seed;
    }
};

template<>
struct std::hash<astfri::csharp::MethodId>
{
    using MethodId = astfri::csharp::MethodId;
    using FuncId   = astfri::csharp::FuncId;

    size_t operator()(const MethodId& m) const noexcept
    {
        size_t seed = std::hash<FuncId>{}(m.func_id);
        astfri::csharp::util::hash_combine(seed, m.is_static);
        return seed;
    }
};

#endif // CSHARP_IDENTIFIERS_INL