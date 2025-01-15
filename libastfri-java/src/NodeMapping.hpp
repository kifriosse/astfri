#include <map>
#include <functional>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>

inline const std::map<std::string, std::function<void*()>> type_map = {
    {"int", astfri::mk_int()},
    {"float", astfri::mk_float()},
    {"char", astfri::mk_bool()},
    {"boolean", astfri::mk_bool()},
    {"void", astfri::mk_void()},
};

inline const std::map<std::string, std::function<Stmt*(std::string, Type*, Expr*)>> stmt_map = {
    {"local_variable_declaration", astfri::mk_local_var_def},
    {"formal_parameter", astfri::mk_param_var_def},
    {"method_declaration", astfri::mk_method_def},
    {"class_declaration", astfri::mk_class_def},
    {"return_statement", astfri::mk_return},
    {"expression_statement", astfri::mk_expr},
    {"if_statement", astfri::mk_if},
    {"while_statement", astfri::mk_while},
    {"for_statement", astfri::mk_for},
};