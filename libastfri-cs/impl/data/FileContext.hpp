#ifndef CSHARP_FILE_CONTEXT_HPP
#define CSHARP_FILE_CONTEXT_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <variant>
#include <vector>

namespace astfri::csharp
{

struct FileContext
{
    // todo add global aliases
    std::vector<Scope> usings{};
    std::vector<UserTypeDefStmt*> static_usings;
    // todo redo this into namespace aware
    IdentifierMap<std::variant<Type*, std::string>> aliases{};
};

} // namespace astfri::csharp

#endif // CSHARP_FILE_CONTEXT_HPP
