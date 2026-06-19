#include <astfri-text/impl/PseudocodeVisitor.hpp>

using namespace astfri::text;

constexpr int LOCAL_VAR  = 0;
constexpr int PARAM_VAR  = 1;
constexpr int MEMBER_VAR = 2;
constexpr int GLOBAL_VAR = 3;

void PseudocodeVisitor::visit(const TranslationUnit& stmt)
{
    bool previous = false;
    if (m_config.shGlobVarDeclar)
    {
        for (GlobalVarDefStmt* a : stmt.globals)
        {
            if (a)
            {
                a->accept(*this);
                m_builder.write_new_line();
                previous = true;
            }
        }
    }
    if (m_config.shFuncDeclar)
    {
        for (FunctionDefStmt* a : stmt.functions)
        {
            if (a)
            {
                if (previous)
                {
                    m_builder.write_new_line();
                }
                a->accept(*this);
                m_builder.write_new_line();
                previous = true;
            }
        }
    }
    if (m_config.shInterfDeclar)
    {
        for (InterfaceDefStmt* a : stmt.interfaces)
        {
            if (a)
            {
                if (previous)
                {
                    m_builder.write_new_line();
                }
                a->accept(*this);
                m_builder.write_new_line();
                previous = true;
            }
        }
    }
    for (ClassDefStmt* a : stmt.classes)
    {
        if (a)
        {
            if (previous)
            {
                m_builder.write_new_line();
            }
            a->accept(*this);
            m_builder.write_new_line();
            previous = true;
        }
    }
}

void PseudocodeVisitor::visit(const CompoundStmt& stmt)
{
    for (size_t i = 0; i < stmt.stmts.size(); ++i)
    {
        accept_node(stmt.stmts.at(i));
        if (i < stmt.stmts.size() - 1)
        {
            m_builder.write_new_line();
        }
    }
}

void PseudocodeVisitor::visit(const ReturnStmt& stmt)
{
    m_builder.write_return_word();
    if (stmt.val)
    {
        m_builder.write_space();
        accept_node(stmt.val);
    }
}

void PseudocodeVisitor::visit(const ExprStmt& stmt)
{
    accept_node(stmt.expr);
}

void PseudocodeVisitor::visit(const IfStmt& stmt)
{
    m_builder.write_if_word();
    process_condition(stmt.cond);
    m_builder.write_space();
    process_body(stmt.iftrue, m_config.conditionBlockBracketNewLine);
    if (stmt.iffalse)
    {
        m_builder.write_opening_else_word();
        process_body(stmt.iffalse, m_config.conditionBlockBracketNewLine);
    }
}

void PseudocodeVisitor::visit(const CaseStmt& stmt)
{
    m_builder.write_case_word();
    m_builder.write_space();
    for (size_t i = 0; i < stmt.exprs.size(); ++i)
    {
        accept_node(stmt.exprs.at(i));
        if (i < stmt.exprs.size() - 1)
        {
            m_builder.write_comma_space();
        }
    }
    m_builder.write_space();
    m_builder.write_do_word();
    process_body(stmt.body, m_config.switchBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const DefaultCaseStmt& stmt)
{
    m_builder.write_default_word();
    m_builder.write_space();
    m_builder.write_do_word();
    process_body(stmt.body, m_config.switchBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const SwitchStmt& stmt)
{
    m_builder.write_switch_word();
    process_condition(stmt.expr);
    m_builder.write_opening_curl_bracket(m_config.switchBlockBracketNewLine);
    m_builder.increase_indentation();
    for (size_t i = 0; i < stmt.cases.size(); ++i)
    {
        accept_node(stmt.cases.at(i));
        if (i < stmt.cases.size() - 1)
        {
            m_builder.write_new_line();
        }
    }
    m_builder.decrease_indentation();
    m_builder.write_new_line();
    m_builder.write_right_bracket("}");
}

void PseudocodeVisitor::visit(const WhileStmt& stmt)
{
    m_builder.write_while_word();
    process_condition(stmt.cond);
    m_builder.write_space();
    m_builder.write_repeat_word();
    process_body(stmt.body, m_config.loopBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const DoWhileStmt& stmt)
{
    m_builder.write_repeat_word();
    process_body(stmt.body, m_config.loopBlockBracketNewLine);
    if (m_config.dowhileConditionNewLine)
    {
        m_builder.write_new_line();
    }
    else
    {
        m_builder.write_space();
    }
    m_builder.write_while_word();
    process_condition(stmt.cond);
}

void PseudocodeVisitor::visit(const ForStmt& stmt)
{
    m_builder.write_for_word();
    m_builder.write_space();
    m_builder.write_left_bracket("(");
    accept_node(stmt.init);
    m_builder.write_separator(";");
    m_builder.write_space();
    accept_node(stmt.cond);
    m_builder.write_separator(";");
    m_builder.write_space();
    accept_node(stmt.step);
    m_builder.write_right_bracket(")");
    m_builder.write_space();
    m_builder.write_repeat_word();
    process_body(stmt.body, m_config.loopBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const ForEachStmt& stmt)
{
    m_builder.write_foreach_word();
    m_builder.write_space();
    m_builder.write_left_bracket("(");
    accept_node(stmt.var);
    m_builder.write_text(" from container "); // TODO: create name from config
    accept_node(stmt.container);
    m_builder.write_right_bracket(")");
    m_builder.write_space();
    m_builder.write_repeat_word();
    process_body(stmt.body, m_config.loopBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const ThrowStmt& stmt)
{
    m_builder.write_throw_word();
    accept_node(stmt.val);
}

void PseudocodeVisitor::visit(const CatchStmt& stmt)
{
    m_builder.write_catch_word();
    //process_params_or_args(stmt.param, false); TODO: fix catch
    process_body(stmt.body, m_config.trycatchBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const TryStmt& stmt)
{
}

void PseudocodeVisitor::visit(const UnknownStmt& /*stmt*/)
{
    m_builder.write_unknown_stmt();
}

void PseudocodeVisitor::visit(const LocalVarDefStmt& stmt)
{
    process_var_def(stmt, LOCAL_VAR);
}

void PseudocodeVisitor::visit(const ParamVarDefStmt& stmt)
{
    process_var_def(stmt, PARAM_VAR);
}

void PseudocodeVisitor::visit(const MemberVarDefStmt& stmt)
{
    process_var_def(stmt, MEMBER_VAR);
}

void PseudocodeVisitor::visit(const GlobalVarDefStmt& stmt)
{
    process_var_def(stmt, GLOBAL_VAR);
}

void PseudocodeVisitor::visit(const FunctionDefStmt& stmt)
{
    if (! m_config.shFuncDeclar)
    {
        return;
    }
    m_builder.write_function_word();
    m_builder.write_space();
    m_builder.write_function_name(stmt.name);
    process_params_or_args(stmt.params, false);
    process_return_type(stmt.retType);
    if (m_config.shFuncDefin)
    {
        process_body(stmt.body, m_config.functionBlockBracketNewLine);
        return;
    }
    m_builder.write_space();
    m_builder.write_left_bracket("{");
    m_builder.write_right_bracket("}");
}

void PseudocodeVisitor::visit(const MultiVarDefStmt& stmt)
{
    m_builder.write_define_word();
    m_builder.write_space();
    accept_node(stmt.defs.at(0)->type);
    for (size_t i = 0; i < stmt.defs.size(); ++i)
    {
        m_builder.write_space();
        m_builder.write_local_var_name(stmt.defs.at(i)->name);
        if (stmt.defs.at(i)->initializer)
        {
            m_builder.write_space();
            m_builder.write_assign_operator();
            m_builder.write_space();
            accept_node(stmt.defs.at(i)->initializer);
        }
        if (i < stmt.defs.size() - 1)
        {
            m_builder.write_comma_space();
        }
    }
}

void PseudocodeVisitor::visit(const MethodDefStmt& stmt)
{
    if (!m_config.shCoDeMeDefin)
    {
        return;
    }
    ClassDefStmt* ownerClass = static_cast<ClassDefStmt*>(stmt.owner);
    if (m_config.shCoDeMeOwner && m_config.shCoDeMeTemplate && stmt.owner)
    {
        process_generic_params_decl(ownerClass->tparams);
    }
    m_builder.write_method_word();
    m_builder.write_space();
    if (m_config.shCoDeMeOwner && stmt.owner)
    {
        // m_builder.write_class_name(stmt.owner_->name_); // TODO what's next
        if (! ownerClass->tparams.empty() && m_config.shCoDeMeTemplate)
        {
            if (m_config.shTemplateDeclar)
            {
                process_params_or_args(ownerClass->tparams, true);
            }
        }
        m_builder.write_separator("::");
    }
    m_builder.write_method_name(stmt.func->name);
    process_params_or_args(stmt.func->params, false);
    process_return_type(stmt.func->retType);
    process_body(stmt.func->body, m_config.functionBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const BaseInitializerStmt& stmt)
{
    m_builder.write_call_word();
    m_builder.write_space();
    m_builder.write_constructor_word();
    m_builder.write_space();
    // m_builder.write_class_name(stmt.base_);
    m_builder.write_class_name(stmt.type->name); // TODO check
    process_params_or_args(stmt.args, false);
}

void PseudocodeVisitor::visit(const SelfInitializerStmt& stmt)
{
}

void PseudocodeVisitor::visit(const MemberInitializerStmt& stmt)
{
}

void PseudocodeVisitor::visit(const ConstructorDefStmt& stmt)
{
    if (!m_config.shCoDeMeDefin)
    {
        return;
    }
    if (m_config.shCoDeMeOwner && m_config.shCoDeMeTemplate && stmt.owner)
    {
        if (m_config.shTemplateDeclar && ! stmt.owner->tparams.empty())
        {
            m_builder.write_template_word();
            process_params_or_args(stmt.owner->tparams, true);
            m_builder.write_new_line();
        }
    }
    m_builder.write_constructor_word();
    m_builder.write_space();
    if (m_config.shCoDeMeOwner && stmt.owner)
    {
        // m_builder.write_class_name(stmt.owner_->name_);
        m_builder.write_class_name(stmt.owner->type->name); // TODO check
        if (! stmt.owner->tparams.empty() && m_config.shCoDeMeTemplate)
        {
            process_params_or_args(stmt.owner->tparams, true);
        }
        m_builder.write_separator("::");
    }
    // m_builder.write_class_name(stmt.owner_->name_);
    m_builder.write_class_name(stmt.owner->type->name); // TODO check
    process_params_or_args(stmt.params, false);
    if (! stmt.baseInit.empty())
    {
        for (size_t i = 0; i < stmt.baseInit.size(); ++i)
        {
            m_builder.write_new_line();
            m_builder.write_space();
            m_builder.write_separator("->");
            m_builder.write_space();
            accept_node(stmt.baseInit.at(i));
        }
    }
    process_body(stmt.body, m_config.functionBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const DestructorDefStmt& stmt)
{
    if (! m_config.shCoDeMeDefin && ! stmt.body)
    {
        return;
    }
    if (m_config.shCoDeMeOwner && m_config.shCoDeMeTemplate && stmt.owner)
    {
        if (m_config.shTemplateDeclar && ! stmt.owner->tparams.empty())
        {
            m_builder.write_template_word();
            process_params_or_args(stmt.owner->tparams, true);
            m_builder.write_new_line();
        }
    }
    m_builder.write_destructor_word();
    m_builder.write_space();
    if (m_config.shCoDeMeOwner && stmt.owner)
    {
        // m_builder.write_class_name(stmt.owner_->name_);
        m_builder.write_class_name(stmt.owner->type->name); // TODO check
        if (! stmt.owner->tparams.empty() && m_config.shCoDeMeTemplate)
        {
            process_params_or_args(stmt.owner->tparams, true);
        }
        m_builder.write_separator("::");
    }
    // m_builder.write_class_name("~" + stmt.owner_->name_);
    m_builder.write_class_name("~" + stmt.owner->type->name); // TODO check
    m_builder.write_left_bracket("(");
    m_builder.write_right_bracket(")");
    process_body(stmt.body, m_config.functionBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const GenericParam& stmt)
{
    m_builder.write_template_name(stmt.name);
}

void PseudocodeVisitor::visit(const InterfaceDefStmt& stmt)
{
    if (! m_config.shInterfDeclar)
    {
        return;
    }
    if (m_config.shTemplateDeclar && ! stmt.tparams.empty())
    {
        m_builder.write_template_word();
        process_params_or_args(stmt.tparams, true);
        m_builder.write_new_line();
    }
    m_builder.write_interface_word();
    m_builder.write_space();
    // m_builder.write_interface_name(stmt.name_);
    m_builder.write_interface_name(stmt.type->name); // TODO check
    if (! stmt.bases.empty())
    {
        process_relations(stmt.bases, true);
    }
    m_builder.write_opening_curl_bracket(m_config.objectBlockBracketNewLine);
    if (m_config.shInterfDefin && m_config.shCoDeMeDeclar
        && ! stmt.methods.empty())
    {
        process_method_decl(stmt.methods);
    }
    m_builder.write_right_bracket("}");
    m_builder.write_new_line();
}

void PseudocodeVisitor::visit(const ClassDefStmt& stmt)
{
    if (m_config.shClassDeclar)
    {
        if (m_config.shTemplateDeclar && ! stmt.tparams.empty())
        {
            m_builder.write_template_word();
            process_params_or_args(stmt.tparams, true);
            m_builder.write_new_line();
        }
        m_builder.write_class_word();
        m_builder.write_space();
        // m_builder.write_class_name(stmt.name_);
        m_builder.write_class_name(stmt.type->name); // TODO check
        if (! stmt.interfaces.empty())
        {
            process_relations(stmt.interfaces, true);
        }
        if (! stmt.bases.empty())
        {
            process_relations(stmt.bases, false);
        }
        m_builder.write_opening_curl_bracket(m_config.objectBlockBracketNewLine);
        if (m_config.shClassDefin)
        {
            if (m_config.shMembVarDeclar && ! stmt.vars.empty())
            {
                process_member_var_decl(stmt.vars);
            }
            if (m_config.shCoDeMeDeclar)
            {
                if (! stmt.constructors.empty())
                {
                    process_constructor_decl(stmt.constructors);
                }
                if (! stmt.destructors.empty())
                {
                    process_destructor_decl(stmt.destructors);
                }
                if (! stmt.methods.empty())
                {
                    process_method_decl(stmt.methods);
                }
            }
        }
        m_builder.write_right_bracket("}");
        m_builder.write_new_line();
    }
    if (m_config.shCoDeMeDefin)
    {
        for (size_t i = 0; i < stmt.constructors.size(); ++i)
        {
            if (stmt.constructors.at(i))
            {
                m_builder.write_new_line();
                stmt.constructors.at(i)->accept(*this);
                m_builder.write_new_line();
            }
        }
        for (size_t i = 0; i < stmt.destructors.size(); ++i)
        {
            if (stmt.destructors.at(i) && stmt.destructors.at(i)->body)
            {
                m_builder.write_new_line();
                stmt.destructors.at(i)->accept(*this);
                m_builder.write_new_line();
            }
        }
        for (size_t i = 0; i < stmt.methods.size(); ++i)
        {
            if (stmt.methods.at(i) && stmt.methods.at(i)->func && stmt.methods.at(i)->func->body)
            {
                m_builder.write_new_line();
                stmt.methods.at(i)->accept(*this);
                m_builder.write_new_line();
            }
        }
    }
}

void PseudocodeVisitor::visit(const ContinueStmt& /*stmt*/)
{
    m_builder.write_continue_word();
}

void PseudocodeVisitor::visit(const BreakStmt& /*stmt*/)
{
    m_builder.write_break_word();
}

void PseudocodeVisitor::process_var_def(const VarDefStmt& var, int vartype)
{
    if (vartype != PARAM_VAR)
    {
        m_builder.write_define_word();
        m_builder.write_space();
    }
    accept_node(var.type);
    m_builder.write_space();
    if (vartype == LOCAL_VAR)
    {
        m_builder.write_local_var_name(var.name);
    }
    else if (vartype == PARAM_VAR)
    {
        m_builder.write_param_var_name(var.name);
    }
    else if (vartype == MEMBER_VAR)
    {
        m_builder.write_member_var_name(var.name);
    }
    else if (vartype == GLOBAL_VAR)
    {
        m_builder.write_global_var_name(var.name);
    }
    if (var.initializer)
    {
        m_builder.write_space();
        m_builder.write_assign_operator();
        m_builder.write_space();
        accept_node(var.initializer);
    }
}

void PseudocodeVisitor::process_return_type(Type* const& type)
{
    m_builder.write_space();
    m_builder.write_returns_word();
    m_builder.write_space();
    accept_node(type);
}

void PseudocodeVisitor::process_generic_params_decl(const std::vector<GenericParam*>& vgeneric)
{
    if (m_config.shTemplateDeclar && ! vgeneric.empty())
    {
        m_builder.write_template_word();
        process_params_or_args(vgeneric, true);
        m_builder.write_new_word();
    }
}

void PseudocodeVisitor::process_member_var_decl(const std::vector<MemberVarDefStmt*>& vmembervars)
{
    std::vector<MemberVarDefStmt*> vfound{};
    if (try_find_access_mod(vmembervars, vfound, AccessModifier::Public))
    {
        m_builder.write_public_word();
        process_member_var(vfound);
    }
    if (try_find_access_mod(vmembervars, vfound, AccessModifier::Private))
    {
        m_builder.write_private_word();
        process_member_var(vfound);
    }
    if (try_find_access_mod(vmembervars, vfound, AccessModifier::Protected))
    {
        m_builder.write_protected_word();
        process_member_var(vfound);
    }
    if (try_find_access_mod(vmembervars, vfound, AccessModifier::Internal))
    {
        m_builder.write_internal_word();
        process_member_var(vfound);
    }
}

void PseudocodeVisitor::process_member_var(std::vector<MemberVarDefStmt*>& vmembervars)
{
    m_builder.write_space();
    m_builder.write_attributes_word();
    m_builder.write_new_line();
    m_builder.increase_indentation();
    for (size_t i = 0; i < vmembervars.size(); ++i)
    {
        if (vmembervars.at(i))
        {
            vmembervars.at(i)->accept(*this);
            m_builder.write_new_line();
        }
    }
    m_builder.decrease_indentation();
    vmembervars.clear();
}

void PseudocodeVisitor::process_constructor_decl(const std::vector<ConstructorDefStmt*>& vconstructors)
{
    std::vector<ConstructorDefStmt*> vfound{};
    if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Public)))
    {
        m_builder.write_public_word();
        process_constructor(vfound);
    }
    if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Private)))
    {
        m_builder.write_private_word();
        process_constructor(vfound);
    }
    if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Protected)))
    {
        m_builder.write_protected_word();
        process_constructor(vfound);
    }
    if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Internal)))
    {
        m_builder.write_internal_word();
        process_constructor(vfound);
    }
}

void PseudocodeVisitor::process_constructor(std::vector<ConstructorDefStmt*>& constr)
{
    m_builder.write_space();
    m_builder.write_constructors_word();
    m_builder.write_new_line();
    m_builder.increase_indentation();
    for (size_t i = 0; i < constr.size(); ++i)
    {
        // m_builder.write_class_name(constr.at(i)->owner_->name_);
        m_builder.write_class_name(constr.at(i)->owner->type->name); // TODO check
        process_params_or_args(constr.at(i)->params, false);
        m_builder.write_new_line();
    }
    m_builder.decrease_indentation();
    constr.clear();
}

void PseudocodeVisitor::process_destructor_decl(const std::vector<DestructorDefStmt*>& vdestructors)
{
    m_builder.write_destructors_word();
    m_builder.write_new_line();
    m_builder.increase_indentation();
    for (size_t i = 0; i < vdestructors.size(); ++i)
    {
        // m_builder.write_class_name("~" + vdestructors.at(i)->owner_->name_);
        m_builder.write_class_name("~" + vdestructors.at(i)->owner->type->name); // TODO check
        m_builder.write_left_bracket("(");
        m_builder.write_right_bracket(")");
        m_builder.write_new_line();
    }
    m_builder.decrease_indentation();
}

void PseudocodeVisitor::process_method_decl(const std::vector<MethodDefStmt*>& vmethods)
{
    std::vector<MethodDefStmt*> vfound{};
    if (try_find_access_mod(vmethods, vfound, AccessModifier::Public))
    {
        m_builder.write_public_word();
        process_method(vfound);
    }
    if (try_find_access_mod(vmethods, vfound, AccessModifier::Private))
    {
        m_builder.write_private_word();
        process_method(vfound);
    }
    if (try_find_access_mod(vmethods, vfound, AccessModifier::Protected))
    {
        m_builder.write_protected_word();
        process_method(vfound);
    }
    if (try_find_access_mod(vmethods, vfound, AccessModifier::Internal))
    {
        m_builder.write_internal_word();
        process_method(vfound);
    }
}

void PseudocodeVisitor::process_method(std::vector<MethodDefStmt*>& meth)
{
    m_builder.write_space();
    m_builder.write_methods_word();
    m_builder.write_new_line();
    m_builder.increase_indentation();
    for (size_t i = 0; i < meth.size(); ++i)
    {
        if (meth.at(i)->func->body)
        {
            if (meth.at(i)->virtuality == Virtuality::Virtual)
            {
                m_builder.write_virtual_word();
                m_builder.write_separator(" -> ");
            }
        }
        else
        {
            m_builder.write_abstract_word();
            m_builder.write_separator(" -> ");
        }
        m_builder.write_method_name(meth.at(i)->func->name);
        process_params_or_args(meth.at(i)->func->params, false);
        process_return_type(meth.at(i)->func->retType);
        m_builder.write_new_line();
    }
    m_builder.decrease_indentation();
    meth.clear();
}