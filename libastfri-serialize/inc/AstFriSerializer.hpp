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
    static AstFriSerializer& getInstance();
    astfri::IVisitable* serialize(std::string filePath);

private:

AstFriSerializer();
astfri::ExprFactory& expressionMaker_;
astfri::StmtFactory& statementMaker_;
astfri::TypeFactory& typeMaker_ ;
JsonFormatChecker& jsonChecker_;



astfri::IntLiteralExpr* serializeIntLiteralExpr(rapidjson::Value& value);
astfri::FloatLiteralExpr* serializeFloatLiteralExpr(rapidjson::Value& value);
astfri::CharLiteralExpr* serializeCharLiteralExpr(rapidjson::Value& value);
astfri::StringLiteralExpr* serializeStringLiteralExpr(rapidjson::Value& value);
astfri::BoolLiteralExpr* serializeBoolLiteralExpr(rapidjson::Value& value);
astfri::NullLiteralExpr* serializeNullLiteralExpr(rapidjson::Value& value);
astfri::IfExpr* serializeIfExpr(rapidjson::Value& value);
astfri::BinOpExpr* serializeBinOpExpr(rapidjson::Value& value);
astfri::UnaryOpExpr* serializeUnaryOpExpr(rapidjson::Value& value);
astfri::AssignExpr* serializeAssignExpr(rapidjson::Value& value);
astfri::CompoundAssignExpr* serializeCompoundAssignExpr(rapidjson::Value& value);
astfri::ParamVarRefExpr* serializeParamVarRefExpr(rapidjson::Value& value);
astfri::LocalVarRefExpr* serializeLocalVarRefExpr(rapidjson::Value& value);
astfri::MemberVarRefExpr* serializeMemberVarRefExpr(rapidjson::Value& value);
astfri::GlobalVarRefExpr* serializeGlobalVarRefExpr(rapidjson::Value& value);
astfri::FunctionCallExpr* serializeFunctionCallExpr(rapidjson::Value& value);
astfri::MethodCallExpr* serializeMethodCallExpr(rapidjson::Value& value);
astfri::LambdaExpr* serializeLambdaExpr(rapidjson::Value& value);
astfri::ThisExpr* serializeThisExpr();
astfri::UnknownExpr* serializeUnknownExpr();
astfri::LocalVarDefStmt* serializeLocalVarDefStmt(rapidjson::Value& value);
astfri::ParamVarDefStmt* serializeParamVarDefStmt(rapidjson::Value& value);
astfri::MemberVarDefStmt* serializeMemberVarDefStmt(rapidjson::Value& value);
astfri::GlobalVarDefStmt* serializeGlobalVarDefStmt(rapidjson::Value& value);
astfri::FunctionDefStmt* serializeFunctionDefStmt(rapidjson::Value& value);
astfri::MethodDefStmt* serializeMethodDefStmt(rapidjson::Value& value);
astfri::GenericParam* serializeGenericParam(rapidjson::Value& value);
astfri::ClassDefStmt* serializeClassDefStmt(rapidjson::Value& value);
astfri::CompoundStmt* serializeCompoundStmt(rapidjson::Value& value);
astfri::ReturnStmt* serializeReturnStmt(rapidjson::Value& value);
astfri::ExprStmt* serializeExprStmt(rapidjson::Value& value);
astfri::IfStmt* serializeIfStmt(rapidjson::Value& value);
astfri::CaseStmt* serializeCaseStmt(rapidjson::Value& value);
astfri::SwitchStmt* serializeSwitchStmt(rapidjson::Value& value);
astfri::WhileStmt* serializeWhileStmt(rapidjson::Value& value);
astfri::DoWhileStmt* serializeDoWhileStmt(rapidjson::Value& value);
astfri::ForStmt* serializeForStmt(rapidjson::Value& value);
astfri::ThrowStmt* serializeThrowStmt(rapidjson::Value& value);
astfri::UnknownStmt* serializeUnknownStmt(rapidjson::Value& value);
astfri::TranslationUnit* serializeTranslationUnit(rapidjson::Value& value);
astfri::Type* serializeType(rapidjson::Value& value);

void checkJsonFormatLitExpressions(rapidjson::Value& jsonVal);


astfri::Expr* resolveExpr(rapidjson::Value& value);
astfri::Stmt* resolveStmt(rapidjson::Value& value);
astfri::Type* resolveType(rapidjson::Value& value);
};
#endif
