#ifndef CSHARP_CS_MODIFIERS_HPP
#define CSHARP_CS_MODIFIERS_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri/inc/Stmt.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string_view>
#include <vector>

namespace astfri::csharp
{

#define BIT(x) (static_cast<MaskType>(1) << (x))

enum class CSModifier : MaskType
{
    None      = 0,
    Public    = BIT(0),
    Private   = BIT(1),
    Protected = BIT(2),
    Internal  = BIT(3),
    File      = BIT(4),
    Static    = BIT(5),
    Readonly  = BIT(6),
    Const     = BIT(7),
    Volatile  = BIT(8),
    Abstract  = BIT(9),
    Virtual   = BIT(10),
    Override  = BIT(11),
    Sealed    = BIT(12),
    Partial   = BIT(13),
    Async     = BIT(15),
    In        = BIT(16),
    Out       = BIT(17),
    Ref       = BIT(18),
};

#undef BIT

class CSModifiers
{
private:
    MaskType modifier_mask = 0;

public:
    static CSModifiers handle_modifiers(
        const std::vector<TSNode>& mod_nodes,
        std::string_view source_code
    );

    [[nodiscard]] bool has(CSModifier mod) const;
    void add_modifier(CSModifier mod);
    void remove_modifier(CSModifier mod);
    [[nodiscard]] std::optional<AccessModifier> get_access_mod() const;
    [[nodiscard]] Virtuality get_virtuality() const;
};

} // namespace astfri::csharp

#endif // CSHARP_CS_MODIFIERS_HPP
