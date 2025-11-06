#include <libastfri-serialize/inc/AstFriDeSerializer.hpp>

#include <fstream>

AstFriDeSerializer& AstFriDeSerializer::get_instance()
{
    static AstFriDeSerializer instance;
    return instance;
}

AstFriDeSerializer::AstFriDeSerializer() :
    expressionMaker_(astfri::ExprFactory::get_instance()),
    statementMaker_(astfri::StmtFactory::get_instance()),
    typeMaker_(astfri::TypeFactory::get_instance()),
    jsonChecker_(JsonFormatChecker::get_instance())
{
}

astfri::IVisitable* AstFriDeSerializer::deserialize(std::string filePath)
{

    std::ifstream file(filePath);
    if (! file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }
    rapidjson::IStreamWrapper isw(file);
    rapidjson::Document document_;
    document_.ParseStream(isw);

    if (document_.HasParseError())
    {
        throw std::runtime_error(
            "JSON pardese error: " + std::to_string(document_.GetParseError())
        );
    }
    if (! document_.HasMember("node"))
    {
        throw std::runtime_error("Missing node field\n Unable to detect node type!");
    }
    if (! document_["node"].IsString())
    {
        throw std::runtime_error("Invalid node format");
    }

    // first try to find node type between statements
    auto itStmt = astfri_deserialize::strToStmtMapping.find(document_["node"].GetString());

    // if node wasnt found among statements,try to resolve type among expressions and lastly among
    // types
    if (itStmt == astfri_deserialize::strToStmtMapping.end())
    {
        auto itExpr = astfri_deserialize::strToExprMapping.find(document_["node"].GetString());

        if (itExpr == astfri_deserialize::strToExprMapping.end())
        {
            auto itType = astfri_deserialize::strToTypeMapping.find(document_["node"].GetString());
            // if node name isnt either between expression and types throw exception
            if (itType == astfri_deserialize::strToTypeMapping.end())
            {
                throw std::runtime_error("Invalide node type");
            }
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

astfri::IntLiteralExpr* AstFriDeSerializer::deserialize_int_lit_expr(rapidjson::Value& value)
{

    return expressionMaker_.mk_int_literal(value["value"].GetInt());
}

astfri::FloatLiteralExpr* AstFriDeSerializer::deserialize_float_lit_expr(rapidjson::Value& value)
{

    return expressionMaker_.mk_float_literal(value["value"].GetFloat());
}

astfri::CharLiteralExpr* AstFriDeSerializer::deserialize_char_lit_expr(rapidjson::Value& value)
{

    return expressionMaker_.mk_char_literal(value["value"].GetString()[0]);
}

astfri::StringLiteralExpr* AstFriDeSerializer::deserialize_string_lit_expr(rapidjson::Value& value)
{

    return expressionMaker_.mk_string_literal(value["value"].GetString());
}

astfri::BoolLiteralExpr* AstFriDeSerializer::deserialize_bool_lit_expr(rapidjson::Value& value)
{

    return expressionMaker_.mk_bool_literal(value["value"].GetBool());
}

astfri::NullLiteralExpr* AstFriDeSerializer::deserialize_null_lit_expr()
{

    return expressionMaker_.mk_null_literal();
}

astfri::IfExpr* AstFriDeSerializer::deserialize_if_expr(rapidjson::Value& value)
{

    astfri::Expr* cond    = this->resolve_expr(value["condition"]);
    astfri::Expr* iftrue  = this->resolve_expr(value["ifTrue"]);
    astfri::Expr* iffalse = this->resolve_expr(value["ifFalse"]);
    return expressionMaker_.mk_if(cond, iftrue, iffalse);
}

astfri::BinOpExpr* AstFriDeSerializer::deserialize_bin_op_expr(rapidjson::Value& value)
{

    astfri::Expr* left  = this->resolve_expr(value["left"]);
    astfri::Expr* right = this->resolve_expr(value["right"]);

    auto it             = astfri_deserialize::binOpTypeMapping.find(value["operator"].GetString());
    if (it == astfri_deserialize::binOpTypeMapping.end())
    {
        throw std::runtime_error("Invalid operator in BinOpExpr");
    }

    astfri::BinOpType op = it->second;
    return this->expressionMaker_.mk_bin_on(left, op, right);
}

astfri::UnaryOpExpr* AstFriDeSerializer::deserialize_unary_op_expr(rapidjson::Value& value)
{

    astfri::Expr* arg = this->resolve_expr(value["argument"]);
    auto it           = astfri_deserialize::unaryOpTypeMapping.find(value["operator"].GetString());
    if (it == astfri_deserialize::unaryOpTypeMapping.end())
    {
        throw std::runtime_error("Invalid operator in UnaryOpExpr");
    }
    astfri::UnaryOpType op = it->second;

    // if it s pre increment/decrement ,check property isPostfix to determine which operator it
    // really is,becaudese in mapping there or only pre variants
    if (op == astfri::UnaryOpType::PreIncrement || op == astfri::UnaryOpType::PreDecrement)
    {
        std::string opString = value["operator"].GetString();
        // if operator is postfix change op to proper operator type
        if (value["isPostfix"].GetBool())
        {
            op = opString == "++" ? astfri::UnaryOpType::PostIncrement
                                  : astfri::UnaryOpType::PostDecrement;
        }
    }

    return this->expressionMaker_.mk_unary_op(it->second, arg);
}

astfri::ParamVarRefExpr* AstFriDeSerializer::deserialize_param_var_ref_expr(rapidjson::Value& value)
{

    return this->expressionMaker_.mk_param_var_ref(std::move(value["name"].GetString()));
}

astfri::LocalVarRefExpr* AstFriDeSerializer::deserialize_local_var_ref_expr(rapidjson::Value& value)
{
    return this->expressionMaker_.mk_local_var_ref(std::move(value["name"].GetString()));
}

astfri::MemberVarRefExpr* AstFriDeSerializer::deserialize_member_var_ref_expr(
    rapidjson::Value& value
)
{
    astfri::Expr* owner = this->resolve_expr(value["owner"]);
    return this->expressionMaker_.mk_member_var_ref(owner, std::move(value["name"].GetString()));
}

astfri::GlobalVarRefExpr* AstFriDeSerializer::deserialize_global_var_ref_expr(
    rapidjson::Value& value
)
{
    return this->expressionMaker_.mk_global_var_ref(std::move(value["name"].GetString()));
}

astfri::ClassRefExpr* AstFriDeSerializer::deserialize_class_ref_expr(rapidjson::Value& value)
{
    return this->expressionMaker_.mk_class_ref(std::move(value["name"].GetString()));
}

astfri::FunctionCallExpr* AstFriDeSerializer::deserialize_function_call_expr(rapidjson::Value& value
)
{
    std::vector<astfri::Expr*> arguments;

    for (auto& arg : value["arguments"].GetArray())
    {
        arguments.push_back(this->resolve_expr(arg));
    }

    return this->expressionMaker_.mk_function_call(
        std::move(value["name"].GetString()),
        std::move(arguments)
    );
}

astfri::MethodCallExpr* AstFriDeSerializer::deserialize_method_call_expr(rapidjson::Value& value)
{

    std::vector<astfri::Expr*> arguments;
    for (auto& arg : value["arguments"].GetArray())
    {
        arguments.push_back(this->resolve_expr(arg));
    }

    return this->expressionMaker_.mk_method_call(
        value["owner"].IsNull() ? nullptr : this->resolve_expr(value["owner"]),
        std::move(value["name"].GetString()),
        std::move(arguments)
    );
}

astfri::LambdaExpr* AstFriDeSerializer::deserialize_lambda_expr(rapidjson::Value& value)
{
    std::vector<astfri::ParamVarDefStmt*> params;

    for (auto& param : value["parameters"].GetArray())
    {
        params.push_back(this->deserialize_param_var_def_stmt(param));
    }
    astfri::Stmt* body = this->resolve_stmt(value["body"]);

    return this->expressionMaker_.mk_lambda_expr(std::move(params), body);
}

astfri::LambdaCallExpr* AstFriDeSerializer::deserialize_lambda_call_expr(rapidjson::Value& value)
{
    astfri::Expr* lambda = this->resolve_expr(value["lambda"]);
    std::vector<astfri::Expr*> arguments;
    for (auto& arg : value["arguments"].GetArray())
    {
        arguments.push_back(this->resolve_expr(arg));
    }
    return this->expressionMaker_.mk_lambda_call(lambda, std::move(arguments));
}

astfri::ThisExpr* AstFriDeSerializer::deserialize_this_expr()
{
    return this->expressionMaker_.mk_this();
}

astfri::UnknownExpr* AstFriDeSerializer::deserialize_unknown_expr()
{
    return this->expressionMaker_.mk_unknown();
}

astfri::ConstructorCallExpr* AstFriDeSerializer::deserialize_constructor_call_expr(
    rapidjson::Value& value
)
{
    std::vector<astfri::Expr*> arguments;

    for (auto& arg : value["arguments"].GetArray())
    {
        arguments.push_back(this->resolve_expr(arg));
    }

    return this->expressionMaker_.mk_constructor_call(
        this->resolve_type(value["type"]),
        std::move(arguments)
    );
}

astfri::NewExpr* AstFriDeSerializer::deserialize_new_expr(rapidjson::Value& value)
{
    return this->expressionMaker_.mk_new(this->deserialize_constructor_call_expr(value["init"]));
}

astfri::DeleteExpr* AstFriDeSerializer::deserialize_delete_expr(rapidjson::Value& value)
{
    return this->expressionMaker_.mk_delete(this->resolve_expr(value["expression"]));
}

astfri::Expr* AstFriDeSerializer::resolve_expr(rapidjson::Value& value)
{

    auto it = astfri_deserialize::strToExprMapping.find(value["node"].GetString());
    if (it == astfri_deserialize::strToExprMapping.end())
    {
        throw std::runtime_error("Invalid node type in expression");
    }
    switch (it->second)
    {
    case astfri_deserialize::IntLiteralExpr:
        return this->deserialize_int_lit_expr(value);
    case astfri_deserialize::FloatLiteralExpr:
        return this->deserialize_float_lit_expr(value);
    case astfri_deserialize::CharLiteralExpr:
        return this->deserialize_char_lit_expr(value);
    case astfri_deserialize::StringLiteralExpr:
        return this->deserialize_string_lit_expr(value);
    case astfri_deserialize::BoolLiteralExpr:
        return this->deserialize_bool_lit_expr(value);
    case astfri_deserialize::NullLiteralExpr:
        return this->deserialize_null_lit_expr();
    case astfri_deserialize::IfExpr:
        return this->deserialize_if_expr(value);
    case astfri_deserialize::BinOpExpr:
        return this->deserialize_bin_op_expr(value);
    case astfri_deserialize::UnaryOpExpr:
        return this->deserialize_unary_op_expr(value);
    case astfri_deserialize::ParamVarRefExpr:
        return this->deserialize_param_var_ref_expr(value);
    case astfri_deserialize::LocalVarRefExpr:
        return this->deserialize_local_var_ref_expr(value);
    case astfri_deserialize::MemberVarRefExpr:
        return this->deserialize_member_var_ref_expr(value);
    case astfri_deserialize::GlobalVarRefExpr:
        return this->deserialize_global_var_ref_expr(value);
    case astfri_deserialize::FunctionCallExpr:
        return this->deserialize_function_call_expr(value);
    case astfri_deserialize::MethodCallExpr:
        return this->deserialize_method_call_expr(value);
    case astfri_deserialize::LambdaExpr:
        return this->deserialize_lambda_expr(value);
    case astfri_deserialize::ThisExpr:
        return this->deserialize_this_expr();
    case astfri_deserialize::UnknownExpr:
        return this->deserialize_unknown_expr();
    case astfri_deserialize::NewExpr:
        return this->deserialize_new_expr(value);
    case astfri_deserialize::DeleteExpr:
        return this->deserialize_delete_expr(value);
    case astfri_deserialize::ConstructorCallExpr:
        return this->deserialize_constructor_call_expr(value);
    case astfri_deserialize::ClassRefExpr:
        return this->deserialize_class_ref_expr(value);
    case astfri_deserialize::LambdaCallExpr:
        return this->deserialize_lambda_call_expr(value);
    }
    return expressionMaker_.mk_unknown();
}

astfri::Type* AstFriDeSerializer::resolve_type(rapidjson::Value& value)
{
    auto it = astfri_deserialize::strToTypeMapping.find(value["node"].GetString());
    if (it == astfri_deserialize::strToTypeMapping.end())
    {
        throw std::runtime_error("Invalid type");
    }
    switch (it->second)
    {
    case astfri_deserialize::IntType:
        return this->typeMaker_.mk_int();
    case astfri_deserialize::FloatType:
        return this->typeMaker_.mk_float();
    case astfri_deserialize::CharType:
        return this->typeMaker_.mk_char();
    case astfri_deserialize::BoolType:
        return this->typeMaker_.mk_bool();
    case astfri_deserialize::VoidType:
        return this->typeMaker_.mk_void();
    case astfri_deserialize::UserType:
        return this->typeMaker_.mk_class(value["name"].GetString(), {});
    case astfri_deserialize::IndirectionType:
        return this->typeMaker_.mk_indirect(this->resolve_type(value["indirect"]));
    case astfri_deserialize::UnknownType:
        return this->typeMaker_.mk_unknown();
    case astfri_deserialize::DynamicType:
        return this->typeMaker_.mk_dynamic();
    }
    return this->typeMaker_.mk_unknown();
}

astfri::Stmt* AstFriDeSerializer::resolve_stmt(rapidjson::Value& value)
{
    std::string name = value["node"].GetString();
    auto it          = astfri_deserialize::strToStmtMapping.find(value["node"].GetString());

    if (it == astfri_deserialize::strToStmtMapping.end())
    {
        throw std::runtime_error("Invalid node type");
    }

    switch (it->second)
    {
    case astfri_deserialize::LocalVarDefStmt:
        return this->deserialize_local_var_def_stmt(value);
    case astfri_deserialize::ParamVarDefStmt:
        return this->deserialize_param_var_def_stmt(value);
    case astfri_deserialize::MemberVarDefStmt:
        return this->deserialize_member_var_def_stmt(value);
    case astfri_deserialize::GlobalVarDefStmt:
        return this->deserialize_global_var_def_stmt(value);
    case astfri_deserialize::FunctionDefStmt:
        return this->deserialize_function_def_stmt(value);
    case astfri_deserialize::MethodDefStmt:
        return this->deserialize_method_def_stmt(value);
    case astfri_deserialize::GenericParam:
        return this->deserialize_generic_param(value);
    case astfri_deserialize::ClassDefStmt:
        return this->deserialize_class_def_stmt(value);
    case astfri_deserialize::CompoundStmt:
        return this->deserialize_compound_stmt(value);
    case astfri_deserialize::ReturnStmt:
        return this->deserialize_return_stmt(value);
    case astfri_deserialize::ExprStmt:
        return this->deserialize_expr_stmt(value);
    case astfri_deserialize::IfStmt:
        return this->deserialize_if_stmt(value);
    case astfri_deserialize::SwitchStmt:
        return this->deserialize_switch_stmt(value);
    case astfri_deserialize::CaseStmt:
        return this->deserialize_case_stmt(value);
    case astfri_deserialize::WhileStmt:
        return this->deserialize_while_stmt(value);
    case astfri_deserialize::DoWhileStmt:
        return this->deserialize_do_while_stmt(value);
    case astfri_deserialize::ForStmt:
        return this->deserialize_for_stmt(value);
    case astfri_deserialize::ThrowStmt:
        return this->deserialize_throw_stmt(value);
    case astfri_deserialize::UnknownStmt:
        return this->deserialize_unknown_stmt();
    case astfri_deserialize::TranslationUnit:
        return this->deserialize_translation_unit(value);
    case astfri_deserialize::DefStmt:
        return this->deserialize_def_stmt(value);
    case astfri_deserialize::ConstructorDefStmt:
        return this->deserialize_constructor_def_stmt(value);
    case astfri_deserialize::DestructorDefStmt:
        return this->deserialize_destructor_def_stmt(value);
    case astfri_deserialize::BaseInitializerStmt:
        return this->deserialize_base_initializer_stmt(value);
    case astfri_deserialize::BreakStmt:
        return this->deserialize_break_stmt();
    case astfri_deserialize::ContinueStmt:
        return this->deserialize_continue_stmt();
    case astfri_deserialize::DefaultCaseStmt:
        return this->deserialize_default_case_stmt(value);
    case astfri_deserialize::InterfaceDefStmt:
        return this->deserialize_interface_def_stmt(value);
    }

    return this->statementMaker_.mk_uknown();
}

astfri::LocalVarDefStmt* AstFriDeSerializer::deserialize_local_var_def_stmt(rapidjson::Value& value)
{

    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer
        = value["initializer"].IsNull() ? nullptr : this->resolve_expr(value["initializer"]);
    return this->statementMaker_
        .mk_local_var_def(std::move(value["name"].GetString()), type, initializer);
}

astfri::ParamVarDefStmt* AstFriDeSerializer::deserialize_param_var_def_stmt(rapidjson::Value& value)
{
    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer
        = value["initializer"].IsNull() ? nullptr : this->resolve_expr(value["initializer"]);
    return this->statementMaker_
        .mk_param_var_def(std::move(value["name"].GetString()), type, initializer);
}

astfri::GlobalVarDefStmt* AstFriDeSerializer::deserialize_global_var_def_stmt(
    rapidjson::Value& value
)
{
    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer
        = value["initializer"].IsNull() ? nullptr : this->resolve_expr(value["initializer"]);
    return this->statementMaker_
        .mk_global_var_def(std::move(value["name"].GetString()), type, initializer);
}

astfri::MemberVarDefStmt* AstFriDeSerializer::deserialize_member_var_def_stmt(
    rapidjson::Value& value
)
{
    astfri::Type* type = this->resolve_type(value["type"]);
    astfri::Expr* initializer
        = value["initializer"].IsNull() ? nullptr : this->resolve_expr(value["initializer"]);
    astfri::AccessModifier modifier = accessModMapping.find(value["access"].GetString())->second;

    return this->statementMaker_
        .mk_member_var_def(std::move(value["name"].GetString()), type, initializer, modifier);
}

astfri::FunctionDefStmt* AstFriDeSerializer::deserialize_function_def_stmt(rapidjson::Value& value)
{
    std::vector<astfri::ParamVarDefStmt*> params;

    for (auto& param : value["parameters"].GetArray())
    {
        params.push_back(this->deserialize_param_var_def_stmt(param));
    }

    astfri::Type* returnType = this->resolve_type(value["return_type"]);
    astfri::CompoundStmt* body
        = value["body"].IsNull() ? nullptr : this->deserialize_compound_stmt(value["body"]);

    return this->statementMaker_
        .mk_function_def(std::move(value["name"].GetString()), std::move(params), returnType, body);
}

astfri::MethodDefStmt* AstFriDeSerializer::deserialize_method_def_stmt(
    rapidjson::Value& value,
    astfri::UserTypeDefStmt* owner
)
{
    std::string name                      = value["name"].GetString();
    astfri::FunctionDefStmt* functDefStmt = this->deserialize_function_def_stmt(value);
    astfri::AccessModifier accessMod = accessModMapping.find(value["access"].GetString())->second;
    return this->statementMaker_.mk_method_def(
        owner,
        functDefStmt,
        accessMod,
        strcmp(value["virtual"].GetString(), "yes") == 0 ? astfri::Virtuality::Virtual
                                                         : astfri::Virtuality::NotVirtual
    );
}

astfri::GenericParam* AstFriDeSerializer::deserialize_generic_param(rapidjson::Value& value)
{

    return this->statementMaker_.mk_generic_param(
        value["constraint"].IsNull() ? std::move("") : std::move(value["constraint"].GetString()),
        std::move(value["name"].GetString())
    );
}

astfri::ClassDefStmt* AstFriDeSerializer::deserialize_class_def_stmt(rapidjson::Value& value)
{

    astfri::ClassDefStmt* classDefStmt
        = this->statementMaker_.mk_class_def(std::move(value["name"].GetString()), astfri::mk_scope());

    for (auto& attribute : value["attributes"].GetArray())
    {
        classDefStmt->vars_.push_back(this->deserialize_member_var_def_stmt(attribute));
    }

    for (auto& genericParam : value["generic_parameters"].GetArray())
    {
        classDefStmt->tparams_.push_back(this->deserialize_generic_param(genericParam));
    }

    for (auto& method : value["methods"].GetArray())
    {
        classDefStmt->methods_.push_back(this->deserialize_method_def_stmt(method, classDefStmt));
    }

    for (auto& constructor : value["constructors"].GetArray())
    {
        classDefStmt->constructors_.push_back(
            this->deserialize_constructor_def_stmt(constructor, classDefStmt)
        );
    }

    for (auto& destructor : value["destructors"].GetArray())
    {
        classDefStmt->destructors_.push_back(
            this->deserialize_destructor_def_stmt(destructor, classDefStmt)
        );
    }

    // if the root of JSON file was Translation unit ,all InterfaceDefStmts should already be
    // resolved
    for (auto& interf : value["interfaces"].GetArray())
    {
        std::string name = interf.GetString();
        auto it          = nameWithInterfaceDefStmtMapping_.find(name);
        // if interface isnt among resolved interfaces create naked interface only with name and add
        // it among resolved interfaces
        if (it == nameWithInterfaceDefStmtMapping_.end())
        {
            astfri::InterfaceDefStmt* interface    = this->statementMaker_.mk_interface_def(name);
            nameWithInterfaceDefStmtMapping_[name] = interface;
            classDefStmt->interfaces_.push_back(interface);
        }
        else
        {
            classDefStmt->interfaces_.push_back(it->second);
        }
    }

    for (auto& base : value["bases"].GetArray())
    {
        std::string name = base.GetString();
        auto it          = nameWithClassDefStmtMapping_.find(name);
        // if class mentioned among base classes isnt resolved yet ,do nothing except for create
        // record in unresolvedClassDefStmt map and add this classDefStmt among classDefStmts which
        // references to unresolved ClassDefStmt
        if (it == nameWithClassDefStmtMapping_.end())
        {
            unResolvedClassDefStmts_[name].push_back(classDefStmt);
        }
        else
        {
            classDefStmt->bases_.push_back(it->second);
        }
    }

    nameWithClassDefStmtMapping_[classDefStmt->name_] = classDefStmt;
    return classDefStmt;
}

astfri::CompoundStmt* AstFriDeSerializer::deserialize_compound_stmt(rapidjson::Value& value)
{
    std::vector<astfri::Stmt*> statements;

    for (auto& stmt : value["statements"].GetArray())
    {
        statements.push_back(this->resolve_stmt(stmt));
    }
    return this->statementMaker_.mk_compound(std::move(statements));
}

astfri::ReturnStmt* AstFriDeSerializer::deserialize_return_stmt(rapidjson::Value& value)
{

    return this->statementMaker_.mk_return(
        value["value"].IsNull() ? nullptr : this->resolve_expr(value["value"])
    );
}

astfri::ExprStmt* AstFriDeSerializer::deserialize_expr_stmt(rapidjson::Value& value)
{
    return this->statementMaker_.mk_expr(this->resolve_expr(value["expression"]));
}

astfri::IfStmt* AstFriDeSerializer::deserialize_if_stmt(rapidjson::Value& value)
{
    astfri::Expr* cond   = this->resolve_expr(value["condition"]);
    astfri::Stmt* ifTrue = this->resolve_stmt(value["ifTrue"]);
    astfri::Stmt* ifFaldese
        = value["ifFalse"].IsNull() ? nullptr : this->resolve_stmt(value["ifFalse"]);

    return this->statementMaker_.mk_if(cond, ifTrue, ifFaldese);
}

astfri::SwitchStmt* AstFriDeSerializer::deserialize_switch_stmt(rapidjson::Value& value)
{
    astfri::Expr* swichEntry = this->resolve_expr(value["entry"]);
    std::vector<astfri::CaseBaseStmt*> cases;

    for (auto& caze : value["cases"].GetArray())
    {
        cases.push_back(dynamic_cast<astfri::CaseBaseStmt*>(this->resolve_stmt(caze)));
    }

    return this->statementMaker_.mk_switch(swichEntry, std::move(cases));
}

astfri::CaseStmt* AstFriDeSerializer::deserialize_case_stmt(rapidjson::Value& value)
{

    std::vector<astfri::Expr*> expressions;
    for (auto& expr : value["expressions"].GetArray())
    {
        expressions.push_back(this->resolve_expr(expr));
    }
    astfri::Stmt* body = this->resolve_stmt(value["body"]);

    return this->statementMaker_.mk_case(std::move(expressions), body);
}

astfri::WhileStmt* AstFriDeSerializer::deserialize_while_stmt(rapidjson::Value& value)
{
    astfri::Expr* condition = this->resolve_expr(value["condition"]);
    astfri::Stmt* body      = this->resolve_stmt(value["body"]);

    return this->statementMaker_.mk_while(condition, body);
}

astfri::DoWhileStmt* AstFriDeSerializer::deserialize_do_while_stmt(rapidjson::Value& value)
{
    astfri::Expr* condition = this->resolve_expr(value["condition"]);
    astfri::Stmt* body      = this->resolve_stmt(value["body"]);

    return this->statementMaker_.mk_do_while(condition, body);
}

astfri::ForStmt* AstFriDeSerializer::deserialize_for_stmt(rapidjson::Value& value)
{
    astfri::Stmt* init = value["init"].IsNull() ? nullptr : this->resolve_stmt(value["init"]);
    astfri::Expr* condition
        = value["condition"].IsNull() ? nullptr : this->resolve_expr(value["condition"]);
    astfri::Stmt* step = value["step"].IsNull() ? nullptr : this->resolve_stmt(value["step"]);
    astfri::Stmt* body = this->resolve_stmt(value["body"]);

    return this->statementMaker_.mk_for(init, condition, step, body);
}

astfri::ThrowStmt* AstFriDeSerializer::deserialize_throw_stmt(rapidjson::Value& value)
{
    return this->statementMaker_.mk_throw(this->resolve_expr(value["expression"]));
}

astfri::UnknownStmt* AstFriDeSerializer::deserialize_unknown_stmt()
{
    return this->statementMaker_.mk_uknown();
}

astfri::TranslationUnit* AstFriDeSerializer::deserialize_translation_unit(rapidjson::Value& value)
{

    std::vector<astfri::InterfaceDefStmt*> interfaces;
    for (auto& interface : value["interfaces"].GetArray())
    {
        interfaces.push_back(this->deserialize_interface_def_stmt(interface));
    }

    std::vector<astfri::ClassDefStmt*> clasdeses;
    for (auto& classDef : value["classes"].GetArray())
    {
        clasdeses.push_back(this->deserialize_class_def_stmt(classDef));
    }

    std::vector<astfri::FunctionDefStmt*> functions;
    for (auto& function : value["functions"].GetArray())
    {
        functions.push_back(this->deserialize_function_def_stmt(function));
    }

    std::vector<astfri::GlobalVarDefStmt*> globals;
    for (auto& global : value["globals"].GetArray())
    {
        globals.push_back(this->deserialize_global_var_def_stmt(global));
    }

    astfri::TranslationUnit* tu = this->statementMaker_.mk_translation_unit();
    tu->classes_                = std::move(clasdeses);
    tu->functions_              = std::move(functions);
    tu->globals_                = std::move(globals);
    tu->interfaces_             = std::move(interfaces);
    return tu;
}

astfri::ConstructorDefStmt* AstFriDeSerializer::deserialize_constructor_def_stmt(
    rapidjson::Value& value,
    astfri::ClassDefStmt* owner
)
{
    std::vector<astfri::ParamVarDefStmt*> params;
    for (auto& param : value["parameters"].GetArray())
    {
        params.push_back(this->deserialize_param_var_def_stmt(param));
    }

    std::vector<astfri::BaseInitializerStmt*> initStmts;
    for (auto& baseInitStmt : value["base_initializers"].GetArray())
    {
        initStmts.push_back(this->deserialize_base_initializer_stmt(baseInitStmt));
    }
    astfri::CompoundStmt* body = this->deserialize_compound_stmt(value["body"]);
    astfri::AccessModifier accessModifier
        = accessModMapping.find(value["access"].GetString())->second;

    return this->statementMaker_
        .mk_constructor_def(owner, std::move(params), std::move(initStmts), body, accessModifier);
}

astfri::DestructorDefStmt* AstFriDeSerializer::deserialize_destructor_def_stmt(
    rapidjson::Value& value,
    astfri::ClassDefStmt* owner
)
{

    return this->statementMaker_.mk_destructor_def(
        owner,
        this->deserialize_compound_stmt(value["body"])
    );
}

astfri::DefStmt* AstFriDeSerializer::deserialize_def_stmt(rapidjson::Value& value)
{
    std::vector<astfri::VarDefStmt*> definitions;
    for (auto& def : value["definitions"].GetArray())
    {
        //?
        definitions.push_back(dynamic_cast<astfri::VarDefStmt*>(this->resolve_stmt(def)));
    }

    return this->statementMaker_.mk_def(std::move(definitions));
}

astfri::BaseInitializerStmt* AstFriDeSerializer::deserialize_base_initializer_stmt(
    rapidjson::Value& value
)
{
    std::vector<astfri::Expr*> arguments;
    for (auto& arg : value["arguments"].GetArray())
    {
        arguments.push_back(this->resolve_expr(arg));
    }

    return this->statementMaker_.mak_base_initializer(
        value["base"].GetString(),
        std::move(arguments)
    );
}

astfri::BreakStmt* AstFriDeSerializer::deserialize_break_stmt()
{
    return this->statementMaker_.mk_break();
}

astfri::ContinueStmt* AstFriDeSerializer::deserialize_continue_stmt()
{
    return this->statementMaker_.mk_continue();
}

astfri::DefaultCaseStmt* AstFriDeSerializer::deserialize_default_case_stmt(rapidjson::Value& value)
{
    return this->statementMaker_.mk_default_case(this->resolve_stmt(value["body"]));
}

astfri::InterfaceDefStmt* AstFriDeSerializer::deserialize_interface_def_stmt(rapidjson::Value& value
)
{

    astfri::InterfaceDefStmt* interfDefStmt
        = this->statementMaker_.mk_interface_def(std::move(value["name"].GetString()));

    for (auto& base : value["bases"].GetArray())
    {
        std::string name = base.GetString();
        auto it          = nameWithInterfaceDefStmtMapping_.find(name);
        if (it == nameWithInterfaceDefStmtMapping_.end())
        {
            unresolvedInterfaceDefStmts_[name].push_back(interfDefStmt);
        }
        else
        {
            interfDefStmt->bases_.push_back(it->second);
        }
    }

    for (auto& method : value["methods"].GetArray())
    {
        interfDefStmt->methods_.push_back(this->deserialize_method_def_stmt(method, interfDefStmt));
    }

    for (auto& genericParam : value["generic_parameters"].GetArray())
    {
        interfDefStmt->tparams_.push_back(this->deserialize_generic_param(genericParam));
    }

    nameWithInterfaceDefStmtMapping_[interfDefStmt->name_] = interfDefStmt;
    return interfDefStmt;
}

void AstFriDeSerializer::resolve_class_def_stmts()
{
    // traverse throw all unresolved classDefStmts
    for (auto& it : unResolvedClassDefStmts_)
    {
        std::string name = it.first;
        // try to find name among already resolved classDefStmts
        auto itResolvedClassStmt = nameWithClassDefStmtMapping_.find(name);

        // if name isnt among resolved statements
        // create empty class def statement only
        // with name,and add it to all child classes
        if (itResolvedClassStmt == nameWithClassDefStmtMapping_.end())
        {
            astfri::ClassDefStmt* classDefStmt
                = this->statementMaker_.mk_class_def(std::move(name), astfri::mk_scope());
            for (auto& clsDefStmt : it.second)
            {
                clsDefStmt->bases_.push_back(classDefStmt);
            }
        }
        else
        {
            for (auto& clsDefStmt : it.second)
            {
                clsDefStmt->bases_.push_back(itResolvedClassStmt->second);
            }
        }
    }
}

void AstFriDeSerializer::resolve_interface_def_stmts()
{
    // traverdese throw all unresolved interfaceDefStmts
    for (auto& it : unresolvedInterfaceDefStmts_)
    {
        std::string name = it.first;

        // try to find name among already resolved interfaceDefStmts
        auto itResolvedInterfaceStmt = nameWithInterfaceDefStmtMapping_.find(name);
        if (itResolvedInterfaceStmt == nameWithInterfaceDefStmtMapping_.end())
        {
            astfri::InterfaceDefStmt* newInterfDefStmt
                = this->statementMaker_.mk_interface_def(std::move(name));
            for (auto& interfaceDefStmt : it.second)
            {
                interfaceDefStmt->bases_.push_back(newInterfDefStmt);
            }
        }
        else
        {
            astfri::InterfaceDefStmt* resolvedInterfDefStmt = itResolvedInterfaceStmt->second;
            for (auto& interfDefStmt : it.second)
            {
                interfDefStmt->bases_.push_back(resolvedInterfDefStmt);
            }
        }
    }
}

bool AstFriDeSerializer::is_class_def_stmt(astfri::Stmt* stmt)
{
    return dynamic_cast<astfri::ClassDefStmt*>(stmt) != nullptr;
}

void AstFriDeSerializer::clear_records()
{
    nameWithClassDefStmtMapping_.clear();
    nameWithInterfaceDefStmtMapping_.clear();
    unResolvedClassDefStmts_.clear();
    unresolvedInterfaceDefStmts_.clear();
}
