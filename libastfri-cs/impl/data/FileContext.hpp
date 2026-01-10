
#ifndef CSHARP_FILE_CONTEXT_HPP
#define CSHARP_FILE_CONTEXT_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <variant>
#include <vector>

namespace astfri::csharp
{

class FileContext
{
private:
    static StmtFactory& stmt_factory_;

    std::vector<Scope> namespaces_{};
    // todo redo this into namespace aware
    IdentifierMap<std::variant<Type*, std::string>> aliases_{};

public:
    void add_using_directive(
        const TSNode& node,
        const TSLanguage* lang,
        std::string_view src_code
    );

    Type* make_type(const TSNode& node, Scope current_scope);
};

} // namespace astfri::csharp

#endif // CSHARP_FILE_CONTEXT_HPP
