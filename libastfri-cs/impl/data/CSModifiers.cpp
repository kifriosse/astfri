#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string_view>

namespace astfri::csharp
{
CSModifiers CSModifiers::handle_modifs_memb(
    const TSNode& memb_node,
    const std::string_view src
)
{
    CSModifiers modifs;
    auto process = [&modifs, &src](const TSQueryMatch& match)
    {
        for (uint32_t i = 0; i < match.capture_count; ++i)
        {
            TSNode n_current = match.captures[i].node;
            modifs.add_modifier(RegManager::get_modifier(n_current, src));
        }
    };
    util::for_each_match(memb_node, regs::QueryType::MethodModif, process);
    return modifs;
}

CSModifiers CSModifiers::handle_modifs_var(
    const TSNode& var_node,
    const std::string_view src,
    TSNode* n_var_decl
)
{
    using namespace regs;

    static constexpr auto q_type = QueryType::VarDecl;

    CSModifiers modifs;
    auto process = [&](const TSQueryMatch& match)
    {
        static const Query* const query = QueryReg::get().get_query(q_type);
        static const uint32_t decl_id   = query->id("decl");
        static const uint32_t modif_id  = query->id("modifier");

        for (uint32_t i = 0; i < match.capture_count; ++i)
        {
            auto [n_current, index] = match.captures[i];
            if (index == decl_id && n_var_decl)
                *n_var_decl = n_current;
            else if (index == modif_id)
                modifs.add_modifier(RegManager::get_modifier(n_current, src));
        }
    };
    util::for_each_match(var_node, q_type, process);
    return modifs;
}

CSModifiers CSModifiers::handle_modifs_param(
    const TSNode& param_node,
    std::string_view src
)
{
    CSModifiers param_mod;
    auto process = [&param_mod, &src](const TSQueryMatch& match)
    {
        for (uint32_t i = 0; i < match.capture_count; ++i)
        {
            const TSNode n_modif = match.captures[i].node;
            param_mod.add_modifier(RegManager::get_modifier(n_modif, src));
        }
    };
    util::for_each_match(param_node, regs::QueryType::ParamModif, process);
    return param_mod;
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

Type* CSModifiers::get_indection_type(Type* type) const
{
    if (has(CSModifier::Ref) || has(CSModifier::Out))
        return TypeFactory::get_instance().mk_indirect(type);
    if (has(CSModifier::In)
        || (has(CSModifier::Readonly) && has(CSModifier::Ref)))
    {
        // todo add handling of readonly ref and in - should be a const
        // reference to const
        return TypeFactory::get_instance().mk_indirect(type);
    }
    return type;
}

} // namespace astfri::csharp
