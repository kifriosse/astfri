#ifndef ASTFRI_SERIALISER_HPP
#define ASTFRI_SERIALISER_HPP

#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri-serialize/inc/JsonFormatChecker.hpp>
#include <rapidjson/istreamwrapper.h>

#include <functional>
#include <libastfri-serialize/inc/Data.hpp>

class AstFriDeSerializer {
public:
    static AstFriDeSerializer& get_instance();
    astfri::IVisitable* deserialize(std::string filePath);

private:

AstFriDeSerializer();
astfri::ExprFactory& expressionMaker_;
astfri::StmtFactory& statementMaker_;
astfri::TypeFactory& typeMaker_ ;
JsonFormatChecker& jsonChecker_;
std::unordered_map<std::string,astfri::ClassDefStmt*>
 nameWithClassDefStmtMapping_;
std::unordered_map<std::string,astfri::InterfaceDefStmt*> 
nameWithInterfaceDefStmtMapping_;

//key is name of unresolved class or interface def statement,value is vector of class or interface def statements,which innherit or implement
//that unresolved Class/Interface
std::unordered_map<std::string,std::vector<astfri::ClassDefStmt*>>
 unResolvedClassDefStmts_;
std::unordered_map<std::string,std::vector<astfri::InterfaceDefStmt*>> 
unresolvedInterfaceDefStmts_;

void resolve_class_def_stmts();
void resolve_interface_def_stmts();
bool is_class_def_stmt(astfri::Stmt* stmt);
void clear_records();

astfri::Expr* resolve_expr(rapidjson::Value& value);

astfri::IntLiteralExpr* deserialize_int_lit_expr(rapidjson::Value& value);
astfri::FloatLiteralExpr* deserialize_float_lit_expr(rapidjson::Value& value);
astfri::CharLiteralExpr* deserialize_char_lit_expr(rapidjson::Value& value);
astfri::StringLiteralExpr* deserialize_string_lit_expr(rapidjson::Value& value);
astfri::BoolLiteralExpr* deserialize_bool_lit_expr(rapidjson::Value& value);
astfri::NullLiteralExpr* deserialize_null_lit_expr();
astfri::IfExpr* deserialize_if_expr(rapidjson::Value& value);
astfri::BinOpExpr* deserialize_bin_op_expr(rapidjson::Value& value);
astfri::UnaryOpExpr* deserialize_unary_op_expr(rapidjson::Value& value);
astfri::ParamVarRefExpr* deserialize_param_var_ref_expr(rapidjson::Value& value);
astfri::LocalVarRefExpr* deserialize_local_var_ref_expr(rapidjson::Value& value);
astfri::MemberVarRefExpr* deserialize_member_var_ref_expr(rapidjson::Value& value);
astfri::GlobalVarRefExpr* deserialize_global_var_ref_expr(rapidjson::Value& value);
astfri::FunctionCallExpr* deserialize_function_call_expr(rapidjson::Value& value);
astfri::LambdaExpr* deserialize_lambda_expr(rapidjson::Value& value);
astfri::MethodCallExpr* deserialize_method_call_expr(rapidjson::Value& value);
astfri::ThisExpr* deserialize_this_expr();
astfri::ConstructorCallExpr* deserialize_constructor_call_expr(rapidjson::Value& value);
astfri::NewExpr* deserialize_new_expr(rapidjson::Value& value);
astfri::DeleteExpr* deserialize_delete_expr(rapidjson::Value& value);
astfri::UnknownExpr* deserialize_unknown_expr();
astfri::ClassRefExpr* deserialize_class_ref_expr(rapidjson::Value& value);
astfri::LambdaCallExpr* deserialize_lambda_call_expr(rapidjson::Value& value);

astfri::Stmt* resolve_stmt(rapidjson::Value& value);

astfri::LocalVarDefStmt* deserialize_local_var_def_stmt(rapidjson::Value& value);
astfri::ParamVarDefStmt* deserialize_param_var_def_stmt(rapidjson::Value& value);
astfri::MemberVarDefStmt* deserialize_member_var_def_stmt(rapidjson::Value& value);
astfri::GlobalVarDefStmt* deserialize_global_var_def_stmt(rapidjson::Value& value);
astfri::FunctionDefStmt* deserialize_function_def_stmt(rapidjson::Value& value);
astfri::MethodDefStmt* deserialize_method_def_stmt(rapidjson::Value& value,astfri::UserTypeDefStmt* owner=nullptr);
astfri::GenericParam* deserialize_generic_param(rapidjson::Value& value);
astfri::ClassDefStmt* deserialize_class_def_stmt(rapidjson::Value& value);
astfri::CompoundStmt* deserialize_compound_stmt(rapidjson::Value& value);
astfri::ReturnStmt* deserialize_return_stmt(rapidjson::Value& value);
astfri::ExprStmt* deserialize_expr_stmt(rapidjson::Value& value);
astfri::IfStmt* deserialize_if_stmt(rapidjson::Value& value);
astfri::CaseStmt* deserialize_case_stmt(rapidjson::Value& value);
astfri::SwitchStmt* deserialize_switch_stmt(rapidjson::Value& value);
astfri::WhileStmt* deserialize_while_stmt(rapidjson::Value& value);
astfri::DoWhileStmt* deserialize_do_while_stmt(rapidjson::Value& value);
astfri::ForStmt* deserialize_for_stmt(rapidjson::Value& value);
astfri::ThrowStmt* deserialize_throw_stmt(rapidjson::Value& value);
astfri::UnknownStmt* deserialize_unknown_stmt();
astfri::ConstructorDefStmt* deserialize_constructor_def_stmt(rapidjson::Value& value,astfri::ClassDefStmt* owner=nullptr);
astfri::BaseInitializerStmt* deserialize_base_initializer_stmt(rapidjson::Value& value);
astfri::DestructorDefStmt* deserialize_destructor_def_stmt(rapidjson::Value& value,astfri::ClassDefStmt* owner=nullptr);
astfri::DefStmt* deserialize_def_stmt(rapidjson::Value& value);
astfri::TranslationUnit* deserialize_translation_unit(rapidjson::Value& value);
astfri::BreakStmt* deserialize_break_stmt();
astfri::ContinueStmt* deserialize_continue_stmt(); 
astfri::DefaultCaseStmt* deserialize_default_case_stmt(rapidjson::Value& value);
astfri::InterfaceDefStmt* deserialize_interface_def_stmt(rapidjson::Value& value);

astfri::Type* deserialize_type(rapidjson::Value& value);
astfri::Type* resolve_type(rapidjson::Value& value);

template<typename T>
void fullfil_vector(std::vector<T>& vector,rapidjson::Value& value,std::function<T(rapidjson::Value&)> function){
    for(auto& element:value.GetArray()){
        vector.push_back(function(element));
    }
}
};
#endif
