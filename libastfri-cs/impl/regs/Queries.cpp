#include <libastfri-cs/impl/regs/QueryRegistry.hpp>

#include <string>

namespace astfri::csharp::regs
{

const std::string QueryReg::top_level_stmts_q =
    R"(
    (namespace_declaration
        body: (declaration_list
        [
            (class_declaration)     @class
            (interface_declaration) @interface
            (struct_declaration)    @struct
            (enum_declaration)      @enum
            (delegate_declaration)  @delegate
            (record_declaration)    @record
        ])
    )
    (compilation_unit
    [
        (class_declaration)     @class
        (interface_declaration) @interface
        (struct_declaration)    @struct
        (enum_declaration)      @enum
        (delegate_declaration)  @delegate
        (record_declaration)    @record
    ])
    )";

const std::string QueryReg::decltor_q =
    R"(
        (variable_declaration (variable_declarator) @var_decl)
    )";

const std::string QueryReg::var_modif_q =
    R"([
        (field_declaration           (modifier) @modifier)
        (local_declaration_statement (modifier) @modifier)
        (parameter                   (modifier) @modifier)
    ])";

const std::string QueryReg::method_modif_q =
    R"(
        [
            (method_declaration (modifier) @modifier)
            (constructor_declaration (modifier) @modifier)
        ]
    )";

const std::string QueryReg::file_namespace_q
    = "(file_scoped_namespace_declaration) @namespace";

const std::string QueryReg::comment_error_q =
    R"(
        (comment) @comment
        (ERROR) @error
    )";

const std::string QueryReg::using_dir_q =
    R"(
        (using_directive) @directive
    )";

} // namespace astfri::csharp
