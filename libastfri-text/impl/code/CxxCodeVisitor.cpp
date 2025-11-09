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

void CxxCodeVisitor::visit(FloatType const& /*type*/)
{
}

void CxxCodeVisitor::visit(BoolType const& /*type*/)
{
}

void CxxCodeVisitor::visit(IndirectionType const& /*type*/)
{
}

void CxxCodeVisitor::visit(LambdaType const& /*type*/)
{
}

void CxxCodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(MemberVarRefExpr const& /*expr*/)
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

void CxxCodeVisitor::visit(DeleteExpr const& /*expr*/)
{
}

void CxxCodeVisitor::visit(TranslationUnit const& /*stmt*/)
{
}

void CxxCodeVisitor::visit(MemberVarDefStmt const& /*stmt*/)
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