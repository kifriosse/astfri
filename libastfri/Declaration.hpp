#ifndef LIBASTFRI_DECLARATION_HPP
#define LIBASTFRI_DECLARATION_HPP

#include <libastfri/impl/Utils.hpp>
#include <libastfri/Visitor.hpp>

#include <string>
#include <vector>

namespace astfri
{
struct Type;
struct Expr;
struct CompoundStmt;





/**
 * @brief TODO
 */
struct UknownDeclaration : Decl, details::MkVisitable<UknownDeclaration>
{
    std::string message_;

    UknownDeclaration(std::string message);
};

} // namespace astfri

#endif