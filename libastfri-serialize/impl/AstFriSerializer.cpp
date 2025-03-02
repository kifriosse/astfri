#include <libastfri-serialize/inc/AstFriSerializer.hpp>

AstFriSerializer& AstFriSerializer::get_instance(){
    static AstFriSerializer instance;
    return instance;
}
AstFriSerializer::AstFriSerializer()
    :expressionMaker_(astfri::ExprFactory::get_instance()),
    statementMaker_(astfri::StmtFactory::get_instance()),
    typeMaker_(astfri::TypeFactory::get_instance()),
    jsonChecker_(JsonFormatChecker::get_instance()){}

astfri::IVisitable* AstFriSerializer::serialize(std::string filePath)
{

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    rapidjson::IStreamWrapper isw(file);
    rapidjson::Document document_;
    document_.ParseStream(isw);

    if (document_.HasParseError()) {
        throw std::runtime_error("JSON parse error: "+ document_.GetParseError());
        
    }
    if (!document_.HasMember("node")) {
        throw std::runtime_error("Missing node field\n Unable to detect node type!");
    }
    if (!document_["node"].IsString())
    {
        throw std::runtime_error("Invalid node format");
    }

    

    auto itExpr = astfri_serialize::strToExprMapping.find(document_["node"].GetString());
    
    if (itExpr == astfri_serialize::strToExprMapping.end()) {
        auto itStmt = astfri_serialize::strToStmtMapping.find(document_["node"].GetString());

        if (itStmt == astfri_serialize::strToStmtMapping.end()) throw std::runtime_error("Invalid node type");
        //return this->resolveStmt(this->document_);
    }

    return this->resolve_expr(document_);

    

}






astfri::IntLiteralExpr* AstFriSerializer::serialize_int_lit_expr(rapidjson::Value& value)
{
    

    return expressionMaker_.mk_int_literal(value["value"].GetInt());
}

astfri::FloatLiteralExpr* AstFriSerializer::serialize_float_lit_expr(rapidjson::Value& value)
{
    
    
    return expressionMaker_.mk_float_literal(value["value"].GetFloat());
}

astfri::CharLiteralExpr* AstFriSerializer::serialize_char_lit_expr(rapidjson::Value& value)
{
    
    
    return expressionMaker_.mk_char_literal(value["value"].GetString()[0]);
}

astfri::StringLiteralExpr* AstFriSerializer::serialize_string_lit_expr(rapidjson::Value& value)
{
    
    
    return expressionMaker_.mk_string_literal(value["value"].GetString());
}
astfri::BoolLiteralExpr* AstFriSerializer::serialize_bool_lit_expr(rapidjson::Value& value)
{
    
    
    return expressionMaker_.mk_bool_literal(value["value"].GetBool());
}
astfri::NullLiteralExpr* AstFriSerializer::serialize_null_lit_expr(rapidjson::Value& value)
{
   
    return expressionMaker_.mk_null_literal();
}
astfri::IfExpr* AstFriSerializer::serialize_if_expr(rapidjson::Value& value)
{

    astfri::Expr* cond = this->resolve_expr(value["condition"]);
    astfri::Expr* iftrue = this->resolve_expr(value["ifTrue"]);
    astfri::Expr* iffalse = this->resolve_expr(value["ifFalse"]);
    return expressionMaker_.mk_if(cond,iftrue,iffalse);
    
}

astfri::BinOpExpr* AstFriSerializer::serialize_bin_op_expr(rapidjson::Value& value){

    
    astfri::Expr* left = this->resolve_expr(value["left"]);
    astfri::Expr* right = this->resolve_expr(value["right"]);
    
    auto it = astfri_serialize::binOpTypeMapping.find(value["operator"].GetString());
    if (it == astfri_serialize::binOpTypeMapping.end()) {
        throw std::runtime_error("Invalid operator in BinOpExpr");
    }

    astfri::BinOpType op = it->second;
    return this->expressionMaker_.mk_bin_on(left,op,right);
}
astfri::UnaryOpExpr* AstFriSerializer::serialize_unary_op_expr(rapidjson::Value& value){

    astfri::Expr* arg = this->resolve_expr(value["argument"]);
    auto it = astfri_serialize::unaryOpTypeMapping.find(value["operator"].GetString());
    if (it == astfri_serialize::unaryOpTypeMapping.end()) {
        throw std::runtime_error("Invalid operator in UnaryOpExpr");
    }
    astfri::UnaryOpType op = it->second;
    
    //if it s pre increment/decrement ,check property isPostfix to determine which operator it really is,because in mapping
    //there or only pre variants
    if(op == astfri::UnaryOpType::PreIncrement || op == astfri::UnaryOpType::PreDecrement){
        std::string opString = value["operator"].GetString();    
       //if operator is postfix change op to proper operator type
       if (value["isPostfix"].GetBool()){
            op = opString == "++" ? astfri::UnaryOpType::PostIncrement : astfri::UnaryOpType::PostDecrement;

       }

    }

    return this->expressionMaker_.mk_unary_op(it->second,arg);
}
astfri::AssignExpr* AstFriSerializer::serialize_assign_expr(rapidjson::Value& value){

    return this->expressionMaker_.mk_assign(this->resolve_expr(value["left"]),this->resolve_expr(value["right"]));
}

astfri::CompoundAssignExpr* AstFriSerializer::serialize_compound_assign_expr(rapidjson::Value& value){
    astfri::Expr* left = this->resolve_expr(value["left"]);
    astfri::Expr* right= this->resolve_expr(value["right"]);

    auto it = astfri_serialize::binOpTypeMapping.find(value["operator"].GetString());
    if (it == astfri_serialize::binOpTypeMapping.end()) {
        throw std::runtime_error("Invalid operator in BinOpExpr");
    }

    return this->expressionMaker_.mk_compound_assign(left,it->second,right);
}


astfri::ParamVarRefExpr* AstFriSerializer::serialize_param_var_ref_expr(rapidjson::Value& value){

   return this->expressionMaker_.mk_param_var_ref(std::move(value["name"].GetString()));
}

astfri::LocalVarRefExpr* AstFriSerializer::serialize_local_var_ref_expr(rapidjson::Value& value){
    return this->expressionMaker_.mk_local_var_ref(std::move(value["name"].GetString()));
    
}
astfri::MemberVarRefExpr* AstFriSerializer::serialize_member_var_ref_expr(rapidjson::Value& value){
   return  this->expressionMaker_.mk_member_var_ref(std::move(value["name"].GetString()));
}

astfri::GlobalVarRefExpr* AstFriSerializer::serialize_global_var_ref_expr(rapidjson::Value& value){
    return this->expressionMaker_.mk_global_var_ref(std::move(value["name"].GetString()));
}
astfri::ClassRefExpr* AstFriSerializer::serialize_class_ref_expr(rapidjson::Value& value){
    return this->expressionMaker_.mk_class_ref(std::move(value["name"].GetString()));
}

astfri::FunctionCallExpr* AstFriSerializer::serialize_function_call_expr(rapidjson::Value& value){
    std::vector<astfri::Expr*> arguments;

    for(auto& arg : value["arguments"].GetArray()){
        arguments.push_back(this->resolve_expr(arg));

    }

    return this->expressionMaker_.mk_function_call(std::move(value["name"].GetString()),std::move(arguments));
}

astfri::MethodCallExpr* AstFriSerializer::serialize_method_call_expr(rapidjson::Value& value){
    
    std::vector<astfri::Expr*> arguments;
    for(auto& arg : value["arguments"].GetArray()){
        arguments.push_back(this->resolve_expr(arg));
    }

    return this->expressionMaker_.mk_method_call(value["owner"].IsNull() ? nullptr : this->resolve_expr(value["owner"])
                                                                ,std::move(value["name"].GetString()),std::move(arguments));
}
//TODO
astfri::LambdaExpr* AstFriSerializer::serialize_lambda_expr(rapidjson::Value& value){
    std::vector<astfri::ParamVarDefStmt*> params;
    /*
    for (auto& param : val["params"].GetArray()){
        params.push_back(this->serializeParamVarDefStmt(param));
    }
    astfri::Stmt* body = this->resolveStmt(val["body"]);

   return this->expressionMaker_.mk_lambda_expr(std::move(params),body);*/
   return this->expressionMaker_.mk_lambda_expr(params,nullptr);
}

astfri::ThisExpr* AstFriSerializer::serialize_this_expr(){
    return this->expressionMaker_.mk_this();
}
astfri::UnknownExpr* AstFriSerializer::serialize_unknown_expr(){
    return this->expressionMaker_.mk_unknown();
}

astfri::ConstructorCallExpr* AstFriSerializer::serialize_constructor_call_expr(rapidjson::Value& value){
    std::vector<astfri::Expr*> arguments;

    for(auto& arg : value["arguments"].GetArray()){
        arguments.push_back(this->resolve_expr(arg));
    }

    return this->expressionMaker_.mk_constructor_call(this->resolve_type(value["type"]),std::move(arguments));
}

astfri::NewExpr* AstFriSerializer::serialize_new_expr(rapidjson::Value& value){
    return this->expressionMaker_.mk_new(this->serialize_constructor_call_expr(value["init"]));
}

astfri::DeleteExpr* AstFriSerializer::serialize_delete_expr(rapidjson::Value& value){
    this->expressionMaker_.mk_delete(this->resolve_expr(value["expression"]));
}




astfri::Expr* AstFriSerializer::resolve_expr(rapidjson::Value& value)
{

    auto it = astfri_serialize::strToExprMapping.find(value["node"].GetString());
    if (it == astfri_serialize::strToExprMapping.end()) {
        throw std::runtime_error("Invalid node type in expression");
    }
    switch(it->second){
        case astfri_serialize::IntLiteralExpr:
            return this->serialize_int_lit_expr(value);
        case astfri_serialize::FloatLiteralExpr:
            return this->serialize_float_lit_expr(value);
        case astfri_serialize::CharLiteralExpr:
            return this->serialize_char_lit_expr(value);
        case astfri_serialize::StringLiteralExpr:
            return this->serialize_string_lit_expr(value);
        case astfri_serialize::BoolLiteralExpr:
            return this->serialize_bool_lit_expr(value);
        case astfri_serialize::NullLiteralExpr:
            return this->serialize_null_lit_expr(value);
        case astfri_serialize::IfExpr:
            return this->serialize_if_expr(value);
        case astfri_serialize::BinOpExpr:
            return this->serialize_bin_op_expr(value);
        case astfri_serialize::UnaryOpExpr:
            return this->serialize_unary_op_expr(value);
        case astfri_serialize::AssignExpr:
            return this->serialize_assign_expr(value);
        case astfri_serialize::CompoundAssignExpr:
            return this->serialize_compound_assign_expr(value);
        case astfri_serialize::ParamVarRefExpr:
            return this->serialize_param_var_ref_expr(value);
        case astfri_serialize::LocalVarRefExpr:
            return this->serialize_local_var_ref_expr(value);
        case astfri_serialize::MemberVarRefExpr:
            return this->serialize_member_var_ref_expr(value);
        case astfri_serialize::GlobalVarRefExpr:
            return this->serialize_global_var_ref_expr(value);
        case astfri_serialize::FunctionCallExpr:
            return this->serialize_function_call_expr(value);
        case astfri_serialize::MethodCallExpr:
            return this->serialize_method_call_expr(value);
        case astfri_serialize::LambdaExpr:
            return this->serialize_lambda_expr(value);
        case astfri_serialize::ThisExpr:
            return this->serialize_this_expr();
        case astfri_serialize::UnknownExpr:
            return this->serialize_unknown_expr();
        case astfri_serialize::NewExpr:
            return this->serialize_new_expr(value);
        case astfri_serialize::DeleteExpr:
            return this->serialize_delete_expr(value);
        case astfri_serialize::ConstructorCallExpr:
            return this->serialize_constructor_call_expr(value);

    }

}
astfri::Type* AstFriSerializer::resolve_type(rapidjson::Value& value)
{
    auto it = astfri_serialize::strToTypeMapping.find(value["node"].GetString());
    if (it == astfri_serialize::strToTypeMapping.end()) {
        throw std::runtime_error("Invalid type");
    }
    switch(it->second){
        case astfri_serialize::IntType:
            return this->typeMaker_.mk_int();
        case astfri_serialize::FloatType:
            return this->typeMaker_.mk_float();
        case astfri_serialize::CharType:
            return this->typeMaker_.mk_char();
        case astfri_serialize::BoolType:
            return this->typeMaker_.mk_bool();
        case astfri_serialize::VoidType:
            return this->typeMaker_.mk_void();
        case astfri_serialize::UserType:
             return this->typeMaker_.mk_user(value["name"].GetString());
        case astfri_serialize::IndirectionType:
            return this->typeMaker_.mk_indirect(this->resolve_type(value["indirect"]));
        case astfri_serialize::UnknownType:
            return this->typeMaker_.mk_unknown();
        case astfri_serialize::DynamicType:
            //return this->typeMaker_.mk_dynamic();
            return nullptr;
    }
    
}

astfri::Stmt* AstFriSerializer::resolve_stmt(rapidjson::Value& value){
    auto it = astfri_serialize::strToStmtMapping.find(value["node"].GetString());
    
    if (it == astfri_serialize::strToStmtMapping.end()) {
        throw std::runtime_error("Invalid node type in expression");
    }

    switch(it->second){
        case astfri_serialize::LocalVarDefStmt:
            return this->serialize_local_var_def_stmt(value);
        case astfri_serialize::ParamVarDefStmt:
            return this->serialize_param_var_def_stmt(value);
        case astfri_serialize::MemberVarDefStmt:
            return this->serialize_member_var_def_stmt(value);
        case astfri_serialize::GlobalVarDefStmt:
            return this->serialize_global_var_def_stmt(value);
        case astfri_serialize::FunctionDefStmt:
            return this->serialize_function_def_stmt(value);
        case astfri_serialize::MethodDefStmt:
            return this->serialize_method_def_stmt(value);
        case astfri_serialize::GenericParam:
            return this->serialize_generic_param(value);
        case astfri_serialize::ClassDefStmt:
            return this->serialize_class_def_stmt(value);
        case astfri_serialize::CompoundStmt:
            return this->serialize_compound_stmt(value);
        case astfri_serialize::ReturnStmt:
            return this->serialize_return_stmt(value);
        case astfri_serialize::ExprStmt:
            return this->serialize_expr_stmt(value);
        case astfri_serialize::IfStmt:
            return this->serialize_if_stmt(value);
        case astfri_serialize::SwitchStmt:
            return this->serialize_switch_stmt(value);
        case astfri_serialize::CaseStmt:
            return this->serialize_case_stmt(value);
        case astfri_serialize::WhileStmt:
            return this->serialize_while_stmt(value);
        case astfri_serialize::DoWhileStmt:
            return this->serialize_do_while_stmt(value);
        case astfri_serialize::ForStmt:
            return this->serialize_for_stmt(value);
        case astfri_serialize::ThrowStmt:
            return this->serialize_throw_stmt(value);
        case astfri_serialize::UnknownStmt:
            return this->serialize_unknown_stmt(value);
        case astfri_serialize::TranslationUnit:
            return this->serialize_translation_unit(value);
        case astfri_serialize::DefStmt:
            return this->serialize_def_stmt(value);
        case astfri_serialize::ConstructorDefStmt:
            return this->serialize_constructor_def_stmt(value);
        case astfri_serialize::DestructorDefStmt:
            return this->serialize_destructor_def_stmt(value);
        case astfri_serialize::BaseInitializerStmt:
            return this->serialize_base_initializer_stmt(value);

    }


}

astfri::LocalVarDefStmt* AstFriSerializer::serialize_local_var_def_stmt(rapidjson::Value& value){
    
    if (!value.HasMember("name") || !value.HasMember("type") ) {
        throw std::runtime_error("Missing fields in LocalVarDefStmt");
    }
    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer = value.HasMember("initializer") ? this->resolve_expr(value["initializer"]) : nullptr;
    return this->statementMaker_.mk_local_var_def(value["name"].GetString(),type,initializer);
}

