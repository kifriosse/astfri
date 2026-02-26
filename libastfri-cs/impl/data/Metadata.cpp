#include <libastfri-cs/impl/data/Metadata.hpp>

namespace astfri::csharp
{

TypeMetadata::TypeMetadata(const TypeBinding& tb) :
    tb_(tb)
{
}

void TypeMetadata::add_def(const TSNode& node, SourceFile* src)
{
    add_def(TypeDefLoc{.nType = node, .src = src});
}

void TypeMetadata::add_def(TypeDefLoc defLocation)
{
    defs_.emplace_back(defLocation);
}

void TypeMetadata::add_method(MethodId id, MethodMetadata methodMetadata)
{
    auto [it, _] = methods_.try_emplace(std::move(id));
    it->second.emplace_back(std::move(methodMetadata));
}

bool TypeMetadata::add_property(std::string name, PropertyNode prop)
{
    auto [it, inserted] = properties_.try_emplace(std::move(name));
    if (inserted)
    {
        it->second = std::move(prop);
    }
    return inserted;
}

bool TypeMetadata::add_memb_var(
    std::string name,
    const MemberVarMetadata& membMetadata
)
{
    auto [it, inserted] = memberVars_.try_emplace(std::move(name));
    if (inserted)
    {
        it->second = membMetadata;
    }
    return inserted;
}

std::span<const TypeDefLoc> TypeMetadata::defs()
{
    return defs_;
}

MethodMetadata* TypeMetadata::find_method(const MethodId& id)
{
    const auto it = methods_.find(id);
    if (it != methods_.end() && it->second.size() == 1)
    {
        return &it->second.front();
    }
    return nullptr;
}

MemberVarMetadata* TypeMetadata::find_memb_var(const std::string_view name)
{
    const auto it = memberVars_.find(name);
    if (it != memberVars_.end())
    {
        return &it->second;
    }
    return nullptr;
}

PropertyNode* TypeMetadata::find_property(const std::string_view name)
{
    const auto it = properties_.find(name);
    if (it != properties_.end())
    {
        return &it->second;
    }
    return nullptr;
}

TypeBinding& TypeMetadata::type_binding()
{
    return tb_;
}

} // namespace astfri::csharp