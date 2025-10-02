#ifndef JSONFORMATCHECKER_HPP
#define JSONFORMATCHECKER_HPP

#include <rapidjson/document.h>

/*
    inline std::unordered_map<std::string,Types>
strToTypeMapping={{"Int",IntType},{"Float",FloatType},
    {"Char",CharType},{"Bool",BoolType},{"Void",VoidType},{"User",UserType},
    {"Indirection",IndirectionType},{"Unknown",UnknownType},{"Dynamic",DynamicType}};

}
*/

class JsonFormatChecker
{
public:
    static JsonFormatChecker& get_instance();

public:
    void check_int_lit_expr(rapidjson::Value const& value);
    void check_float_lit_expr(rapidjson::Value const& value);
    void check_bool_lit_expr(rapidjson::Value const& value);
    void check_char_lit_expr(rapidjson::Value const& value);
    void check_str_lit_expr(rapidjson::Value const& value);
    void check_null_lit_expr(rapidjson::Value const& value);
    void check_if_expr(rapidjson::Value const& value);
    void check_bin_op_expr(rapidjson::Value const& value);
    void check_un_op_expr(rapidjson::Value const& value);
    void check_assign_expr(rapidjson::Value const& value);
    void check_comp_assign_expr(rapidjson::Value const& value);
    void check_param_var_ref_expr(rapidjson::Value const& value);
    void check_loc_var_ref_expr(rapidjson::Value const& value);
    void check_memb_var_ref_expr(rapidjson::Value const& value);
    void check_glob_var_ref_expr(rapidjson::Value const& value);
    void check_func_call_expr(rapidjson::Value const& value);
    void check_method_call_expr(rapidjson::Value const& value);
    void check_lambda_expr(rapidjson::Value const& value);
    void check_this_expr(rapidjson::Value const& value);
    void check_unkn_expr(rapidjson::Value const& value);
    void check_loc_var_def_stmt(rapidjson::Value const& value);
    void check_param_var_def_stmt(rapidjson::Value const& value);
    void check_memb_var_def_stmt(rapidjson::Value const& value);
    void check_glob_var_def_stmt(rapidjson::Value const& value);
    void check_func_def_stmt(rapidjson::Value const& value);
    void check_method_def_stmt(rapidjson::Value const& value);
    void check_class_def_stmt(rapidjson::Value const& value);
    void check_compound_stmt(rapidjson::Value const& value);
    void check_return_stmt(rapidjson::Value const& value);
    void check_expr_stmt(rapidjson::Value const& value);
    void check_if_stmt(rapidjson::Value const& value);
    void check_swich_stmt(rapidjson::Value const& value);
    void check_case_stmt(rapidjson::Value const& value);
    void check_while_stmt(rapidjson::Value const& value);
    void check_do_while_stmt(rapidjson::Value const& value);
    void check_for_stmt(rapidjson::Value const& value);
    void check_throw_stmt(rapidjson::Value const& value);
    void check_unknown_stmt(rapidjson::Value const& value);
    void check_int_type(rapidjson::Value const& value);
    void check_float_type(rapidjson::Value const& value);
    void check_char_type(rapidjson::Value const& value);
    void check_bool_type(rapidjson::Value const& value);
    void check_void_type(rapidjson::Value const& value);
    void check_user_type(rapidjson::Value const& value);
    void check_indirection_type(rapidjson::Value const& value);
    void check_unknown_type(rapidjson::Value const& value);
    void check_dynamic_type(rapidjson::Value const& value);
};

#endif
