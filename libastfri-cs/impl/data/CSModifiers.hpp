#ifndef CSHARP_CS_MODIFIERS_HPP
#define CSHARP_CS_MODIFIERS_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri/inc/Stmt.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string_view>

namespace astfri::csharp {

/**
 * @brief Macro used for creating bitmask for enums
 * @param x number of bits to bitshift to left
 */
#define BIT(x) (static_cast<MaskType>(1) << (x))

/**
 * @brief Enum representing C# modifiers
 * @note Enum doesn't include \code new\endcode and \code unsafe\endcode
 * modifiers
 */
enum class CSModifier : MaskType {
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

/**
 * @brief Class representing modifiers of C# members, variables and parameters.
 * It uses bitmask to store multiple modifiers.
 */
class CSModifiers {
private:
    MaskType modifier_mask{0};

public:
    /**
     * @brief Factory method for parsing/extrating modifiers of method
     * declarations (including constructors, operators etc.)
     * @param nMethod Tree-sitter node of method declaration
     * @param src source code of the file containing the member declaration
     * @return CSModifiers object with modifiers of the method declaration
     */
    static CSModifiers parser_method_modifs(const TSNode& nMethod, std::string_view src);
    /**
     * @brief Factory method for parsing/extrating modifiers of variable
     * declarations
     * @param nVar Tree-sitter node of variable declaration - field or local
     * variable
     * @param src source code of the file containing the variable declaration
     * @param nVarDecl output parameter - tree-sitter node of the first variable
     * declarator
     * @return CSModifiers object with modifiers of the variable declaration
     */
    static CSModifiers parse_var_modifs(const TSNode& nVar, std::string_view src, TSNode* nVarDecl);
    /**
     * @brief Factory method for parsing/extracing modifiers of parameter
     * declarations
     * @param nParam Tree-sitter node of parameter declaration
     * @param src source code of the file containing the parameter declaration
     * @return CSModifiers object with modifiers of the parameter declaration
     */
    static CSModifiers parse_param_modifs(const TSNode& nParam, std::string_view src);

    /**
     * @brief Checks if given modifier is present
     * @param mod modifier to check
     * @return true if modifier is present, false otherwise
     */
    [[nodiscard]] bool has(CSModifier mod) const;
    /**
     * @brief Adds given modifier
     * @param mod modifier to add
     */
    void add_modifier(CSModifier mod);
    /**
     * @brief Removes given modifier
     * @param mod modifier to remove
     */
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
