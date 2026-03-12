#ifndef CSHARP_SYMBOL_TABLE_INL
#define CSHARP_SYMBOL_TABLE_INL

namespace astfri::csharp
{

inline auto SymbolTable::get_type_metadata()
{
    auto transform = [this](UserTypeDefStmt* const def)
    { return this->get_type_metadata(def); };

    auto filter = [](const TypeMetadata* metadata) -> bool
    { return metadata != nullptr; };

    return userTypes_ | std::views::transform(transform)
         | std::views::filter(filter);
}

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_INL