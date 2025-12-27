#ifndef CSHARP_SEMANTIC_CONTEXT_INL
#define CSHARP_SEMANTIC_CONTEXT_INL

template<>
struct std::hash<astfri::csharp::FunctionIdentifier>
{
    size_t operator()(
        const astfri::csharp::FunctionIdentifier& f
    ) const noexcept
    {
        size_t seed = std::hash<std::string>{}(f.name);
        astfri::csharp::util::hash_combine(seed, f.param_count);
        return seed;
    }
};

template<>
struct std::hash<astfri::csharp::MethodIdentifier>
{
    size_t operator()(const astfri::csharp::MethodIdentifier& m) const noexcept
    {
        size_t seed
            = std::hash<astfri::csharp::FunctionIdentifier>{}(m.func_id);
        astfri::csharp::util::hash_combine(seed, m.is_static);
        return seed;
    }
};

#endif // CSHARP_SEMANTIC_CONTEXT_INL