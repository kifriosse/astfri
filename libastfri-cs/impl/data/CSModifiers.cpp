#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri/inc/Stmt.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace astfri::csharp
{
CSModifiers CSModifiers::handle_modifiers(
    const std::vector<TSNode>& mod_nodes,
    const std::string_view source_code
)
{
    CSModifiers modifiers;
    for (const TSNode& node : mod_nodes)
    {
        std::string mod_str = util::extract_node_text(node, source_code);
        if (const auto res = RegManager::get_modifier(mod_str))
        {
            modifiers.add_modifier(*res);
        }
    }
    return modifiers;
}

bool CSModifiers::has(CSModifier mod) const
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
    if (has(CSModifier::Internal) && has(CSModifier::Protected))
        return AccessModifier::Internal;
    if (has(CSModifier::Private) && has(CSModifier::Protected))
        return AccessModifier::Protected;
    if (has(CSModifier::Public))
        return AccessModifier::Public;
    if (has(CSModifier::Private))
        return AccessModifier::Private;
    if (has(CSModifier::Internal))
        return AccessModifier::Internal;
    if (has(CSModifier::Protected))
        return AccessModifier::Protected;
    if (has(CSModifier::File))
        return AccessModifier::Internal;

    return {};
}

Virtuality CSModifiers::get_virtuality() const
{
    if (has(CSModifier::Abstract))
        return Virtuality::PureVirtual;
    if (has(CSModifier::Virtual) || has(CSModifier::Override))
        return Virtuality::Virtual;
    return Virtuality::NotVirtual;
}

} // namespace astfri::csharp
