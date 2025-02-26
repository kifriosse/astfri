#include <libastfri-serialize/inc/AstFriSerializer.hpp>

AstFriSerializer& AstFriSerializer::getInstance(){
    static AstFriSerializer instance;
    return instance;
}
AstFriSerializer::AstFriSerializer()
    :expressionMaker_(astfri::ExprFactory::get_instance()),
    statementMaker_(astfri::StmtFactory::get_instance()),
    typeMaker_(astfri::TypeFactory::get_instance()),
    jsonChecker_(JsonFormatChecker::getInstance()){}

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

    return this->resolveExpr(document_);

    

}




//check for proper json format for LiteralExpressions with value
void AstFriSerializer::checkJsonFormatLitExpressions(rapidjson::Value& jsonVal)
{
   
    if(!jsonVal.HasMember("value"))
    {
        throw std::runtime_error("Missing value field");
    }
}

astfri::IntLiteralExpr* AstFriSerializer::serializeIntLiteralExpr(rapidjson::Value& value)
{
    this->checkJsonFormatLitExpressions(value);

    return expressionMaker_.mk_int_literal(value["value"].GetInt());
}

astfri::FloatLiteralExpr* AstFriSerializer::serializeFloatLiteralExpr(rapidjson::Value& value)
{
    this->checkJsonFormatLitExpressions(value);
    
    return expressionMaker_.mk_float_literal(value["value"].GetFloat());
}

astfri::CharLiteralExpr* AstFriSerializer::serializeCharLiteralExpr(rapidjson::Value& value)
{
    this->checkJsonFormatLitExpressions(value);
    
    return expressionMaker_.mk_char_literal(value["value"].GetString()[0]);
}

astfri::StringLiteralExpr* AstFriSerializer::serializeStringLiteralExpr(rapidjson::Value& value)
{
    this->checkJsonFormatLitExpressions(value);
    
    return expressionMaker_.mk_string_literal(value["value"].GetString());
}
astfri::BoolLiteralExpr* AstFriSerializer::serializeBoolLiteralExpr(rapidjson::Value& value)
{
    this->checkJsonFormatLitExpressions(value);
    
    return expressionMaker_.mk_bool_literal(value["value"].GetBool());
}
astfri::NullLiteralExpr* AstFriSerializer::serializeNullLiteralExpr(rapidjson::Value& value)
{
   
    return expressionMaker_.mk_null_literal();
}
astfri::IfExpr* AstFriSerializer::serializeIfExpr(rapidjson::Value& value)
{

    if (!value.HasMember("condition") || !value.HasMember("ifTrue") || !value.HasMember("ifFalse")) {
        throw std::runtime_error("Missing fields in IfExpr");
    }
    astfri::Expr* cond = this->resolveExpr(value["condition"]);
    astfri::Expr* iftrue = this->resolveExpr(value["ifTrue"]);
    astfri::Expr* iffalse = this->resolveExpr(value["ifFalse"]);
    return expressionMaker_.mk_if(cond,iftrue,iffalse);
    
}

astfri::BinOpExpr* AstFriSerializer::serializeBinOpExpr(rapidjson::Value& value){

    
    astfri::Expr* left = this->resolveExpr(value["left"]);
    astfri::Expr* right = this->resolveExpr(value["right"]);
    
    auto it = astfri_serialize::binOpTypeMapping.find(value["operator"].GetString());
    if (it == astfri_serialize::binOpTypeMapping.end()) {
        throw std::runtime_error("Invalid operator in BinOpExpr");
    }

    astfri::BinOpType op = it->second;
    return this->expressionMaker_.mk_bin_on(left,op,right);
}
astfri::UnaryOpExpr* AstFriSerializer::serializeUnaryOpExpr(rapidjson::Value& val){

    astfri::Expr* arg = this->resolveExpr(val["argument"]);
    auto it = astfri_serialize::unaryOpTypeMapping.find(val["operator"].GetString());
    if (it == astfri_serialize::unaryOpTypeMapping.end()) {
        throw std::runtime_error("Invalid operator in UnaryOpExpr");
    }

    return this->expressionMaker_.mk_unary_op(it->second,arg);
}
astfri::AssignExpr* AstFriSerializer::serializeAssignExpr(rapidjson::Value& val){


    return this->expressionMaker_.mk_assign(this->resolveExpr(val["left"]),this->resolveExpr(val["right"]));
}
astfri::CompoundAssignExpr* AstFriSerializer::serializeCompoundAssignExpr(rapidjson::Value& val){
    astfri::Expr* left = this->resolveExpr(val["left"]);
    astfri::Expr* right= this->resolveExpr(val["right"]);

    auto it = astfri_serialize::binOpTypeMapping.find(val["operator"].GetString());
    if (it == astfri_serialize::binOpTypeMapping.end()) {
        throw std::runtime_error("Invalid operator in BinOpExpr");
    }

    return this->expressionMaker_.mk_compound_assign(left,it->second,right);
}


astfri::ParamVarRefExpr* AstFriSerializer::serializeParamVarRefExpr(rapidjson::Value& val){

   return this->expressionMaker_.mk_param_var_ref(std::move(val["name"].GetString()));
}

astfri::LocalVarRefExpr* AstFriSerializer::serializeLocalVarRefExpr(rapidjson::Value& val){
    return this->expressionMaker_.mk_local_var_ref(std::move(val["name"].GetString()));
    
}
astfri::MemberVarRefExpr* AstFriSerializer::serializeMemberVarRefExpr(rapidjson::Value& val){
   return  this->expressionMaker_.mk_member_var_ref(std::move(val["name"].GetString()));
}

astfri::GlobalVarRefExpr* AstFriSerializer::serializeGlobalVarRefExpr(rapidjson::Value& val){
    return this->expressionMaker_.mk_global_var_ref(std::move(val["name"].GetString()));
}

astfri::FunctionCallExpr* AstFriSerializer::serializeFunctionCallExpr(rapidjson::Value& val){
    std::vector<astfri::Expr*> arguments;

    for(auto& arg : val["arguments"].GetArray()){
        arguments.push_back(this->resolveExpr(arg));

    }

    return this->expressionMaker_.mk_function_call(std::move(val["name"].GetString()),std::move(arguments));
}

astfri::MethodCallExpr* AstFriSerializer::serializeMethodCallExpr(rapidjson::Value& val){
    astfri::Expr* owner = this->resolveExpr(val["owner"]);

    std::vector<astfri::Expr*> arguments;
    for(auto& arg : val["arguments"].GetArray()){
        arguments.push_back(this->resolveExpr(arg));
    }

    return this->expressionMaker_.mk_method_call(owner,std::move(val["name"].GetString()),std::move(arguments));
}
astfri::LambdaExpr* AstFriSerializer::serializeLambdaExpr(rapidjson::Value& val){
    std::vector<astfri::ParamVarDefStmt*> params;
    /*
    for (auto& param : val["params"].GetArray()){
        params.push_back(this->serializeParamVarDefStmt(param));
    }
    astfri::Stmt* body = this->resolveStmt(val["body"]);

   return this->expressionMaker_.mk_lambda_expr(std::move(params),body);*/
   return this->expressionMaker_.mk_lambda_expr(params,nullptr);
}

astfri::ThisExpr* AstFriSerializer::serializeThisExpr(){
    return this->expressionMaker_.mk_this();
}
astfri::UnknownExpr* AstFriSerializer::serializeUnknownExpr(){
    return this->expressionMaker_.mk_unknown();
}

astfri::Expr* AstFriSerializer::resolveExpr(rapidjson::Value& value)
{
    if (!value.HasMember("node")) {
        throw std::runtime_error("Missing node field in expression");
    }
    if (!value["node"].IsString()) {
        throw std::runtime_error("Invalid node field in expression");
    }
    auto it = astfri_serialize::strToExprMapping.find(value["node"].GetString());
    if (it == astfri_serialize::strToExprMapping.end()) {
        throw std::runtime_error("Invalid node type in expression");
    }
    switch(it->second){
        case astfri_serialize::IntLiteralExpr:
            return this->serializeIntLiteralExpr(value);
        case astfri_serialize::FloatLiteralExpr:
            return this->serializeFloatLiteralExpr(value);
        case astfri_serialize::CharLiteralExpr:
            return this->serializeCharLiteralExpr(value);
        case astfri_serialize::StringLiteralExpr:
            return this->serializeStringLiteralExpr(value);
        case astfri_serialize::BoolLiteralExpr:
            return this->serializeBoolLiteralExpr(value);
        case astfri_serialize::NullLiteralExpr:
            return this->serializeNullLiteralExpr(value);
        case astfri_serialize::IfExpr:
            return this->serializeIfExpr(value);
        case astfri_serialize::BinOpExpr:
            return this->serializeBinOpExpr(value);
        case astfri_serialize::UnaryOpExpr:
            return this->serializeUnaryOpExpr(value);
        case astfri_serialize::AssignExpr:
            return this->serializeAssignExpr(value);
        case astfri_serialize::CompoundAssignExpr:
            return this->serializeCompoundAssignExpr(value);
        case astfri_serialize::ParamVarRefExpr:
            return this->serializeParamVarRefExpr(value);
        case astfri_serialize::LocalVarRefExpr:
            return this->serializeLocalVarRefExpr(value);
        case astfri_serialize::MemberVarRefExpr:
            return this->serializeMemberVarRefExpr(value);
        case astfri_serialize::GlobalVarRefExpr:
            return this->serializeGlobalVarRefExpr(value);
        case astfri_serialize::FunctionCallExpr:
            return this->serializeFunctionCallExpr(value);
        case astfri_serialize::MethodCallExpr:
            return this->serializeMethodCallExpr(value);
        case astfri_serialize::LambdaExpr:
            return this->serializeLambdaExpr(value);
        case astfri_serialize::ThisExpr:
            return this->serializeThisExpr();
        case astfri_serialize::UnknownExpr:
            return this->serializeUnknownExpr();

    }

}
astfri::Type* AstFriSerializer::resolveType(rapidjson::Value& value)
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
            return this->typeMaker_.mk_indirect(this->resolveType(value["indirect"]));
        case astfri_serialize::UnknownType:
            return this->typeMaker_.mk_unknown();
        case astfri_serialize::DynamicType:
            //return this->typeMaker_.mk_dynamic();
            return nullptr;
    }
    
}

astfri::LocalVarDefStmt* AstFriSerializer::serializeLocalVarDefStmt(rapidjson::Value& value){
    
    if (!value.HasMember("name") || !value.HasMember("type") ) {
        throw std::runtime_error("Missing fields in LocalVarDefStmt");
    }
    astfri::Type* type = this->resolveType(value["type"]);
    astfri::Expr* initializer = value.HasMember("initializer") ? this->resolveExpr(value["initializer"]) : nullptr;
    return this->statementMaker_.mk_local_var_def(value["name"].GetString(),type,initializer);
}

