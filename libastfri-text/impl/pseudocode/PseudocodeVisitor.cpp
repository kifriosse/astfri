#include <libastfri-text/inc/pseudocode/PseudocodeVisitor.hpp>

#include <libastfri-text/inc/pseudocode/PlainTextBuilder.hpp>
#include <libastfri-text/inc/pseudocode/HtmlTextBuilder.hpp>

using namespace astfri::text;

PseudocodeVisitor& PseudocodeVisitor::get_instance()
{
    AbstractTextBuilder* builder;
    TextConfigurator& tc = TextConfigurator::get_instance();
    if (tc.output_file_format()->view() == "html")
    {
        builder = &HtmlTextBuilder::get_instance();
    }
    else
    {
        builder = &PlainTextBuilder::get_instance();
    }
    builder->reset_builder();
    static PseudocodeVisitor visitor(*builder);
    return visitor;
}

PseudocodeVisitor::PseudocodeVisitor(AbstractTextBuilder& builder) :
    AbstractVisitor(builder),
    configurator_(&TextConfigurator::get_instance())
{
}

//
// -----
//

void PseudocodeVisitor::export_pseudocode()
{
    static_cast<AbstractTextBuilder*>(builder_)->export_text();
}

void PseudocodeVisitor::clear_builder()
{
    builder_->reset_builder();
}

void PseudocodeVisitor::append_text(std::string const& text)
{
    builder_->append_text(text);
}

void PseudocodeVisitor::append_new_line()
{
    builder_->append_new_line();
}

void PseudocodeVisitor::append_space()
{
    builder_->append_space();
}

void PseudocodeVisitor::update_configuration()
{
    configurator_->update_configuration();
    if (configurator_->output_file_format()->view() == "html")
    {
        builder_ = &HtmlTextBuilder::get_instance();
    }
    else
    {
        builder_ = &PlainTextBuilder::get_instance();
    }
    builder_->reset_builder();
}

void PseudocodeVisitor::reload_configuration()
{
    configurator_->reload_configuration();
    if (configurator_->output_file_format()->view() == "html")
    {
        builder_ = &HtmlTextBuilder::get_instance();
    }
    else
    {
        builder_ = &PlainTextBuilder::get_instance();
    }
    builder_->reset_builder();
}

// -----

void PseudocodeVisitor::visit(DynamicType const& /*type*/)
{
}

void PseudocodeVisitor::visit(IntType const& /*type*/)
{
}

void PseudocodeVisitor::visit(FloatType const& /*type*/)
{
}

void PseudocodeVisitor::visit(CharType const& /*type*/)
{
}

void PseudocodeVisitor::visit(BoolType const& /*type*/)
{
}

void PseudocodeVisitor::visit(VoidType const& /*type*/)
{
}

void PseudocodeVisitor::visit(IndirectionType const& /*type*/)
{
}

void PseudocodeVisitor::visit(ClassType const& /*type*/)
{
}

void PseudocodeVisitor::visit(InterfaceType const& /*type*/)
{
}

void PseudocodeVisitor::visit(LambdaType const& /*type*/)
{
}

void PseudocodeVisitor::visit(IncompleteType const& /*type*/)
{
}

void PseudocodeVisitor::visit(UnknownType const& /*type*/)
{
}

// -----

void PseudocodeVisitor::visit(IntLiteralExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(FloatLiteralExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(CharLiteralExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(StringLiteralExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(BoolLiteralExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(IfExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(BinOpExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(UnaryOpExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(ParamVarRefExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(LocalVarRefExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(MemberVarRefExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(GlobalVarRefExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(ClassRefExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(FunctionCallExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(MethodCallExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(LambdaCallExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(LambdaExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(ThisExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(ConstructorCallExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(NewExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(DeleteExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(BracketExpr const& /*expr*/)
{
}

void PseudocodeVisitor::visit(UnknownExpr const& /*expr*/)
{
}

// -----

void PseudocodeVisitor::visit(TranslationUnit const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(CompoundStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(ReturnStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(ExprStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(IfStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(CaseStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(DefaultCaseStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(SwitchStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(WhileStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(DoWhileStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(ForStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(ThrowStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(UnknownStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(LocalVarDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(ParamVarDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(MemberVarDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(GlobalVarDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(FunctionDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(DefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(MethodDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(BaseInitializerStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(ConstructorDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(DestructorDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(GenericParam const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(InterfaceDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(ClassDefStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(ContinueStmt const& /*stmt*/)
{
}

void PseudocodeVisitor::visit(BreakStmt const& /*stmt*/)
{
}