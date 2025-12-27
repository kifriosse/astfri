#include <libastfri-cs/impl/CSModifiers.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/utils.hpp>

#include <optional>

namespace astfri::csharp
{
CSModifiers CSModifiers::handle_modifiers(
    const std::vector<TSNode>& mod_nodes,
    const std::string& source_code
)
{
    CSModifiers modifiers;
    for (const TSNode& node : mod_nodes)
    {
        std::string mod_str = util::extract_node_text(node, source_code);
        const auto res      = RegManager::get_modifier(mod_str);
        if (res.has_value())
        {
            modifiers.add_modifier(*res);
        }
    }
    return modifiers;
}

bool CSModifiers::has_modifier(CSModifier mod) const
{
    return (static_cast<MaskType>(mod) & modifier_mask) != 0;
}

void CSModifiers::add_modifier(CSModifier mod)
{
    modifier_mask |= static_cast<MaskType>(mod);
}

void CSModifiers::remove_modifier(CSModifier mod)
{
    modifier_mask &= ~static_cast<MaskType>(mod);
}

std::optional<AccessModifier> CSModifiers::get_access_mod() const
{
    if (has_modifier(CSModifier::Internal)
        && has_modifier(CSModifier::Protected))
        return AccessModifier::Internal;
    if (has_modifier(CSModifier::Private)
        && has_modifier(CSModifier::Protected))
        return AccessModifier::Protected;
    if (has_modifier(CSModifier::Public))
        return AccessModifier::Public;
    if (has_modifier(CSModifier::Private))
        return AccessModifier::Private;
    if (has_modifier(CSModifier::Internal))
        return AccessModifier::Internal;
    if (has_modifier(CSModifier::Protected))
        return AccessModifier::Protected;
    if (has_modifier(CSModifier::File))
        return AccessModifier::Internal;

    return {};
}

Virtuality CSModifiers::get_virtuality() const
{
    if (has_modifier(CSModifier::Abstract))
        return Virtuality::PureVirtual;
    if (has_modifier(CSModifier::Virtual) || has_modifier(CSModifier::Override))
        return Virtuality::Virtual;
    return Virtuality::NotVirtual;
}

} // namespace astfri::csharp
