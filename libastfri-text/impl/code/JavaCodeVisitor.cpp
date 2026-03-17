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
    if (!try_accept_node(type.indirect))
    {
        builder_->write_invalid_type();
    }
}

void JavaCodeVisitor::visit(LambdaType const& type)
{
    builder_->append_text(type.name);
}

// -----

void JavaCodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
    builder_->append_text("null");
}

void JavaCodeVisitor::visit(MemberVarRefExpr const& expr)
{
    if (!try_accept_node(expr.owner))
    {
        builder_->append_text("this");
    }
    builder_->append_text(".");
    builder_->append_text(expr.member);
}

void JavaCodeVisitor::visit(GlobalVarRefExpr const& expr)
{
    builder_->append_text("UnclassifiedElements." + expr.global);
}

void JavaCodeVisitor::visit(FunctionCallExpr const& expr)
{
    builder_->append_text("UnclassifiedElements." + expr.name);
    process_pargs(expr.args, false);
}

void JavaCodeVisitor::visit(MethodCallExpr const& expr)
{
    if (!try_accept_node(expr.owner))
    {
        builder_->append_text("this");
    }
    builder_->append_text(".");
    builder_->append_text(expr.name);
    process_pargs(expr.args, false);
}

void JavaCodeVisitor::visit(LambdaCallExpr const& expr)
{
    process_pargs(expr.args, false);
    builder_->append_text(" ->");
    builder_->write_opening_curl_bracket();
    if (!try_accept_node(expr.lambda))
    {
        builder_->write_invalid_expr();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->write_right_bracket("}");
}

// -----

void JavaCodeVisitor::visit(TranslationUnit const& stmt)
{
    process_unclassified(stmt.functions, stmt.globals);
    for (auto* const& cds : stmt.classes)
    {
        if (!try_accept_node(cds))
        {
            builder_->write_invalid_stmt();
        }
    }
    for (auto* const& ids : stmt.interfaces)
    {
        if (!try_accept_node(ids))
        {
            builder_->write_invalid_stmt();
        }
    }
}

void JavaCodeVisitor::visit(MemberVarDefStmt const& stmt)
{
    process_access_mod(stmt.access);
    if (!try_accept_node(stmt.type))
    {
        builder_->write_invalid_type();
    }
    builder_->append_text(" " + stmt.name);
    if (stmt.initializer)
    {
        builder_->append_text(" = ");
        try_accept_node(stmt.initializer);
    }
    builder_->append_text(";");
}

void JavaCodeVisitor::visit(GlobalVarDefStmt const& stmt)
{
    builder_->append_text("public static ");
    if (!try_accept_node(stmt.type))
    {
        builder_->write_invalid_type();
    }
    builder_->append_text(" " + stmt.name);
    if (stmt.initializer)
    {
        builder_->append_text(" = ");
        try_accept_node(stmt.initializer);
    }
    builder_->append_text(";");
}

void JavaCodeVisitor::visit(FunctionDefStmt const& stmt)
{
    if (!try_accept_node(stmt.retType))
    {
        builder_->write_invalid_type();
    }
    builder_->append_text(" " + stmt.name);
    process_pargs(stmt.params, false);
    builder_->write_opening_curl_bracket();
    if (!try_accept_node(stmt.body))
    {
        builder_->write_invalid_type();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void JavaCodeVisitor::visit(DefStmt const& stmt)
{
    if (stmt.defs.empty())
    {
        builder_->write_invalid_stmt();
        return;
    }
    if (!try_accept_node(stmt.defs.at(0)->type))
    {
        builder_->write_invalid_type();
    }
    for (size_t i = 0; i < stmt.defs.size(); ++i)
    {
        builder_->append_text(" " + stmt.defs.at(i)->name);
        if (stmt.defs.at(i)->initializer)
        {
            builder_->append_text(" = ");
            try_accept_node(stmt.defs.at(i)->initializer);
        }
        if (i < stmt.defs.size() - 1)
        {
            builder_->append_text(",");
        }
    }
}

void JavaCodeVisitor::visit(MethodDefStmt const& stmt)
{
    process_access_mod(stmt.access);
    if (!try_accept_node(stmt.func))
    {
        builder_->write_invalid_stmt();
    }
}

void JavaCodeVisitor::visit(BaseInitializerStmt const& stmt)
{
    builder_->append_text("super");
    process_pargs(stmt.args, false);
    builder_->append_text(";");
    builder_->append_new_line();
}

void JavaCodeVisitor::visit(ConstructorDefStmt const& stmt)
{
    process_access_mod(stmt.access);
    //builder_->append_text(stmt.owner_->name_);
    builder_->append_text(stmt.owner->type->name); // TODO check
    process_pargs(stmt.params, false);
    builder_->write_opening_curl_bracket();
    if (!stmt.baseInit.empty())
    {
        if (!try_accept_node(stmt.baseInit.at(0)))
        {
            builder_->write_invalid_expr();
        }
        if (stmt.baseInit.size() > 1)
        {
            builder_->append_text("ERROR - ONLY ONE SUPER CALL POSSIBLE");
            builder_->append_new_line();
        }
    }
    if (!try_accept_node(stmt.body))
    {
        builder_->write_invalid_stmt();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void JavaCodeVisitor::visit(GenericParam const& stmt)
{
    builder_->append_text(stmt.name);
    if (stmt.constraint != "")
    {
        builder_->append_text(" extends " + stmt.constraint);
    }
}

void JavaCodeVisitor::visit(InterfaceDefStmt const& stmt)
{
    Scope const& scope = stmt.type->scope;
    process_package(scope);
    //builder_->append_text("public interface " + stmt.name_);
    builder_->append_text("public interface " + stmt.type->name); // TODO check
    if (!stmt.tparams.empty())
    {
        process_pargs(stmt.tparams, true);
    }
    builder_->append_space();
    process_supertypes(stmt.bases, true, false);
    builder_->append_text("{");
    builder_->append_new_line();
    builder_->append_new_line();
    builder_->increase_indentation();
    for (size_t i = 0; i < stmt.methods.size(); ++i)
    {
        if (!try_accept_node(stmt.methods.at(i)->func->retType))
        {
            builder_->write_invalid_type();
        }
        builder_->append_space();
        builder_->append_text(stmt.methods.at(i)->func->name);
        process_pargs(stmt.methods.at(i)->func->params, false);
        builder_->append_text(";");
        builder_->append_new_line();
        if (i < stmt.methods.size() - 1)
        {
            builder_->append_new_line();
        }
    }
    builder_->decrease_indentation();
    builder_->append_text("}");
    //static_cast<JavaCodeBuilder*>(builder_)->create_java_file(stmt.name_, scope);
    static_cast<JavaCodeBuilder*>(builder_)->create_java_file(stmt.type->name, scope); // TODO check
}

void JavaCodeVisitor::visit(ClassDefStmt const& stmt)
{
    Scope const& scope = stmt.type->scope;
    process_package(scope);
    //builder_->append_text("public class " + stmt.name_);
    builder_->append_text("public class " + stmt.type->name); // TODO check
    if (!stmt.tparams.empty())
    {
        process_pargs(stmt.tparams, true);
    }
    builder_->append_space();
    process_supertypes(stmt.bases, true, true);
    process_supertypes(stmt.interfaces, false, false);
    builder_->append_text("{");
    builder_->append_new_line();
    builder_->append_new_line();
    builder_->increase_indentation();
    if (!stmt.vars.empty())
    {
        for (size_t i = 0; i < stmt.vars.size(); ++i)
        {
            if (!try_accept_node(stmt.vars.at(i)))
            {
                builder_->write_invalid_expr();
            }
            builder_->append_new_line();
        }
        builder_->append_new_line();
    }
    bool tmp = false;
    if (!stmt.constructors.empty())
    {
        tmp = true;
        for (size_t i = 0; i < stmt.constructors.size(); ++i)
        {
            if (!try_accept_node(stmt.constructors.at(i)))
            {
                builder_->write_invalid_stmt();
            }
            builder_->append_new_line();
            if (i < stmt.constructors.size() - 1)
            {
                builder_->append_new_line();
            }
        }
    }
    if (!stmt.methods.empty())
    {
        if (tmp)
        {
            builder_->append_new_line();
        }
        for (size_t i = 0; i < stmt.methods.size(); ++i)
        {
            if (!try_accept_node(stmt.methods.at(i)))
            {
                builder_->write_invalid_stmt();
            }
            builder_->append_new_line();
            if (i < stmt.methods.size() - 1)
            {
                builder_->append_new_line();
            }
        }
    }
    builder_->decrease_indentation();
    builder_->append_text("}");
    //static_cast<JavaCodeBuilder*>(builder_)->create_java_file(stmt.name_, scope);
    static_cast<JavaCodeBuilder*>(builder_)->create_java_file(stmt.type->name, scope); // TODO check
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
    if (scope.names.empty())
    {
        return;
    }
    builder_->append_text("package ");
    for (size_t i = 0; i < scope.names.size(); ++i)
    {
        builder_->append_text(scope.names.at(i));
        if (i < scope.names.size() - 1)
        {
            builder_->append_text(".");
        }
    }
    builder_->append_text(";");
    builder_->append_new_line();
    builder_->append_new_line();
}

void JavaCodeVisitor::process_access_mod(AccessModifier const& mod)
{
    switch (mod)
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