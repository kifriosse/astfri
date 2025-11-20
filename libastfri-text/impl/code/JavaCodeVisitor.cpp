#include <libastfri-text/inc/code/JavaCodeVisitor.hpp>

#include <libastfri-text/inc/code/JavaCodeBuilder.hpp>

using namespace astfri::text;

JavaCodeVisitor& JavaCodeVisitor::get_instance()
{
    static JavaCodeVisitor visitor;
    return visitor;
}

JavaCodeVisitor::JavaCodeVisitor() :
    AbstractCodeVisitor(JavaCodeBuilder::get_instance())
{
}

//
// -----
//

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
    if (!try_accept_node(type.indirect_))
    {
        builder_->write_invalid_type();
    }
}

void JavaCodeVisitor::visit(LambdaType const& /*type*/)
{
    // TODO: lambda type text
    builder_->append_text("NOT IMPLEMENTED YET");
}

void JavaCodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
    builder_->append_text("null");
}

void JavaCodeVisitor::visit(MemberVarRefExpr const& expr)
{
    if (!try_accept_node(expr.owner_))
    {
        builder_->append_text("this");
    }
    builder_->append_text(".");
    builder_->append_text(expr.member_);
}

void JavaCodeVisitor::visit(GlobalVarRefExpr const& expr)
{
    builder_->append_text("UnclassifiedElements." + expr.global_);
}

void JavaCodeVisitor::visit(FunctionCallExpr const& expr)
{
    builder_->append_text("UnclassifiedElements." + expr.name_);
    process_pargs(expr.args_, false);
}

void JavaCodeVisitor::visit(MethodCallExpr const& expr)
{
    if (!try_accept_node(expr.owner_))
    {
        builder_->append_text("this");
    }
    builder_->append_text(".");
    builder_->append_text(expr.name_);
    process_pargs(expr.args_, false);
}

void JavaCodeVisitor::visit(LambdaCallExpr const& /*expr*/)
{
    // TODO: implement lambda call
    builder_->append_text("NOT IMPLEMENTED YET");
}

void JavaCodeVisitor::visit(LambdaExpr const& /*expr*/)
{
    // TODO: implement lambda expression
    builder_->append_text("NOT IMPLEMENTED YET");
}

void JavaCodeVisitor::visit(TranslationUnit const& stmt)
{
    process_unclassified(stmt.functions_, stmt.globals_);
    for (auto* const& cds : stmt.classes_)
    {
        if (!try_accept_node(cds))
        {
            builder_->write_invalid_stmt();
        }
    }
    for (auto* const& ids : stmt.interfaces_)
    {
        if (!try_accept_node(ids))
        {
            builder_->write_invalid_stmt();
        }
    }
}

void JavaCodeVisitor::visit(MemberVarDefStmt const& stmt)
{
    process_identifier(stmt.access_);
    if (!try_accept_node(stmt.type_))
    {
        builder_->write_invalid_type();
    }
    builder_->append_text(" " + stmt.name_);
    if (stmt.initializer_)
    {
        builder_->append_text(" = ");
        try_accept_node(stmt.initializer_);
    }
    builder_->append_text(";");
}

void JavaCodeVisitor::visit(GlobalVarDefStmt const& stmt)
{
    builder_->append_text("public static ");
    if (!try_accept_node(stmt.type_))
    {
        builder_->write_invalid_type();
    }
    builder_->append_text(" " + stmt.name_);
    if (stmt.initializer_)
    {
        builder_->append_text(" = ");
        try_accept_node(stmt.initializer_);
    }
    builder_->append_text(";");
}

void JavaCodeVisitor::visit(FunctionDefStmt const& stmt)
{
    if (!try_accept_node(stmt.retType_))
    {
        builder_->write_invalid_type();
    }
    builder_->append_text(" " + stmt.name_);
    process_pargs(stmt.params_, false);
    builder_->write_opening_curl_bracket();
    if (!try_accept_node(stmt.body_))
    {
        builder_->write_invalid_type();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void JavaCodeVisitor::visit(DefStmt const& stmt)
{
    if (stmt.defs_.empty())
    {
        builder_->write_invalid_stmt();
        return;
    }
    if (!try_accept_node(stmt.defs_.at(0)->type_))
    {
        builder_->write_invalid_type();
    }
    for (size_t i = 0; i < stmt.defs_.size(); ++i)
    {
        builder_->append_text(" " + stmt.defs_.at(i)->name_);
        if (stmt.defs_.at(i)->initializer_)
        {
            builder_->append_text(" = ");
            try_accept_node(stmt.defs_.at(i)->initializer_);
        }
        if (i < stmt.defs_.size() - 1)
        {
            builder_->append_text(",");
        }
    }
}

void JavaCodeVisitor::visit(MethodDefStmt const& stmt)
{
    process_identifier(stmt.access_);
    if (!try_accept_node(stmt.func_))
    {
        builder_->write_invalid_stmt();
    }
}

void JavaCodeVisitor::visit(BaseInitializerStmt const& stmt)
{
    builder_->append_text("super");
    process_pargs(stmt.args_, false);
    builder_->append_text(";");
    builder_->append_new_line();
}

void JavaCodeVisitor::visit(ConstructorDefStmt const& stmt)
{
    process_identifier(stmt.access_);
    builder_->append_text(stmt.owner_->name_);
    process_pargs(stmt.params_, false);
    builder_->write_opening_curl_bracket();
    if (!stmt.baseInit_.empty())
    {
        if (!try_accept_node(stmt.baseInit_.at(0)))
        {
            builder_->write_invalid_expr();
        }
        if (stmt.baseInit_.size() > 1)
        {
            builder_->append_text("ERROR - ONLY ONE SUPER CALL POSSIBLE");
            builder_->append_new_line();
        }
    }
    if (!try_accept_node(stmt.body_))
    {
        builder_->write_invalid_stmt();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void JavaCodeVisitor::visit(GenericParam const& stmt)
{
    builder_->append_text(stmt.name_);
    if (stmt.constraint_ != "")
    {
        builder_->append_text(" extends " + stmt.constraint_);
    }
}

void JavaCodeVisitor::visit(InterfaceDefStmt const& stmt)
{
    Scope const& scope = stmt.m_type->scope_;
    process_package(scope);
    builder_->append_text("public interface " + stmt.name_);
    if (!stmt.tparams_.empty())
    {
        process_pargs(stmt.tparams_, true);
    }
    builder_->append_space();
    process_supertypes(stmt.bases_, true);
    builder_->append_text("{");
    builder_->append_new_line();
    builder_->append_new_line();
    builder_->increase_indentation();
    for (size_t i = 0; i < stmt.methods_.size(); ++i)
    {
        if (!try_accept_node(stmt.methods_.at(i)->func_->retType_))
        {
            builder_->write_invalid_type();
        }
        builder_->append_space();
        builder_->append_text(stmt.methods_.at(i)->func_->name_);
        process_pargs(stmt.methods_.at(i)->func_->params_, false);
        builder_->append_text(";");
        builder_->append_new_line();
        if (i < stmt.methods_.size() - 1)
        {
            builder_->append_new_line();
        }
    }
    builder_->decrease_indentation();
    builder_->append_text("}");
    static_cast<JavaCodeBuilder*>(builder_)->create_java_file(stmt.name_, scope);
}

void JavaCodeVisitor::visit(ClassDefStmt const& stmt)
{
    Scope const& scope = stmt.type_->scope_;
    process_package(scope);
    builder_->append_text("public class " + stmt.name_);
    if (!stmt.tparams_.empty())
    {
        process_pargs(stmt.tparams_, true);
    }
    builder_->append_space();
    process_supertypes(stmt.bases_, true);
    process_supertypes(stmt.interfaces_, false);
    builder_->append_text("{");
    builder_->append_new_line();
    builder_->append_new_line();
    builder_->increase_indentation();
    if (!stmt.vars_.empty())
    {
        for (size_t i = 0; i < stmt.vars_.size(); ++i)
        {
            if (!try_accept_node(stmt.vars_.at(i)))
            {
                builder_->write_invalid_expr();
            }
            builder_->append_new_line();
        }
        builder_->append_new_line();
    }
    bool tmp = false;
    if (!stmt.constructors_.empty())
    {
        tmp = true;
        for (size_t i = 0; i < stmt.constructors_.size(); ++i)
        {
            if (!try_accept_node(stmt.constructors_.at(i)))
            {
                builder_->write_invalid_stmt();
            }
            builder_->append_new_line();
            if (i < stmt.constructors_.size() - 1)
            {
                builder_->append_new_line();
            }
        }
    }
    if (!stmt.methods_.empty())
    {
        if (tmp)
        {
            builder_->append_new_line();
        }
        for (size_t i = 0; i < stmt.methods_.size(); ++i)
        {
            if (!try_accept_node(stmt.methods_.at(i)))
            {
                builder_->write_invalid_stmt();
            }
            builder_->append_new_line();
            if (i < stmt.methods_.size() - 1)
            {
                builder_->append_new_line();
            }
        }
    }
    builder_->decrease_indentation();
    builder_->append_text("}");
    static_cast<JavaCodeBuilder*>(builder_)->create_java_file(stmt.name_, scope);
}

// --------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------- //

void JavaCodeVisitor::process_unclassified(
    std::vector<FunctionDefStmt*> const& functions,
    std::vector<GlobalVarDefStmt*> const& vars)
{
    builder_->append_text("public class UnclassifiedElements");
    builder_->write_opening_curl_bracket();
    builder_->append_new_line();
    bool tmp = false;
    if (!vars.empty())
    {
        tmp = true;
        for (size_t i = 0; i < vars.size(); ++i)
        {
            if (!try_accept_node(vars.at(i)))
            {
                builder_->write_invalid_expr();
            }
            builder_->append_new_line();
        }
    }
    if (!functions.empty())
    {
        if (tmp)
        {
            builder_->append_new_line();
        }
        for (size_t i = 0; i < functions.size(); ++i)
        {
            builder_->append_text("public static ");
            if (!try_accept_node(functions.at(i)))
            {
                builder_->write_invalid_stmt();
            }
            builder_->append_new_line();
            if (i < functions.size() - 1)
            {
                builder_->append_new_line();
            }
        }
    }
    builder_->decrease_indentation();
    builder_->append_text("}");
    static_cast<JavaCodeBuilder*>(builder_)->create_java_file("UnclassifiedElements");
}

void JavaCodeVisitor::process_package(Scope const& scope)
{
    if (scope.names_.empty())
    {
        return;
    }
    builder_->append_text("package ");
    for (size_t i = 0; i < scope.names_.size(); ++i)
    {
        builder_->append_text(scope.names_.at(i));
        if (i < scope.names_.size() - 1)
        {
            builder_->append_text(".");
        }
    }
    builder_->append_text(";");
    builder_->append_new_line();
    builder_->append_new_line();
}

void JavaCodeVisitor::process_identifier(AccessModifier const& access)
{
    switch (access)
    {
        case AccessModifier::Public:
            builder_->append_text("public ");
            break;
        case AccessModifier::Protected:
            builder_->append_text("protected ");
            break;
        case AccessModifier::Private:
            builder_->append_text("private ");
            break;
        default:
            break;
    }
}