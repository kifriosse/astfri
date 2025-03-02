#ifndef ASTFRI_SERIALISER_HPP
#define ASTFRI_SERIALISER_HPP

#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri-serialize/inc/JsonFormatChecker.hpp>
#include <lib/rapidjson/istreamwrapper.h>

#include <format>
#include <fstream>
#include <algorithm>
#include <libastfri-serialize/inc/Data.hpp>

class AstFriSerializer {
public:
    static AstFriSerializer& get_instance();
    astfri::IVisitable* serialize(std::string filePath);

private:

AstFriSerializer();
astfri::ExprFactory& expressionMaker_;
astfri::StmtFactory& statementMaker_;
astfri::TypeFactory& typeMaker_ ;
JsonFormatChecker& jsonChecker_;

astfri::Expr* resolve_expr(rapidjson::Value& value);

astfri::IntLiteralExpr* serialize_int_lit_expr(rapidjson::Value& value);
astfri::FloatLiteralExpr* serialize_float_lit_expr(rapidjson::Value& value);
astfri::CharLiteralExpr* serialize_char_lit_expr(rapidjson::Value& value);
astfri::StringLiteralExpr* serialize_string_lit_expr(rapidjson::Value& value);
astfri::BoolLiteralExpr* serialize_bool_lit_expr(rapidjson::Value& value);
astfri::NullLiteralExpr* serialize_null_lit_expr(rapidjson::Value& value);
astfri::IfExpr* serialize_if_expr(rapidjson::Value& value);
astfri::BinOpExpr* serialize_bin_op_expr(rapidjson::Value& value);
astfri::UnaryOpExpr* serialize_unary_op_expr(rapidjson::Value& value);
astfri::AssignExpr* serialize_assign_expr(rapidjson::Value& value);
astfri::CompoundAssignExpr* serialize_compound_assign_expr(rapidjson::Value& value);
astfri::ParamVarRefExpr* serialize_param_var_ref_expr(rapidjson::Value& value);
astfri::LocalVarRefExpr* serialize_local_var_ref_expr(rapidjson::Value& value);
astfri::MemberVarRefExpr* serialize_member_var_ref_expr(rapidjson::Value& value);
astfri::GlobalVarRefExpr* serialize_global_var_ref_expr(rapidjson::Value& value);
astfri::FunctionCallExpr* serialize_function_call_expr(rapidjson::Value& value);
astfri::LambdaExpr* serialize_lambda_expr(rapidjson::Value& value);
astfri::MethodCallExpr* serialize_method_call_expr(rapidjson::Value& value);
astfri::ThisExpr* serialize_this_expr();
astfri::ConstructorCallExpr* serialize_constructor_call_expr(rapidjson::Value& value);
astfri::NewExpr* serialize_new_expr(rapidjson::Value& value);
astfri::DeleteExpr* serialize_delete_expr(rapidjson::Value& value);
astfri::UnknownExpr* serialize_unknown_expr();
astfri::ClassRefExpr* serialize_class_ref_expr(rapidjson::Value& value);

astfri::Stmt* resolve_stmt(rapidjson::Value& value);

astfri::LocalVarDefStmt* serialize_local_var_def_stmt(rapidjson::Value& value);
astfri::ParamVarDefStmt* serialize_param_var_def_stmt(rapidjson::Value& value);
astfri::MemberVarDefStmt* serialize_member_var_def_stmt(rapidjson::Value& value);
astfri::GlobalVarDefStmt* serialize_global_var_def_stmt(rapidjson::Value& value);
astfri::FunctionDefStmt* serialize_function_def_stmt(rapidjson::Value& value);
astfri::MethodDefStmt* serialize_method_def_stmt(rapidjson::Value& value,astfri::ClassDefStmt* owner=nullptr);
astfri::GenericParam* serialize_generic_param(rapidjson::Value& value);
astfri::ClassDefStmt* serialize_class_def_stmt(rapidjson::Value& value);
astfri::CompoundStmt* serialize_compound_stmt(rapidjson::Value& value);
astfri::ReturnStmt* serialize_return_stmt(rapidjson::Value& value);
astfri::ExprStmt* serialize_expr_stmt(rapidjson::Value& value);
astfri::IfStmt* serialize_if_stmt(rapidjson::Value& value);
astfri::CaseStmt* serialize_case_stmt(rapidjson::Value& value);
astfri::SwitchStmt* serialize_switch_stmt(rapidjson::Value& value);
astfri::WhileStmt* serialize_while_stmt(rapidjson::Value& value);
astfri::DoWhileStmt* serialize_do_while_stmt(rapidjson::Value& value);
astfri::ForStmt* serialize_for_stmt(rapidjson::Value& value);
astfri::ThrowStmt* serialize_throw_stmt(rapidjson::Value& value);
astfri::UnknownStmt* serialize_unknown_stmt(rapidjson::Value& value);
astfri::ConstructorDefStmt* serialize_constructor_def_stmt(rapidjson::Value& value);
astfri::BaseInitializerStmt* serialize_base_initializer_stmt(rapidjson::Value& value);
astfri::DestructorDefStmt* serialize_destructor_def_stmt(rapidjson::Value& value);
astfri::DefStmt* serialize_def_stmt(rapidjson::Value& value);
astfri::TranslationUnit* serialize_translation_unit(rapidjson::Value& value);

astfri::Type* serialize_type(rapidjson::Value& value);






astfri::Type* resolve_type(rapidjson::Value& value);


};
#endif
