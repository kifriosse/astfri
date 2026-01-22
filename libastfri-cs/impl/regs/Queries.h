#ifndef CSHARP_QUERIES_HPP
#define CSHARP_QUERIES_HPP

#include <string_view>

namespace astfri::csharp::regs::queries
{
inline static constexpr std::string_view qTopLevelStmts =
    R"(
    (namespace_declaration
        body: (declaration_list
            [
                (class_declaration)
                (interface_declaration)
                (struct_declaration)
                (enum_declaration)
                (delegate_declaration)
                (record_declaration)
            ] @top_level_stmt
        )
    )
    (compilation_unit
        [
            (class_declaration)
            (interface_declaration)
            (struct_declaration)
            (enum_declaration)
            (delegate_declaration)
            (record_declaration)
        ] @top_level_stmt
    ))";

inline static constexpr std::string_view qDeclor =
    R"(
        (variable_declaration (variable_declarator)+ @var_decl)
    )";

inline static constexpr std::string_view qVarDecl =
    R"([
        (field_declaration
            (modifier)* @modifier
            (variable_declaration) @decl)
        (local_declaration_statement
            (modifier)* @modifier
            (variable_declaration) @decl)
    ])";

inline static constexpr std::string_view qParamModif
    = "(parameter (modifier)* @modifier)";

inline static constexpr std::string_view qMethodModif =
    R"([
        (method_declaration (modifier)* @modifier)
        (constructor_declaration (modifier)* @modifier)
    ])";

inline static constexpr std::string_view qFileNamespace
    = "(file_scoped_namespace_declaration) @namespace";

inline static constexpr std::string_view qCommentError =
    R"(
        (comment) @comment
        (ERROR) @error
    )";

inline static constexpr std::string_view qUsingDir
    = "(using_directive) @directive";

} // namespace astfri::csharp::regs::queries

#endif // CSHARP_QUERIES_HPP
