#include <libastfri-text/inc/pseudocode/PseudocodeVisitor.hpp>

using namespace astfri::text;

constexpr int LOCAL_VAR  = 0;
constexpr int PARAM_VAR  = 1;
constexpr int MEMBER_VAR = 2;
constexpr int GLOBAL_VAR = 3;

PseudocodeVisitor::PseudocodeVisitor(PseudocodeBuilder* builder) :
    AbstractVisitor(builder),
    m_builder(builder)
{
}

void PseudocodeVisitor::visit(const TranslationUnit& stmt) {
    bool previous = false;
    if (configurator_->sh_global_vars()) {
        for (GlobalVarDefStmt* a : stmt.globals) {
            if (a) {
                a->accept(*this);
                m_builder.append_new_line();
                previous = true;
            }
        }
    }
    if (configurator_->sh_func_declar()) {
        for (FunctionDefStmt* a : stmt.functions) {
            if (a) {
                if (previous) {
                    m_builder.append_new_line();
                }
                a->accept(*this);
                m_builder.append_new_line();
                previous = true;
            }
        }
    }
    if (configurator_->sh_interf_declar()) {
        for (InterfaceDefStmt* a : stmt.interfaces) {
            if (a) {
                if (previous) {
                    m_builder.append_new_line();
                }
                a->accept(*this);
                m_builder.append_new_line();
                previous = true;
            }
        }
    }
    for (ClassDefStmt* a : stmt.classes) {
        if (a) {
            if (previous) {
                m_builder.append_new_line();
            }
            a->accept(*this);
            m_builder.append_new_line();
            previous = true;
        }
    }
}




void PseudocodeVisitor::visit(const CaseStmt& stmt) {
    m_builder.write_case_word();
    m_builder.append_space();
    if (stmt.exprs.empty()) {
        m_builder.write_invalid_stmt();
    }
    else {
        for (size_t i = 0; i < stmt.exprs.size(); ++i) {
            if (! try_accept_node(stmt.exprs.at(i))) {
                m_builder.write_invalid_stmt();
            }
            if (i < stmt.exprs.size() - 1) {
                m_builder.write_separator(",");
                m_builder.append_space();
            }
        }
    }
    m_builder.append_space();
    m_builder.write_do_word();
    process_body(stmt.body);
}


void PseudocodeVisitor::visit(const SwitchStmt& stmt) {
    m_builder.write_switch_word();
    process_condition(stmt.expr_);
    m_builder.write_opening_curl_bracket();
    m_builder.increase_indentation();
    for (size_t i = 0; i < stmt.cases.size(); ++i) {
        if (! try_accept_node(stmt.cases.at(i))) {
            m_builder.write_invalid_stmt();
        }
        if (i < stmt.cases.size() - 1) {
            m_builder.append_new_line();
        }
    }
    m_builder.decrease_indentation();
    m_builder.append_new_line();
    m_builder.write_right_bracket("}");
}

void PseudocodeVisitor::visit(const ThrowStmt& stmt) {
    m_builder.write_throw_word();
    m_builder.append_space();
    if (! try_accept_node(stmt.val)) {
        m_builder.write_invalid_expr();
    }
}

void PseudocodeVisitor::visit(const UnknownStmt& /*stmt*/) {
    m_builder.write_unknown_stmt();
}

void PseudocodeVisitor::visit(const LocalVarDefStmt& stmt) {
    process_var_def(stmt, LOCAL_VAR);
}

void PseudocodeVisitor::visit(const ParamVarDefStmt& stmt) {
    process_var_def(stmt, PARAM_VAR);
}

void PseudocodeVisitor::visit(const MemberVarDefStmt& stmt) {
    process_var_def(stmt, MEMBER_VAR);
}

void PseudocodeVisitor::visit(const GlobalVarDefStmt& stmt) {
    process_var_def(stmt, GLOBAL_VAR);
}

void PseudocodeVisitor::visit(const FunctionDefStmt& stmt) {
    if (! configurator_->sh_func_declar()) {
        return;
    }
    if (configurator_->sh_other_expr()) {
        m_builder.write_function_word();
        m_builder.append_space();
    }
    m_builder.write_function_name(stmt.name);
    process_pargs(stmt.params, false);
    process_return_type(stmt.retType);
    if (configurator_->sh_func_defin()) {
        process_body(stmt.body);
        return;
    }
    m_builder.append_space();
    m_builder.write_left_bracket("{");
    m_builder.write_right_bracket("}");
}

void PseudocodeVisitor::visit(const DefStmt& stmt) {
    if (stmt.defs.empty()) {
        m_builder.write_invalid_stmt();
        return;
    }
    if (configurator_->sh_other_expr()) {
        m_builder.write_define_word();
        m_builder.append_space();
    }
    if (! try_accept_node(stmt.defs.at(0)->type)) {
        m_builder.write_invalid_type();
    }
    for (size_t i = 0; i < stmt.defs.size(); ++i) {
        m_builder.append_space();
        m_builder.write_local_var_name(stmt.defs.at(i)->name);
        if (stmt.defs.at(i)->initializer) {
            m_builder.append_space();
            m_builder.write_assign_operator();
            m_builder.append_space();
            if (! try_accept_node(stmt.defs.at(i)->initializer)) {
                m_builder.write_invalid_expr();
            }
        }
        if (i < stmt.defs.size() - 1) {
            m_builder.write_separator(",");
            m_builder.append_space();
        }
    }
}

void PseudocodeVisitor::visit(const MethodDefStmt& stmt) {
    if (! configurator_->sh_con_des_meth_defin()
        || (stmt.access != AccessModifier::Public && ! configurator_->use_inner_view())
        || ! stmt.func) {
        return;
    }
    ClassDefStmt* ownerClass = static_cast<ClassDefStmt*>(stmt.owner);
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template()
        && stmt.owner) {
        process_generic_params_decl(ownerClass->tparams);
    }
    if (configurator_->sh_other_expr()) {
        m_builder.write_method_word();
        m_builder.append_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner) {
        // m_builder.write_class_name(stmt.owner_->name_); // TODO what's next
        if (! ownerClass->tparams.empty() && configurator_->sh_con_des_meth_template()) {
            if (configurator_->sh_generic_param()) {
                process_pargs(ownerClass->tparams, true);
            }
        }
        m_builder.write_separator("::");
    }
    m_builder.write_method_name(stmt.func->name);
    process_pargs(stmt.func->params, false);
    process_return_type(stmt.func->retType);
    process_body(stmt.func->body);
}

void PseudocodeVisitor::visit(const BaseInitializerStmt& stmt) {
    if (configurator_->sh_other_expr()) {
        m_builder.write_call_word();
        m_builder.append_space();
        m_builder.write_constructor_word();
        m_builder.append_space();
    }
    // m_builder.write_class_name(stmt.base_);
    m_builder.write_class_name(stmt.type->name); // TODO check
    process_pargs(stmt.args, false);
}

void PseudocodeVisitor::visit(const ConstructorDefStmt& stmt) {
    if (! configurator_->sh_con_des_meth_defin()
        || (stmt.access != AccessModifier::Public && ! configurator_->use_inner_view())) {
        return;
    }
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template()
        && stmt.owner) {
        if (configurator_->sh_generic_param() && ! stmt.owner->tparams.empty()) {
            m_builder.write_template_word();
            process_pargs(stmt.owner->tparams, true);
            m_builder.append_new_line();
        }
    }
    if (configurator_->sh_other_expr()) {
        m_builder.write_constructor_word();
        m_builder.append_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner) {
        // m_builder.write_class_name(stmt.owner_->name_);
        m_builder.write_class_name(stmt.owner->type->name); // TODO check
        if (! stmt.owner->tparams.empty() && configurator_->sh_con_des_meth_template()) {
            process_pargs(stmt.owner->tparams, true);
        }
        m_builder.write_separator("::");
    }
    if (stmt.owner) {
        // m_builder.write_class_name(stmt.owner_->name_);
        m_builder.write_class_name(stmt.owner->type->name); // TODO check
    }
    else {
        m_builder.write_invalid_expr();
    }
    process_pargs(stmt.params, false);
    if (! stmt.baseInit.empty()) {
        for (size_t i = 0; i < stmt.baseInit.size(); ++i) {
            m_builder.append_new_line();
            m_builder.append_space();
            m_builder.write_separator("->");
            m_builder.append_space();
            if (! try_accept_node(stmt.baseInit.at(i))) {
                m_builder.write_invalid_stmt();
            }
        }
    }
    process_body(stmt.body);
}

void PseudocodeVisitor::visit(const DestructorDefStmt& stmt) {
    if (! configurator_->sh_con_des_meth_defin() && ! stmt.body) {
        return;
    }
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template()
        && stmt.owner) {
        if (configurator_->sh_generic_param() && ! stmt.owner->tparams.empty()) {
            m_builder.write_template_word();
            process_pargs(stmt.owner->tparams, true);
            m_builder.append_new_line();
        }
    }
    if (configurator_->sh_other_expr()) {
        m_builder.write_destructor_word();
        m_builder.append_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner) {
        // m_builder.write_class_name(stmt.owner_->name_);
        m_builder.write_class_name(stmt.owner->type->name); // TODO check
        if (! stmt.owner->tparams.empty() && configurator_->sh_con_des_meth_template()) {
            process_pargs(stmt.owner->tparams, true);
        }
        m_builder.write_separator("::");
    }
    if (stmt.owner) {
        // m_builder.write_class_name("~" + stmt.owner_->name_);
        m_builder.write_class_name("~" + stmt.owner->type->name); // TODO check
    }
    else {
        m_builder.write_invalid_expr();
    }
    m_builder.write_left_bracket("(");
    m_builder.write_right_bracket(")");
    process_body(stmt.body);
}

void PseudocodeVisitor::visit(const GenericParam& stmt) {
    m_builder.write_gen_param_name(stmt.name);
}

void PseudocodeVisitor::visit(const InterfaceDefStmt& stmt) {
    if (! configurator_->sh_interf_declar()) {
        return;
    }
    if (configurator_->sh_generic_param() && ! stmt.tparams.empty()) {
        m_builder.write_template_word();
        process_pargs(stmt.tparams, true);
        m_builder.append_new_line();
    }
    m_builder.write_interface_word();
    m_builder.append_space();
    // m_builder.write_interface_name(stmt.name_);
    m_builder.write_interface_name(stmt.type->name); // TODO check
    if (! stmt.bases.empty()) {
        process_relations(stmt.bases, true);
    }
    m_builder.write_opening_curl_bracket();
    if (configurator_->sh_interf_defin() && configurator_->sh_con_des_meth_declar()
        && ! stmt.methods.empty()) {
        process_method_decl(stmt.methods);
    }
    m_builder.write_right_bracket("}");
    m_builder.append_new_line();
}

void PseudocodeVisitor::visit(const ClassDefStmt& stmt) {
    if (configurator_->sh_class_declar()) {
        if (configurator_->sh_generic_param() && ! stmt.tparams.empty()) {
            m_builder.write_template_word();
            process_pargs(stmt.tparams, true);
            m_builder.append_new_line();
        }
        m_builder.write_class_word();
        m_builder.append_space();
        // m_builder.write_class_name(stmt.name_);
        m_builder.write_class_name(stmt.type->name); // TODO check
        if (! stmt.interfaces.empty()) {
            process_relations(stmt.interfaces, true);
        }
        if (! stmt.bases.empty()) {
            process_relations(stmt.bases, false);
        }
        m_builder.write_opening_curl_bracket();
        if (configurator_->sh_class_defin()) {
            if (configurator_->sh_member_vars() && ! stmt.vars.empty()) {
                process_member_var_decl(stmt.vars);
            }
            if (configurator_->sh_con_des_meth_declar()) {
                if (! stmt.constructors.empty()) {
                    process_constructor_decl(stmt.constructors);
                }
                if (! stmt.destructors.empty()) {
                    process_destructor_decl(stmt.destructors);
                }
                if (! stmt.methods.empty()) {
                    process_method_decl(stmt.methods);
                }
            }
        }
        m_builder.write_right_bracket("}");
        m_builder.append_new_line();
    }
    if (configurator_->sh_con_des_meth_defin()) {
        for (size_t i = 0; i < stmt.constructors.size(); ++i) {
            if (stmt.constructors.at(i)) {
                m_builder.append_new_line();
                stmt.constructors.at(i)->accept(*this);
                m_builder.append_new_line();
            }
        }
        for (size_t i = 0; i < stmt.destructors.size(); ++i) {
            if (stmt.destructors.at(i) && stmt.destructors.at(i)->body) {
                m_builder.append_new_line();
                stmt.destructors.at(i)->accept(*this);
                m_builder.append_new_line();
            }
        }
        for (size_t i = 0; i < stmt.methods.size(); ++i) {
            if (stmt.methods.at(i) && stmt.methods.at(i)->func && stmt.methods.at(i)->func->body) {
                m_builder.append_new_line();
                stmt.methods.at(i)->accept(*this);
                m_builder.append_new_line();
            }
        }
    }
}

//
// -----
//

void PseudocodeVisitor::process_var_def(const VarDefStmt& var, int vartype) {
    if (configurator_->sh_other_expr() && vartype != PARAM_VAR) {
        m_builder.write_define_word();
        m_builder.append_space();
    }
    if (! try_accept_node(var.type)) {
        m_builder.write_invalid_type();
    }
    m_builder.append_space();
    if (vartype == LOCAL_VAR) {
        m_builder.write_local_var_name(var.name);
    }
    else if (vartype == PARAM_VAR) {
        m_builder.write_param_var_name(var.name);
    }
    else if (vartype == MEMBER_VAR) {
        m_builder.write_member_var_name(var.name);
    }
    else if (vartype == GLOBAL_VAR) {
        m_builder.write_global_var_name(var.name);
    }
    if (var.initializer) {
        m_builder.append_space();
        m_builder.write_assign_operator();
        m_builder.append_space();
        if (! try_accept_node(var.initializer)) {
            m_builder.write_invalid_expr();
        }
    }
}

void PseudocodeVisitor::process_return_type(const Type* const& type) {
    m_builder.append_space();
    m_builder.write_separator("->");
    m_builder.append_space();
    if (configurator_->sh_other_expr()) {
        m_builder.write_returns_word();
        m_builder.append_space();
    }
    if (! try_accept_node(type)) {
        m_builder.write_invalid_type();
    }
}

void PseudocodeVisitor::process_generic_params_decl(const std::vector<GenericParam*>& vgeneric) {
    if (configurator_->sh_generic_param() && ! vgeneric.empty()) {
        m_builder.write_template_word();
        process_pargs(vgeneric, true);
        m_builder.write_new_word();
    }
}

void PseudocodeVisitor::process_member_var_decl(const std::vector<MemberVarDefStmt*>& vmembervars) {
    std::vector<MemberVarDefStmt*> vfound{};
    if (try_find_access_mod(vmembervars, vfound, AccessModifier::Public)) {
        m_builder.write_public_word();
        process_member_var(vfound);
    }
    if (configurator_->use_inner_view()) {
        if (try_find_access_mod(vmembervars, vfound, AccessModifier::Private)) {
            m_builder.write_private_word();
            process_member_var(vfound);
        }
        if (try_find_access_mod(vmembervars, vfound, AccessModifier::Protected)) {
            m_builder.write_protected_word();
            process_member_var(vfound);
        }
        if (try_find_access_mod(vmembervars, vfound, AccessModifier::Internal)) {
            m_builder.write_internal_word();
            process_member_var(vfound);
        }
    }
}

void PseudocodeVisitor::process_member_var(std::vector<MemberVarDefStmt*>& vmembervars) {
    m_builder.append_space();
    m_builder.write_attributes_word();
    m_builder.append_new_line();
    m_builder.increase_indentation();
    for (size_t i = 0; i < vmembervars.size(); ++i) {
        if (vmembervars.at(i)) {
            vmembervars.at(i)->accept(*this);
            m_builder.append_new_line();
        }
    }
    m_builder.decrease_indentation();
    vmembervars.clear();
}

void PseudocodeVisitor::process_constructor_decl(
    const std::vector<ConstructorDefStmt*>& vconstructors
) {
    std::vector<ConstructorDefStmt*> vfound{};
    if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Public))) {
        m_builder.write_public_word();
        process_constructor(vfound);
    }
    if (configurator_->use_inner_view()) {
        if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Private))) {
            m_builder.write_private_word();
            process_constructor(vfound);
        }
        if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Protected))) {
            m_builder.write_protected_word();
            process_constructor(vfound);
        }
        if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Internal))) {
            m_builder.write_internal_word();
            process_constructor(vfound);
        }
    }
}

void PseudocodeVisitor::process_constructor(std::vector<ConstructorDefStmt*>& constr) {
    m_builder.append_space();
    m_builder.write_constructors_word();
    m_builder.append_new_line();
    m_builder.increase_indentation();
    for (size_t i = 0; i < constr.size(); ++i) {
        if (constr.at(i) && constr.at(i)->owner) {
            // m_builder.write_class_name(constr.at(i)->owner_->name_);
            m_builder.write_class_name(constr.at(i)->owner->type->name); // TODO check
            process_pargs(constr.at(i)->params, false);
        }
        else {
            m_builder.write_invalid_stmt();
        }
        m_builder.append_new_line();
    }
    m_builder.decrease_indentation();
    constr.clear();
}

void PseudocodeVisitor::process_destructor_decl(const std::vector<DestructorDefStmt*>& vdestructors
) {
    m_builder.write_destructors_word();
    m_builder.append_new_line();
    m_builder.increase_indentation();
    for (size_t i = 0; i < vdestructors.size(); ++i) {
        if (vdestructors.at(i) && vdestructors.at(i)->owner) {
            // m_builder.write_class_name("~" + vdestructors.at(i)->owner_->name_);
            m_builder.write_class_name("~" + vdestructors.at(i)->owner->type->name); // TODO check
            m_builder.write_left_bracket("(");
            m_builder.write_right_bracket(")");
        }
        else {
            m_builder.write_invalid_stmt();
        }
        m_builder.append_new_line();
    }
    m_builder.decrease_indentation();
}

void PseudocodeVisitor::process_method_decl(const std::vector<MethodDefStmt*>& vmethods) {
    std::vector<MethodDefStmt*> vfound{};
    if (try_find_access_mod(vmethods, vfound, AccessModifier::Public)) {
        m_builder.write_public_word();
        process_method(vfound);
    }
    if (configurator_->use_inner_view()) {
        if (try_find_access_mod(vmethods, vfound, AccessModifier::Private)) {
            m_builder.write_private_word();
            process_method(vfound);
        }
        if (try_find_access_mod(vmethods, vfound, AccessModifier::Protected)) {
            m_builder.write_protected_word();
            process_method(vfound);
        }
        if (try_find_access_mod(vmethods, vfound, AccessModifier::Internal)) {
            m_builder.write_internal_word();
            process_method(vfound);
        }
    }
}

void PseudocodeVisitor::process_method(std::vector<MethodDefStmt*>& meth) {
    m_builder.append_space();
    m_builder.write_methods_word();
    m_builder.append_new_line();
    m_builder.increase_indentation();
    for (size_t i = 0; i < meth.size(); ++i) {
        if (meth.at(i) && meth.at(i)->func) {
            if (meth.at(i)->func->body) {
                if (meth.at(i)->virtuality == Virtuality::Virtual) {
                    m_builder.write_virtual_word();
                    m_builder.write_separator(" -> ");
                }
            }
            else {
                m_builder.write_abstract_word();
                m_builder.write_separator(" -> ");
            }
            m_builder.write_method_name(meth.at(i)->func->name);
            process_pargs(meth.at(i)->func->params, false);
            process_return_type(meth.at(i)->func->retType);
        }
        else {
            m_builder.write_invalid_stmt();
        }
        m_builder.append_new_line();
    }
    m_builder.decrease_indentation();
    meth.clear();
}