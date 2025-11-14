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
    builder_->append_text("var");
}

void JavaCodeVisitor::visit(FloatType const& /*type*/)
{
    builder_->append_text("double");
}

void JavaCodeVisitor::visit(BoolType const& /*type*/)
{
    builder_->append_text("boolean");
}

void JavaCodeVisitor::visit(IndirectionType const& type)
{
    check_and_accept_pointer_w_error(type.indirect_);
}

void JavaCodeVisitor::visit(LambdaType const& type)
{
    builder_->append_text(type.m_name);
}

void JavaCodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
    builder_->append_text("null");
}

void JavaCodeVisitor::visit(MemberVarRefExpr const& expr)
{
    if (expr.owner_) {
        check_and_accept_pointer_w_error(expr.owner_);
    } else {
        builder_->append_text("this");
    }
    builder_->append_text(".");
    builder_->append_text(expr.member_);
}

void JavaCodeVisitor::visit(FunctionCallExpr const& expr)
{
    builder_->append_text("this.");
    builder_->append_text(expr.name_);
    write_params_or_args(expr.args_, false);
}

void JavaCodeVisitor::visit(MethodCallExpr const& expr)
{
    if (expr.owner_) {
        check_and_accept_pointer_w_error(expr.owner_);
    } else {
        builder_->append_text("this");
    }
    builder_->append_text(".");
    builder_->append_text(expr.name_);
    write_params_or_args(expr.args_, false);
}

void JavaCodeVisitor::visit(LambdaCallExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(LambdaExpr const& /*expr*/)
{
}

void JavaCodeVisitor::visit(DeleteExpr const& /*expr*/)
{
    builder_->append_text("UNSUPPORTED_OPERATION!");
}

void JavaCodeVisitor::visit(TranslationUnit const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(MemberVarDefStmt const& stmt)
{
    switch (stmt.access_) {
        case AccessModifier::Protected:
            builder_->append_text("protected ");
            break;
        case AccessModifier::Private:
            builder_->append_text("private ");
            break;
        default:
            builder_->append_text("public ");
    }
    check_and_accept_pointer_w_error(stmt.type_);
    builder_->append_text(" " + stmt.name_);
    if (stmt.initializer_) {
        builder_->append_text(" = ");
        check_and_accept_pointer_w_error(stmt.initializer_);
        builder_->append_text(";");
    }
}

void JavaCodeVisitor::visit(FunctionDefStmt const& stmt)
{
    check_and_accept_pointer_w_error(stmt.retType_);
    builder_->append_text(" " + stmt.name_);
    write_params_or_args(stmt.params_, false);
    builder_->write_opening_curl_bracket();
    check_and_accept_pointer_w_error(stmt.body_);
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void JavaCodeVisitor::visit(DefStmt const& /*stmt*/)
{
}

void JavaCodeVisitor::visit(MethodDefStmt const& stmt)
{
    switch (stmt.access_) {
        case AccessModifier::Protected:
            builder_->append_text("protected ");
            break;
        case AccessModifier::Private:
            builder_->append_text("private ");
            break;
        default:
            builder_->append_text("public ");
    }
    check_and_accept_pointer_w_error(stmt.func_);
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