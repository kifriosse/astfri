#include <libastfri-text/inc/code/JavaCodeVisitor.hpp>

#include <libastfri-text/inc/code/JavaCodeBuilder.hpp>

using namespace astfri::text;

JavaCodeVisitor& JavaCodeVisitor::get_instance()
{
    static JavaCodeVisitor visitor;
    return visitor;
}

JavaCodeVisitor::JavaCodeVisitor() :
    AbstractCodeVisitor(&JavaCodeBuilder::get_instance())
{
}

void JavaCodeVisitor::visit(DynamicType const& /*type*/)
{
}

void JavaCodeVisitor::visit(IntType const& /*type*/)
{
}

void JavaCodeVisitor::visit(FloatType const& /*type*/)
{
}

void JavaCodeVisitor::visit(CharType const& /*type*/)
{
}

void JavaCodeVisitor::visit(BoolType const& /*type*/)
{
}

void JavaCodeVisitor::visit(VoidType const& /*type*/)
{
}

void JavaCodeVisitor::visit(IndirectionType const& /*type*/)
{
}

void JavaCodeVisitor::visit(ClassType const& /*type*/)
{
}

void JavaCodeVisitor::visit(InterfaceType const& /*type*/)
{
}

void JavaCodeVisitor::visit(LambdaType const& /*type*/)
{
}

void JavaCodeVisitor::visit(UnknownType const& /*type*/)
{
}

void JavaCodeVisitor::visit(IntLiteralExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(FloatLiteralExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(CharLiteralExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(StringLiteralExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(BoolLiteralExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(IfExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(BinOpExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(UnaryOpExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(ParamVarRefExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(LocalVarRefExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(MemberVarRefExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(GlobalVarRefExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(ClassRefExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(FunctionCallExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(MethodCallExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(LambdaCallExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(LambdaExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(ThisExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(ConstructorCallExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(NewExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(DeleteExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(UnknownExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(TranslationUnit const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(CompoundStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(ReturnStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(ExprStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(IfStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(CaseStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(DefaultCaseStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(SwitchStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(WhileStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(DoWhileStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(ForStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(ThrowStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(UnknownStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(LocalVarDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(ParamVarDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(MemberVarDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(GlobalVarDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(FunctionDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(DefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(MethodDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(BaseInitializerStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(ConstructorDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(DestructorDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(GenericParam const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(InterfaceDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(ClassDefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(ContinueStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(BreakStmt const& /*stmt*/)
{
}