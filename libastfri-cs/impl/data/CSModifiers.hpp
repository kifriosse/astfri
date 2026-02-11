#ifndef CSHARP_CS_MODIFIERS_HPP
#define CSHARP_CS_MODIFIERS_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri/inc/Stmt.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string_view>

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
    MaskType modifier_mask{0};

public:
    static CSModifiers handle_memb_modifs(
        const TSNode& nMemb,
        std::string_view src
    );
    static CSModifiers handle_var_modifs(
        const TSNode& nVar,
        std::string_view src,
        TSNode* nVarDecl
    );
    static CSModifiers handle_param_modifs(
        const TSNode& nParam,
        std::string_view src
    );

    [[nodiscard]] bool has(CSModifier mod) const;
    void add_modifier(CSModifier mod);
    void remove_modifier(CSModifier mod);
    /**
     * returns access modifier based on modifiers stored in this object
     * @return access modifier
     */
    [[nodiscard]] std::optional<AccessModifier> get_access_mod() const;
    /**
     * returns virtuality based on modifiers stored in this object
     * @return virtuality type
     */
    [[nodiscard]] Virtuality get_virtuality() const;
    /**
     * @brief Get indection type based on modifiers stored in this object
     * @param type type to apply indection to
     * @return indection type based on modifiers - if modifiers reference type
     * isn't reference type, original type is returned
     */
    [[nodiscard]] Type* get_indirection_type(Type* type) const;
};

} // namespace astfri::csharp

#endif // CSHARP_CS_MODIFIERS_HPP
