#ifndef CSHARP_REGISTRIES_INL
#define CSHARP_REGISTRIES_INL

namespace astfri::csharp {

namespace maps {

template<class Self, class RetType>
RetType Mappers::visit_error(Self*, const TSNode& node) {
    const auto [row, column] = ts_node_start_point(node);
    throw std::runtime_error(
        "Invalid C# syntax in source code at row" + std::to_string(row) + "and column "
        + std::to_string(column)
    );
}

} // namespace maps

template<class Factory, class Self, class RetType>
requires is_valid_factory<Factory, RetType>
RetType MapManager::default_visit(Self*, const TSNode&) {
    return Factory::get_instance().mk_unknown();
}

template<class Type>
Type MapManager::get_or_default(const RegistryMap<Type>& map, NodeType nodeType, Type nDefVal) {
    const auto it = map.find(nodeType);
    return it != map.end() ? it->second : nDefVal;
}

template<class RetType>
std::optional<RetType> MapManager::get_opt(
    const RegistryStrViewMap<RetType>& map,
    std::string_view name
) {
    const auto it = map.find(name);
    return it != map.end() ? std::optional{it->second} : std::nullopt;
}

} // namespace astfri::csharp

#endif // CSHARP_REGISTRIES_INL
