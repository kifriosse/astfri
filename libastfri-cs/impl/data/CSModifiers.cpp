#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string_view>

namespace astfri::csharp
{
CSModifiers CSModifiers::handle_memb_modifs(
    const TSNode& nMemb,
    const std::string_view src
)
{
    CSModifiers modifs;
    auto process = [&modifs, &src](const TSQueryMatch& match)
    {
        for (CaptureId i = 0; i < match.capture_count; ++i)
        {
            const TSNode nModif = match.captures[i].node;
            modifs.add_modifier(RegManager::get_modifier(nModif, src));
        }
    };
    util::for_each_match(nMemb, regs::QueryType::ParamModif, process);
    return modifs;
}

CSModifiers CSModifiers::handle_var_modifs(
    const TSNode& nVar,
    const std::string_view src,
    TSNode* nVarDecl
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
            auto [nCurrent, index] = match.captures[i];
            if (index == decl_id && nVarDecl)
                *nVarDecl = nCurrent;
            else if (index == modif_id)
                modifs.add_modifier(RegManager::get_modifier(nCurrent, src));
        }
    };
    util::for_each_match(nVar, q_type, process);
    return modifs;
}

CSModifiers CSModifiers::handle_param_modifs(
    const TSNode& nParam,
    std::string_view src
)
{
    CSModifiers paramMod;
    auto process = [&paramMod, &src](const TSQueryMatch& match)
    {
        for (uint32_t i = 0; i < match.capture_count; ++i)
        {
            const TSNode n_modif = match.captures[i].node;
            paramMod.add_modifier(RegManager::get_modifier(n_modif, src));
        }
    };
    util::for_each_match(nParam, regs::QueryType::ParamModif, process);
    return paramMod;
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

Type* CSModifiers::get_indirection_type(Type* type) const
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
