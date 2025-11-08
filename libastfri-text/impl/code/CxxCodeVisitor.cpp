#include <libastfri-text/inc/code/CxxCodeVisitor.hpp>

#include <libastfri-text/inc/code/CxxCodeBuilder.hpp>

using namespace astfri::text;

CxxCodeVisitor& CxxCodeVisitor::get_instance()
{
    static CxxCodeVisitor visitor;
    return visitor;
}

CxxCodeVisitor::CxxCodeVisitor() :
    AbstractCodeVisitor(&CxxCodeBuilder::get_instatnce())
{
}

void CxxCodeVisitor::visit(DynamicType const& /*type*/)
{
}

void CxxCodeVisitor::visit(IntType const& /*type*/)
{
}

void CxxCodeVisitor::visit(FloatType const& /*type*/)
{
}

void CxxCodeVisitor::visit(CharType const& /*type*/)
{
}

void CxxCodeVisitor::visit(BoolType const& /*type*/)
{
}

void CxxCodeVisitor::visit(VoidType const& /*type*/)
{
}

void CxxCodeVisitor::visit(IndirectionType const& /*type*/)
{
}

void CxxCodeVisitor::visit(ClassType const& /*type*/)
{
}

void CxxCodeVisitor::visit(InterfaceType const& /*type*/)
{
}

void CxxCodeVisitor::visit(LambdaType const& /*type*/)
{
}

void CxxCodeVisitor::visit(UnknownType const& /*type*/)
{
}

void CxxCodeVisitor::visit(IntLiteralExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(FloatLiteralExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(CharLiteralExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(StringLiteralExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(BoolLiteralExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(IfExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(BinOpExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(UnaryOpExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(ParamVarRefExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(LocalVarRefExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(MemberVarRefExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(GlobalVarRefExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(ClassRefExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(FunctionCallExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(MethodCallExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(LambdaCallExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(LambdaExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(ThisExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(ConstructorCallExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(NewExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(DeleteExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(UnknownExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(TranslationUnit const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(CompoundStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(ReturnStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(ExprStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(IfStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(CaseStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(DefaultCaseStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(SwitchStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(WhileStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(DoWhileStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(ForStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(ThrowStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(UnknownStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(LocalVarDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(ParamVarDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(MemberVarDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(GlobalVarDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(FunctionDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(DefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(MethodDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(BaseInitializerStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(ConstructorDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(DestructorDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(GenericParam const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(InterfaceDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(ClassDefStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(ContinueStmt const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(BreakStmt const& /*stmt*/)
{
}