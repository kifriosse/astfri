#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

using namespace astfri::text;

AbstractCodeVisitor::AbstractCodeVisitor(AbstractCodeBuilder* const& builder) :
    AbstractVisitor(builder)
{
}

void AbstractCodeVisitor::visit(IntType const& /*type*/)
{
}

void AbstractCodeVisitor::visit(CharType const& /*type*/)
{
}

void AbstractCodeVisitor::visit(VoidType const& /*type*/)
{
}

void AbstractCodeVisitor::visit(ClassType const& /*type*/)
{
}

void AbstractCodeVisitor::visit(InterfaceType const& /*type*/)
{
}

void AbstractCodeVisitor::visit(UnknownType const& /*type*/)
{
}

void AbstractCodeVisitor::visit(IntLiteralExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(FloatLiteralExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(CharLiteralExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(StringLiteralExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(BoolLiteralExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(IfExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(BinOpExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(UnaryOpExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(ParamVarRefExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(LocalVarRefExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(GlobalVarRefExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(ClassRefExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(ThisExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(ConstructorCallExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(NewExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(UnknownExpr const& /*expr*/)
{
}

void AbstractCodeVisitor::visit(CompoundStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(ReturnStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(ExprStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(IfStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(CaseStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(DefaultCaseStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(SwitchStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(WhileStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(DoWhileStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(ForStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(ThrowStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(UnknownStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(LocalVarDefStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(ParamVarDefStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(GlobalVarDefStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(ContinueStmt const& /*stmt*/)
{
}

void AbstractCodeVisitor::visit(BreakStmt const& /*stmt*/)
{
}