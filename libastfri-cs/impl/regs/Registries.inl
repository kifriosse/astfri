#ifndef CSHARP_REGISTRIES_INL
#define CSHARP_REGISTRIES_INL

namespace astfri::csharp
{

namespace regs
{

template<class Self, class RetType>
RetType Handlers::visit_error(Self*, const TSNode& node)
{
    const auto [row, column] = ts_node_start_point(node);
    throw std::runtime_error(
        "Invalid C# syntax in source code at: row" + std::to_string(row)
        + "and column " + std::to_string(column)
    );
}

} // namespace regs

template<class Factory, class Self, class RetType>
RetType RegManager::default_visit(Self*, const TSNode&)
{
    return Factory::get_instance().mk_unknown();
}

template<class Type>
Type RegManager::get_or_default(
    const RegistryMap<Type>& map,
    std::string_view name,
    Type nDefVal
)
{
    const auto it = map.find(name);
    return it != map.end() ? it->second : nDefVal;
}

template<class RetType>
std::optional<RetType> RegManager::get_opt(
    const RegistryMap<RetType>& map,
    std::string_view name
)
{
    const auto it = map.find(name);
    return it != map.end() ? std::optional{it->second} : std::nullopt;
}

} // namespace astfri::csharp

#endif // CSHARP_REGISTRIES_INL
