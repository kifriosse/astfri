#include "NodeMapper.hpp"

NodeMapper::NodeMapper() : stmtFactory(astfri::StmtFactory::get_instance()), exprFactory(astfri::ExprFactory::get_instance()) {
    astfri::TypeFactory& typeFactory = astfri::TypeFactory::get_instance();
    
    const std::map<std::string, astfri::Type*> typeMap = {
        {"int", typeFactory.mk_int()},
        {"float", typeFactory.mk_float()},
        {"char", typeFactory.mk_char()},
        {"boolean", typeFactory.mk_bool()},
        {"void", typeFactory.mk_void()},
   };

   const std::map<std::string, std::function<astfri::Stmt*(StmtArguments)>> stmtMap = {
       {"local_variable_declaration", map_local_variable()},
       {"field_declaration", map_member_variable()},
       {"formal_parameter", map_formal_parameter()},
       {"class_declaration", map_class_node()},
       {"method_decleration", map_method_node()},
       {"body", map_body_node()},
       {"return_statement", map_return_node()},
       {"expression_statement", map_expression_statement()},
       {"if_statement", map_if_statement()},
       {"switch_label", map_case_node()},
       {"switch_expression", map_switch_node()},
       {"while_statement", map_while_node()},
       {"do_statement", map_do_while_node()},
       {"for_statement", map_for_node()},
   };

   const std::map<std::string, std::function<astfri::Expr*(ExprArguments)>> exprMap = {
       {"decimal_integer_literal", map_integer_literal()},
       {"decimal_floating_point_literal", map_float_literal()},
       {"character_literal", map_char_literal()},
       {"string_literal", map_string_literal()},
       {"true", map_boolean_literal()},
       {"false", map_boolean_literal()},
       {"binary_expression", map_binary_operation()},
       {"unary_expression", map_unary_operation()},
       {"=", map_assignment()},
       {"+=", map_compound_assignment()},
       {"-=", map_compound_assignment()},
       {"*=", map_compound_assignment()},
       {"/=", map_compound_assignment()},
       {"%=", map_compound_assignment()},
       {"identifier", map_parameter_variable_reference()},
   };
}

std::map<std::string, astfri::Type*> NodeMapper::get_typeMap() {
    return this->typeMap;
}

std::map<std::string, std::function<astfri::Expr*(NodeMapper::ExprArguments)>> NodeMapper::get_exprMap() {
    return this->exprMap;
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_local_variable() {
    return [this](NodeMapper::StmtArguments arg) {
        return std::apply([this](std::string name, astfri::Type* type, astfri::Expr* init) {
            return stmtFactory.mk_local_var_def(name, type, init);
        }, std::get<std::tuple<std::string, astfri::Type*, astfri::Expr*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_member_variable() {
    return [this](NodeMapper::StmtArguments arg) {
        return std::apply([this](std::string name, astfri::Type* type, astfri::Expr* init, astfri::AccessModifier access) {
            return stmtFactory.mk_member_var_def(name, type, init, access);
        }, std::get<std::tuple<std::string, astfri::Type*, astfri::Expr*, astfri::AccessModifier>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_formal_parameter() {
    return [this](NodeMapper::StmtArguments arg) {
        return std::apply([this](std::string name, astfri::Type* type, astfri::Expr* init) {
            return stmtFactory.mk_param_var_def(name, type, init);
        }, std::get<std::tuple<std::string, astfri::Type*, astfri::Expr*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_class_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](std::string name, std::vector<astfri::MemberVarDefStmt*> vars, std::vector<astfri::MethodDefStmt*> methods, std::vector<astfri::GenericParam*> tparams) {
            return stmtFactory.mk_class_def(name, vars, methods, tparams);
        }, std::get<std::tuple<std::string, std::vector<astfri::MemberVarDefStmt*>, std::vector<astfri::MethodDefStmt*>, std::vector<astfri::GenericParam*>>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_method_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](std::string name, std::vector<astfri::ParamVarDefStmt*> params, astfri::Type* type, astfri::CompoundStmt* body) {
            return stmtFactory.mk_function_def(name, params, type, body);
        }, std::get<std::tuple<std::string, std::vector<astfri::ParamVarDefStmt*>, astfri::Type*, astfri::CompoundStmt*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_body_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](std::vector<astfri::Stmt*> stmts) {
            return stmtFactory.mk_compound(stmts);
        }, std::get<std::tuple<std::vector<astfri::Stmt*>>>(arg));
    };    
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_return_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](astfri::Expr* val) {
            return stmtFactory.mk_return(val);
        }, std::get<std::tuple<astfri::Expr*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_expression_statement() {
    return [this](StmtArguments arg) {
        return std::apply([this](astfri::Expr* expr) {
            return stmtFactory.mk_expr(expr);
        }, std::get<std::tuple<astfri::Expr*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_if_statement() {
    return [this](StmtArguments arg) {
        return std::apply([this](astfri::Expr* cond, astfri::Stmt* iftrue, astfri::Stmt* iffalse) {
            return stmtFactory.mk_if(cond, iftrue, iffalse);
        }, std::get<std::tuple<astfri::Expr*, astfri::Stmt*, astfri::Stmt*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_case_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](astfri::Expr* expr, astfri::Stmt* body) {
            return stmtFactory.mk_case(expr, body);
        }, std::get<std::tuple<astfri::Expr*, astfri::Stmt*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_switch_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](astfri::Expr* expr, std::vector<astfri::CaseStmt*> cases) {
            return stmtFactory.mk_switch(expr, cases);
        }, std::get<std::tuple<astfri::Expr*, std::vector<astfri::CaseStmt*>>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_while_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](astfri::Expr* cond, astfri::CompoundStmt* body) {
            return stmtFactory.mk_while(cond, body);
        }, std::get<std::tuple<astfri::Expr*, astfri::CompoundStmt*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_do_while_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](astfri::Expr* cond, astfri::CompoundStmt* body) {
            return stmtFactory.mk_do_while(cond, body);
        }, std::get<std::tuple<astfri::Expr*, astfri::CompoundStmt*>>(arg));
    };
}

std::function<astfri::Stmt*(NodeMapper::StmtArguments)> NodeMapper::map_for_node() {
    return [this](StmtArguments arg) {
        return std::apply([this](astfri::Stmt* init, astfri::Expr* cond, astfri::Stmt* step, astfri::CompoundStmt* body) {
            return stmtFactory.mk_for(init, cond, step, body);
        }, std::get<std::tuple<astfri::Stmt*, astfri::Expr*, astfri::Stmt*, astfri::CompoundStmt*>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_integer_literal() {
    return [this](ExprArguments arg) {
        return std::apply([this](int val) {
            return exprFactory.mk_int_literal(val);
        }, std::get<std::tuple<int>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_float_literal() {
    return [this](ExprArguments arg) {
        return std::apply([this](float val) {
            return exprFactory.mk_float_literal(val);
        }, std::get<std::tuple<float>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_char_literal() {
    return [this](ExprArguments arg) {
        return std::apply([this](char val) {
            return exprFactory.mk_char_literal(val);
        }, std::get<std::tuple<char>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_string_literal() {
    return [this](ExprArguments arg) {
        return std::apply([this](std::string val) {
            return exprFactory.mk_string_literal(val);
        }, std::get<std::tuple<std::string>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_boolean_literal() {
    return [this](ExprArguments arg) {
        return std::apply([this](bool val) {
            return exprFactory.mk_bool_literal(val);
        }, std::get<std::tuple<bool>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_binary_operation() {
    return [this](ExprArguments arg) {
        return std::apply([this](astfri::Expr* left, astfri::BinOpType op, astfri::Expr* right) {
            return exprFactory.mk_bin_on(left, op, right);
        }, std::get<std::tuple<astfri::Expr*, astfri::BinOpType, astfri::Expr*>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_unary_operation() {
    return [this](ExprArguments arg) {
        return std::apply([this](astfri::UnaryOpType op, astfri::Expr* arg) {
            return exprFactory.mk_unary_op(op, arg);
        }, std::get<std::tuple<astfri::UnaryOpType, astfri::Expr*>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_assignment() {
    return [this](ExprArguments arg) {
        return std::apply([this](astfri::Expr* lhs, astfri::Expr* rhs) {
            return exprFactory.mk_assign(lhs, rhs);
        }, std::get<std::tuple<astfri::Expr*, astfri::Expr*>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_compound_assignment() {
    return [this](ExprArguments arg) {
        return std::apply([this](astfri::Expr* lhs, astfri::BinOpType op, astfri::Expr* rhs) {
            return exprFactory.mk_compound_assign(lhs, op, rhs);
        }, std::get<std::tuple<astfri::Expr*, astfri::BinOpType, astfri::Expr*>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_parameter_variable_reference() {
    return [this](ExprArguments arg) {
        return std::apply([this](std::string param) {
            return exprFactory.mk_param_var_ref(param);
        }, std::get<std::tuple<std::string>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_local_variable_reference() {
    return [this](ExprArguments arg) {
        return std::apply([this](std::string var) {
            return exprFactory.mk_local_var_ref(var);
        }, std::get<std::tuple<std::string>>(arg));
    };
}

std::function<astfri::Expr*(NodeMapper::ExprArguments)> NodeMapper::map_member_variable_reference() {
    return [this](ExprArguments arg) {
        return std::apply([this](std::string member) {
            return exprFactory.mk_member_var_ref(member);
        }, std::get<std::tuple<std::string>>(arg));
    };
}