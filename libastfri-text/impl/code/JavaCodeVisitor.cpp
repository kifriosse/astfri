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

void JavaCodeVisitor::visit(FloatType const& /*type*/)
{
}

void JavaCodeVisitor::visit(BoolType const& /*type*/)
{
}

void JavaCodeVisitor::visit(IndirectionType const& /*type*/)
{
}

void JavaCodeVisitor::visit(LambdaType const& /*type*/)
{
}

void JavaCodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(MemberVarRefExpr const& /*expr*/)
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

void JavaCodeVisitor::visit(DeleteExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(TranslationUnit const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(MemberVarDefStmt const& /*stmt*/)
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