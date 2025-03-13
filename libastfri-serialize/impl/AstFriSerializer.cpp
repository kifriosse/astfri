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

    
    //first try to find node type between statements
    auto itStmt = astfri_serialize::strToStmtMapping.find(document_["node"].GetString());
    
    //if node wasnt found among statements,try to resolve type among expressions and lastly among types
    if (itStmt == astfri_serialize::strToStmtMapping.end()) {
        auto itExpr = astfri_serialize::strToExprMapping.find(document_["node"].GetString());

        if (itExpr == astfri_serialize::strToExprMapping.end()) {
            auto itType = astfri_serialize::strToTypeMapping.find(document_["node"].GetString());
            // if node name isnt either between expression and types throw exception
            if(itType == astfri_serialize::strToTypeMapping.end()) {throw std::runtime_error("Invalide node type");}
            return this->resolve_type(document_);
        }
        return this->resolve_expr(document_);
    }
    astfri::Stmt* stmt = this->resolve_stmt(document_);
    this->resolve_class_def_stmts(); 
    this->resolve_interface_def_stmts();
    this->clear_records();
    return stmt;

    

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
astfri::NullLiteralExpr* AstFriSerializer::serialize_null_lit_expr()
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


astfri::ParamVarRefExpr* AstFriSerializer::serialize_param_var_ref_expr(rapidjson::Value& value){

   return this->expressionMaker_.mk_param_var_ref(std::move(value["name"].GetString()));
}

astfri::LocalVarRefExpr* AstFriSerializer::serialize_local_var_ref_expr(rapidjson::Value& value){
    return this->expressionMaker_.mk_local_var_ref(std::move(value["name"].GetString()));
    
}
astfri::MemberVarRefExpr* AstFriSerializer::serialize_member_var_ref_expr(rapidjson::Value& value){
    astfri::Expr* owner = this->resolve_expr(value["owner"]);
    return  this->expressionMaker_.mk_member_var_ref(owner, std::move(value["name"].GetString()));
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

astfri::LambdaExpr* AstFriSerializer::serialize_lambda_expr(rapidjson::Value& value){
    std::vector<astfri::ParamVarDefStmt*> params;
    
    for (auto& param : value["params"].GetArray()){
        params.push_back(this->serialize_param_var_def_stmt(param));
    }
    astfri::Stmt* body = this->resolve_stmt(value["body"]);

   return this->expressionMaker_.mk_lambda_expr(std::move(params),body);
   
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
    return this->expressionMaker_.mk_delete(this->resolve_expr(value["expression"]));
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
            return this->serialize_null_lit_expr();
        case astfri_serialize::IfExpr:
            return this->serialize_if_expr(value);
        case astfri_serialize::BinOpExpr:
            return this->serialize_bin_op_expr(value);
        case astfri_serialize::UnaryOpExpr:
            return this->serialize_unary_op_expr(value);
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
        case astfri_serialize::ClassRefExpr:
            return this->serialize_class_ref_expr(value);


    }
    return expressionMaker_.mk_unknown();
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
            return this->typeMaker_.mk_dynamic();
            
    }
    return this->typeMaker_.mk_unknown();
}

astfri::Stmt* AstFriSerializer::resolve_stmt(rapidjson::Value& value){
    std::string name = value["node"].GetString();
    auto it = astfri_serialize::strToStmtMapping.find(value["node"].GetString());
    
    if (it == astfri_serialize::strToStmtMapping.end()) {
        throw std::runtime_error("Invalid node type");
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
            return this->serialize_unknown_stmt();
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
        case astfri_serialize::BreakStmt:
            return this->serialize_break_stmt();
        case astfri_serialize::ContinueStmt:
            return this->serialize_continue_stmt();
        case astfri_serialize::DefaultCaseStmt:
            return this->serialize_default_case_stmt(value);   
        case astfri_serialize::InterfaceDefStmt:
            return this->serialize_interface_def_stmt(value); 

    }

    return this->statementMaker_.mk_uknown();
}

astfri::LocalVarDefStmt* AstFriSerializer::serialize_local_var_def_stmt(rapidjson::Value& value){
    
    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer = value["initializer"].IsNull() ?  nullptr : this->resolve_expr(value["initializer"]);
    return this->statementMaker_.mk_local_var_def(std::move(value["name"].GetString()),type,initializer);
}

astfri::ParamVarDefStmt* AstFriSerializer::serialize_param_var_def_stmt(rapidjson::Value& value){
    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer = value["initializer"].IsNull() ?  nullptr : this->resolve_expr(value["initializer"]);
    return this->statementMaker_.mk_param_var_def(std::move(value["name"].GetString()),type,initializer);    

}

astfri::GlobalVarDefStmt* AstFriSerializer::serialize_global_var_def_stmt(rapidjson::Value& value){
    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer = value["initializer"].IsNull() ?  nullptr : this->resolve_expr(value["initializer"]);
    return this->statementMaker_.mk_global_var_def(std::move(value["name"].GetString()),type,initializer);
}

astfri::MemberVarDefStmt* AstFriSerializer::serialize_member_var_def_stmt(rapidjson::Value& value){
    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer = value["initializer"].IsNull() ?  nullptr : this->resolve_expr(value["initializer"]);
    astfri::AccessModifier modifier = accessModMapping.find(value["access"].GetString())->second; 

    return this->statementMaker_.mk_member_var_def(std::move(value["name"].GetString()),type,initializer,modifier);
}

astfri::FunctionDefStmt* AstFriSerializer::serialize_function_def_stmt(rapidjson::Value& value){
    std::vector<astfri::ParamVarDefStmt*> params ;

    for (auto& param : value["parameters"].GetArray()){
        params.push_back(this->serialize_param_var_def_stmt(param));
    }

    astfri::Type* returnType = this->resolve_type(value["return_type"]); 
    astfri::CompoundStmt* body = value["body"].IsNull() ? nullptr : this->serialize_compound_stmt(value["body"]);

    return this->statementMaker_.mk_function_def(std::move(value["name"].GetString()),std::move(params),returnType,body);
}

astfri::MethodDefStmt* AstFriSerializer::serialize_method_def_stmt(rapidjson::Value& value,astfri::ClassDefStmt* owner){
    std::string name = value["name"].GetString();
    astfri::FunctionDefStmt* functDefStmt = this->serialize_function_def_stmt(value);
    astfri::AccessModifier accessMod = accessModMapping.find(value["access"].GetString())->second;
    return this->statementMaker_.mk_method_def(owner,functDefStmt,accessMod, strcmp(value["virtual"].GetString(),"yes") == 0 ? 
                                                                            astfri::Virtuality::Virtual : astfri::Virtuality::NotVirtual);
}

astfri::GenericParam* AstFriSerializer::serialize_generic_param(rapidjson::Value& value){

    return this->statementMaker_.mk_generic_param(value["constraint"].IsNull() ? std::move("") : 
                                                    std::move(value["constraint"].GetString()),
                                                    std::move(value["name"].GetString()));
}

astfri::ClassDefStmt* AstFriSerializer::serialize_class_def_stmt(rapidjson::Value& value){
    
    astfri::ClassDefStmt*  classDefStmt = this->statementMaker_.mk_class_def(std::move(value["name"].GetString()));
    
    for (auto& attribute : value["attributes"].GetArray()){
        classDefStmt->vars_.push_back(this->serialize_member_var_def_stmt(attribute));
    }

    for (auto& genericParam : value["generic_parameters"].GetArray()){
        classDefStmt->tparams_.push_back(this->serialize_generic_param(genericParam));
    }

    for (auto& method : value["methods"].GetArray()){
        classDefStmt->methods_.push_back(this->serialize_method_def_stmt(method,classDefStmt));
    }
    
    for (auto& constructor : value["constructors"].GetArray()){
        classDefStmt->constructors_.push_back(this->serialize_constructor_def_stmt(constructor,classDefStmt));
    }

    for (auto& destructor : value["destructors"].GetArray()){
        classDefStmt->destructors_.push_back(this->serialize_destructor_def_stmt(destructor,classDefStmt));
    }

    //if the root of JSON file was Translation unit ,all InterfaceDefStmts should already be resolved
    for(auto& base : value["interfaces"].GetArray()){
        std::string name = base.GetString();
        auto it = nameWithInterfaceDefStmtMapping_.find(name);
        //if interface isnt among resolved interfaces create naked interface only with name and add it among resolved interfaces
        if (it==nameWithInterfaceDefStmtMapping_.end()){
            astfri::InterfaceDefStmt* interface = this->statementMaker_.mk_interface_def(base.GetString());
            nameWithInterfaceDefStmtMapping_[name]=interface;
            classDefStmt->interfaces_.push_back(interface);
        }else{
            classDefStmt->interfaces_.push_back(it->second);
        }
        
    }

    for (auto& base : value["bases"].GetArray() ){
        std::string name = base.GetString();
        auto it = nameWithClassDefStmtMapping_.find(name);
        //if class mentioned among base classes isnt resolved yet ,do nothing except for create record in unresolvedClassDefStmt map
        //and add this classDefStmt among classDefStmts which references to unresolved ClassDefStmt
        if (it==nameWithClassDefStmtMapping_.end()){
            unResolvedClassDefStmts_[name].push_back(classDefStmt);
            
        }else{
            classDefStmt->bases_.push_back(it->second);
        }
    }

    nameWithClassDefStmtMapping_[classDefStmt->name_]=classDefStmt;
    return classDefStmt;
    
}

astfri::CompoundStmt* AstFriSerializer::serialize_compound_stmt(rapidjson::Value& value){
    std::vector<astfri::Stmt*> statements;

    for(auto& stmt : value["statements"].GetArray()){
        statements.push_back(this->resolve_stmt(stmt));
    }
    return this->statementMaker_.mk_compound(std::move(statements)); 
}

astfri::ReturnStmt* AstFriSerializer::serialize_return_stmt(rapidjson::Value& value){
    
   return  this->statementMaker_.mk_return(value["value"].IsNull() ? nullptr : this->resolve_expr(value["value"]));
}

astfri::ExprStmt* AstFriSerializer::serialize_expr_stmt(rapidjson::Value& value){
   return  this->statementMaker_.mk_expr(this->resolve_expr(value["expression"]));

}

astfri::IfStmt* AstFriSerializer::serialize_if_stmt(rapidjson::Value& value){
    astfri::Expr* cond = this->resolve_expr(value["condition"]);
    astfri::Stmt* ifTrue = this->resolve_stmt(value["ifTrue"]);
    astfri::Stmt* ifFalse = value["ifFalse"].IsNull() ? nullptr : this->resolve_stmt(value["ifFalse"]);
    
    return this->statementMaker_.mk_if(cond,ifTrue,ifFalse);
}

astfri::SwitchStmt* AstFriSerializer::serialize_switch_stmt(rapidjson::Value& value){
    astfri::Expr* swichEntry = this->resolve_expr(value["entry"]);
    std::vector<astfri::CaseBaseStmt*> cases;

    for (auto& caze : value["cases"].GetArray()){
        cases.push_back(dynamic_cast<astfri::CaseBaseStmt*>(this->resolve_stmt(caze)));
    }

    
    return this->statementMaker_.mk_switch(swichEntry,std::move(cases));
}

astfri::CaseStmt* AstFriSerializer::serialize_case_stmt(rapidjson::Value& value){
    
    std::vector<astfri::Expr*> expressions;
    for(auto& expr : value["expressions"].GetArray()){
        expressions.push_back(this->resolve_expr(expr));
    }
    astfri::Stmt* body = this->resolve_stmt(value["body"]);

    
    return this->statementMaker_.mk_case(std::move(expressions),body);
}

astfri::WhileStmt* AstFriSerializer::serialize_while_stmt(rapidjson::Value& value){
    astfri::Expr* condition = this->resolve_expr(value["condition"]);
    astfri::Stmt* body = this->resolve_stmt(value["body"]);

    return this->statementMaker_.mk_while(condition,body);
}

astfri::DoWhileStmt* AstFriSerializer::serialize_do_while_stmt(rapidjson::Value& value){
    astfri::Expr* condition = this->resolve_expr(value["condition"]);
    astfri::Stmt* body = this->resolve_stmt(value["body"]);

    return this->statementMaker_.mk_do_while(condition,body);    
}

astfri::ForStmt* AstFriSerializer::serialize_for_stmt(rapidjson::Value& value){
    astfri::Stmt* init = value["init"].IsNull() ? nullptr : this->resolve_stmt(value["init"]);
    astfri::Expr* condition = value["condition"].IsNull() ? nullptr : this->resolve_expr(value["condition"]);
    astfri::Stmt* step = value["step"].IsNull() ? nullptr : this->resolve_stmt(value["step"]);
    astfri::Stmt* body = this->resolve_stmt(value["body"]);

    return this->statementMaker_.mk_for(init,condition,step,body);
}

astfri::ThrowStmt* AstFriSerializer::serialize_throw_stmt(rapidjson::Value& value){
    return this->statementMaker_.mk_throw(this->resolve_expr(value["expression"]));
}

astfri::UnknownStmt* AstFriSerializer::serialize_unknown_stmt(){
    return this->statementMaker_.mk_uknown();
}

astfri::TranslationUnit* AstFriSerializer::serialize_translation_unit(rapidjson::Value& value){
    
    std::vector<astfri::InterfaceDefStmt*> interfaces;
    for (auto& interface : value["interfaces"].GetArray()){
        interfaces.push_back(this->serialize_interface_def_stmt(interface));
    }
    
    std::vector<astfri::ClassDefStmt*> classes;
    for (auto& classDef : value["classes"].GetArray()){
        classes.push_back(this->serialize_class_def_stmt(classDef));
    }

    std::vector<astfri::FunctionDefStmt*> functions;
    for (auto& function : value["functions"].GetArray()){
        functions.push_back(this->serialize_function_def_stmt(function));
    }

    std::vector<astfri::GlobalVarDefStmt*> globals;
    for (auto& global : value["globals"].GetArray()){
        globals.push_back(this->serialize_global_var_def_stmt(global));
    }

    

    astfri::TranslationUnit* tu = this->statementMaker_.mk_translation_unit();
    tu->classes_ = std::move(classes);
    tu->functions_ = std::move(functions);
    tu->globals_ = std::move(globals);
    tu->interfaces_=std::move(interfaces);
    return tu;
}



astfri::ConstructorDefStmt* AstFriSerializer::serialize_constructor_def_stmt(rapidjson::Value& value,astfri::ClassDefStmt* owner){
    std::vector<astfri::ParamVarDefStmt*> params;
    for (auto& param : value["parameters"].GetArray()){
        params.push_back(this->serialize_param_var_def_stmt(param));
    }

    std::vector<astfri::BaseInitializerStmt*> initStmts;
    for(auto& baseInitStmt : value["base_initializers"].GetArray()){
        initStmts.push_back(this->serialize_base_initializer_stmt(baseInitStmt));
    }
    astfri::CompoundStmt* body = this->serialize_compound_stmt(value["body"]);
    astfri::AccessModifier accessModifier = accessModMapping.find(value["access"].GetString())->second;

    return this->statementMaker_.mk_constructor_def(owner,std::move(params),std::move(initStmts),body,accessModifier);
}

astfri::DestructorDefStmt* AstFriSerializer::serialize_destructor_def_stmt(rapidjson::Value& value,astfri::ClassDefStmt* owner){

    return this->statementMaker_.mk_destructor_def(owner,this->serialize_compound_stmt(value["body"]));
}

astfri::DefStmt* AstFriSerializer::serialize_def_stmt(rapidjson::Value& value){
    std::vector<astfri::VarDefStmt*> definitions;
    for(auto& def : value["definitions"].GetArray()){
        //?
        definitions.push_back(dynamic_cast<astfri::VarDefStmt*>(this->resolve_stmt(def)));  
    }

    return this->statementMaker_.mk_def(std::move(definitions));
}

astfri::BaseInitializerStmt* AstFriSerializer::serialize_base_initializer_stmt(rapidjson::Value& value){
    std::vector<astfri::Expr*> arguments;
    for(auto& arg : value["arguments"].GetArray()){
        arguments.push_back(this->resolve_expr(arg));
    }
    
    return this->statementMaker_.mak_base_initializer(value["base"].GetString(),std::move(arguments));
}

astfri::BreakStmt* AstFriSerializer::serialize_break_stmt(){
    return this->statementMaker_.mk_break();
}


astfri::ContinueStmt* AstFriSerializer::serialize_continue_stmt(){
    return this->statementMaker_.mk_continue();
}

astfri::DefaultCaseStmt* AstFriSerializer::serialize_default_case_stmt(rapidjson::Value& value){
    return this->statementMaker_.mk_default_case(this->resolve_stmt(value["body"]));
}

astfri::InterfaceDefStmt* AstFriSerializer::serialize_interface_def_stmt(rapidjson::Value& value){

    astfri::InterfaceDefStmt* interfDefStmt =  this->statementMaker_.mk_interface_def(std::move(value["name"].GetString()));
    
    for(auto& base : value["bases"].GetArray()){
        std::string name = base.GetString();
        auto it = nameWithInterfaceDefStmtMapping_.find(name);
        if (it == nameWithInterfaceDefStmtMapping_.end()){
            unresolvedInterfaceDefStmts_[name].push_back(interfDefStmt);
        }else{
            interfDefStmt->bases_.push_back(it->second);
        }
        
    }

    for(auto& method : value["methods"].GetArray()){
        interfDefStmt->methods_.push_back(this->serialize_method_def_stmt(method));
    }

    for(auto& genericParam : value["generic_parameters"].GetArray()){
        interfDefStmt->tparams_.push_back(this->serialize_generic_param(genericParam));
    }

    nameWithInterfaceDefStmtMapping_[interfDefStmt->name_]=interfDefStmt;
    return interfDefStmt;

}

void AstFriSerializer::resolve_class_def_stmts(){
    //traverse throw all unresolved classDefStmts
    for (auto& it : unResolvedClassDefStmts_){
        std::string name = it.first;
        //try to find name among already resolved classDefStmts
        auto itResolvedClassStmt =  nameWithClassDefStmtMapping_.find(name);

        //if name isnt among resolved statements ,create empty class def statement only with name,and add it to all child classes
        if(itResolvedClassStmt == nameWithClassDefStmtMapping_.end()){
            astfri::ClassDefStmt* classDefStmt = this->statementMaker_.mk_class_def(std::move(name));
            for(auto& clsDefStmt : it.second){
                clsDefStmt->bases_.push_back(classDefStmt);
            }

        } else {
            for(auto& clsDefStmt : it.second){
                clsDefStmt->bases_.push_back(itResolvedClassStmt->second);
            }
        }


    }


}

void AstFriSerializer::resolve_interface_def_stmts(){
    //traverse throw all unresolved interfaceDefStmts
    for (auto& it : unresolvedInterfaceDefStmts_){
        std::string name = it.first;
        
        //try to find name among already resolved interfaceDefStmts
        auto itResolvedInterfaceStmt = nameWithInterfaceDefStmtMapping_.find(name);
        if (itResolvedInterfaceStmt == nameWithInterfaceDefStmtMapping_.end()){
            astfri::InterfaceDefStmt* interfDefStmt = this->statementMaker_.mk_interface_def(std::move(name));
            for (auto& defStmt : it.second){
                if(is_class_def_stmt(defStmt)){
                    dynamic_cast<astfri::ClassDefStmt*>(defStmt)->interfaces_.push_back(interfDefStmt);
                }else{
                    dynamic_cast<astfri::InterfaceDefStmt*>(defStmt)->bases_.push_back(interfDefStmt);
                }

            }
        }else{
            astfri::InterfaceDefStmt* interfDefStmt = itResolvedInterfaceStmt->second;
            for (auto& defStmt : it.second){
                if(is_class_def_stmt(defStmt)){
                    dynamic_cast<astfri::ClassDefStmt*>(defStmt)->interfaces_.push_back(interfDefStmt);
                }else{
                    dynamic_cast<astfri::InterfaceDefStmt*>(defStmt)->bases_.push_back(interfDefStmt);
                }

            }

        }

    }
}

bool AstFriSerializer::is_class_def_stmt(astfri::Stmt* stmt){
    return dynamic_cast<astfri::ClassDefStmt*>(stmt) != nullptr;
}

void AstFriSerializer::clear_records(){
    nameWithClassDefStmtMapping_.clear();
    nameWithInterfaceDefStmtMapping_.clear();
    unResolvedClassDefStmts_.clear();
    unresolvedInterfaceDefStmts_.clear();
}
