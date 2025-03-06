#ifndef DATA_HPP
#define DATA_HPP
#include <variant>
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/Stmt.hpp>
#include <libastfri/inc/Type.hpp>
#include <unordered_map>
#include <string>
namespace astfri_serialize{


enum  ExprNodeType{
    IntLiteralExpr,FloatLiteralExpr,CharLiteralExpr,StringLiteralExpr,BoolLiteralExpr,NullLiteralExpr,IfExpr,
    BinOpExpr,UnaryOpExpr,ParamVarRefExpr,LocalVarRefExpr,MemberVarRefExpr,
    GlobalVarRefExpr,FunctionCallExpr,MethodCallExpr,LambdaExpr,ThisExpr,UnknownExpr,ConstructorCallExpr,NewExpr,
    DeleteExpr,ClassRefExpr};
    
enum StmtNodeType{    
    LocalVarDefStmt,ParamVarDefStmt,
    MemberVarDefStmt,GlobalVarDefStmt,FunctionDefStmt,MethodDefStmt,GenericParam,ClassDefStmt,CompoundStmt,ReturnStmt,
    ExprStmt,IfStmt,CaseStmt,SwitchStmt,WhileStmt,DoWhileStmt,ForStmt,ThrowStmt,UnknownStmt,TranslationUnit,
    DefStmt,ConstructorDefStmt,DestructorDefStmt,BaseInitializerStmt,BreakStmt,ContinueStmt,DefaultCaseStmt,InterfaceDefStmt};
    
    




    
inline    std::unordered_map<std::string,ExprNodeType> strToExprMapping={
    {"IntLitExpr",IntLiteralExpr},{"FloatLitExpr",FloatLiteralExpr},{"CharLitExpr",CharLiteralExpr},
    {"StringLitExpr",StringLiteralExpr},{"BoolLitExpr",BoolLiteralExpr},{"NullLitExpr",NullLiteralExpr},
    {"IfExpr",IfExpr},{"BinOpExpr",BinOpExpr},{"UnaryOpExpr",UnaryOpExpr},{"ParamVarRefExpr",ParamVarRefExpr},
    {"LocalVarRefExpr",LocalVarRefExpr},{"MemberVarRefExpr",MemberVarRefExpr},{"GlobalVarRefExpr",GlobalVarRefExpr},
    {"FunctionCallExpr",FunctionCallExpr},{"MethodCallExpr",MethodCallExpr},{"LambdaExpr",LambdaExpr},
    {"ThisExpr",ThisExpr},{"UnknownExpr",UnknownExpr},{"ConstructorCallExpr",ConstructorCallExpr},{"NewExpr",NewExpr},
    {"DeleteExpr",DeleteExpr},{"ClassRefExpr",ClassRefExpr}};
    
inline std::unordered_map<std::string,StmtNodeType> strToStmtMapping={{"LocalVarDefStmt",LocalVarDefStmt},
    {"ParamVarDefStmt",ParamVarDefStmt},{"MemberVarDefStmt",MemberVarDefStmt},{"GlobalVarDefStmt",GlobalVarDefStmt},
    {"FunctionDefStmt",FunctionDefStmt},{"MethodDefStmt",MethodDefStmt},
    {"ClassDefStmt",ClassDefStmt},{"CompoundStmt",CompoundStmt},{"ReturnStmt",ReturnStmt},{"ExpressionStmt",ExprStmt},
    {"IfStmt",IfStmt},{"CaseStmt",CaseStmt},{"SwitchStmt",SwitchStmt},{"WhileStmt",WhileStmt},
    {"DoWhileStmt",DoWhileStmt},{"ForStmt",ForStmt},{"ThrowStmt",ThrowStmt},{"UnknownStmt",UnknownStmt},
    {"TranslationUnit",TranslationUnit},{"GenericParam",GenericParam},{"DefStmt",DefStmt},
    {"ConstructorDefStmt",ConstructorDefStmt},{"DestructorDefStmt",DestructorDefStmt},
    {"BaseInitializerStmt",BaseInitializerStmt},{"BreakStmt",BreakStmt},{"ContinueStmt",ContinueStmt},
    {"DefaultCaseStmt",DefaultCaseStmt},{"InterfaceDefStmt",InterfaceDefStmt}};


inline std::unordered_map<std::string,astfri::BinOpType> binOpTypeMapping={{"=",astfri::BinOpType::Equal},{"+",astfri::BinOpType::Add},
{"-",astfri::BinOpType::Subtract},{"*",astfri::BinOpType::Multiply},{"/",astfri::BinOpType::Divide},{"%",astfri::BinOpType::Modulo},
{"==",astfri::BinOpType::Equal},{"!=",astfri::BinOpType::NotEqual},{"<",astfri::BinOpType::Less},{"<=",astfri::BinOpType::LessEqual},
{">",astfri::BinOpType::Greater},{">=",astfri::BinOpType::GreaterEqual},{">>",astfri::BinOpType::BitShiftRight},
{"<<",astfri::BinOpType::BitShiftLeft},{"&",astfri::BinOpType::BitAnd},{"|",astfri::BinOpType::BitOr},{"^",astfri::BinOpType::BitXor},
{",",astfri::BinOpType::Comma},{"&&",astfri::BinOpType::LogicalAnd},{"||",astfri::BinOpType::LogicalOr}};


//if it s post increment/decrement operator is solved in serialize_unary_expr-there is property to recognise,which says if its
//post or pre increment/decrement type 
inline std::unordered_map<std::string,astfri::UnaryOpType> unaryOpTypeMapping={{"-",astfri::UnaryOpType::Minus},{"+",astfri::UnaryOpType::Plus},
{"*",astfri::UnaryOpType::Dereference},{"&",astfri::UnaryOpType::AddressOf},{"!",astfri::UnaryOpType::LogicalNot},{"~",astfri::UnaryOpType::BitFlip},
{"++",astfri::UnaryOpType::PreIncrement},{"--",astfri::UnaryOpType::PreDecrement}};

enum Types{IntType,FloatType,CharType,BoolType,VoidType,UserType,IndirectionType,UnknownType,DynamicType};

inline std::unordered_map<std::string,Types> strToTypeMapping={{"Int",IntType},{"Float",FloatType},
    {"Char",CharType},{"Bool",BoolType},{"Void",VoidType},{"User",UserType},
    {"Indirection",IndirectionType},{"Unknown",UnknownType},{"Dynamic",DynamicType}};

}

inline std::unordered_map<std::string,astfri::AccessModifier> accessModMapping = {{"private",astfri::AccessModifier::Private},
{"public",astfri::AccessModifier::Public},{"protected",astfri::AccessModifier::Protected},{"internal",astfri::AccessModifier::Internal}};



#endif
