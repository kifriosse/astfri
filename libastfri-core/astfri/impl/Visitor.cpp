#include <astfri/impl/Visitor.hpp>

#include <astfri/impl/ExprDef.hpp>
#include <astfri/impl/StmtDef.hpp>
#include <astfri/impl/TypeDef.hpp>

#include <stdexcept>


namespace astfri {


void ThrowingVisitorAdapter::visit(const DynamicType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const IntType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const FloatType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const CharType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const BoolType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const VoidType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const IndirectionType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ClassType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const InterfaceType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const LambdaType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const IncompleteType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const DeducedType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const UnknownType& /*type*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const IntLiteralExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const FloatLiteralExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const CharLiteralExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const StringLiteralExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const BoolLiteralExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const NullLiteralExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const IfExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const BinOpExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const UnaryOpExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ParamVarRefExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const LocalVarRefExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const MemberVarRefExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const GlobalVarRefExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ClassRefExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const FunctionCallExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const MethodCallExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const LambdaCallExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const LambdaExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ThisExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const BaseExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ConstructorCallExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const NewExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const DeleteExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const BracketExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const NamedExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const UnknownExpr& /*expr*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const TranslationUnit& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const CompoundStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ReturnStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ExprStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const IfStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const GotoLabelStmt&) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const GotoStmt&) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const CaseStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const DefaultCaseStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const SwitchStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const WhileStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const DoWhileStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ForStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ForEachStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ThrowStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const CatchStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const TryStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const UnknownStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const LocalVarDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ParamVarDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const MemberVarDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const GlobalVarDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const FunctionDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const MultiVarDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const MethodDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const BaseInitializerStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const SelfInitializerStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const MemberInitializerStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ConstructorDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const DestructorDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const GenericParam& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const InterfaceDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ClassDefStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const ContinueStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}

void ThrowingVisitorAdapter::visit(const BreakStmt& /*stmt*/) {
    throw std::logic_error("Not Implemented Yet!");
}



void PostOrderVisitor::visit(const DynamicType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const IntType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const FloatType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const CharType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const BoolType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const VoidType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const IndirectionType &type) {
    this->on_enter(type);
    type.indirect->accept(*this);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const ClassType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const InterfaceType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const LambdaType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const IncompleteType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const DeducedType &type) {
    this->on_enter(type);
    if (type.realType) {
        type.realType->accept(*this);
    }
    this->process(type);
    this->on_exit(type);
}

void PostOrderVisitor::visit(const UnknownType &type) {
    this->on_enter(type);
    this->process(type);
    this->on_exit(type);
}

// --------------------------------------------------
// Expressions
// --------------------------------------------------

void PostOrderVisitor::visit(const IntLiteralExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const FloatLiteralExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const CharLiteralExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const StringLiteralExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const BoolLiteralExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const NullLiteralExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const IfExpr &expr) {
    this->on_enter(expr);
    expr.cond->accept(*this);
    expr.iftrue->accept(*this);
    expr.iffalse->accept(*this);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const BinOpExpr &expr) {
    this->on_enter(expr);
    expr.left->accept(*this);
    expr.right->accept(*this);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const UnaryOpExpr &expr) {
    this->on_enter(expr);
    expr.arg->accept(*this);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const ParamVarRefExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const LocalVarRefExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const MemberVarRefExpr &expr) {
    this->on_enter(expr);
    if (expr.owner) {
        expr.owner->accept(*this);
    }
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const GlobalVarRefExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const ClassRefExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const FunctionCallExpr &expr) {
    this->on_enter(expr);
    for (auto *item : expr.args) {
        item->accept(*this);
    }
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const MethodCallExpr &expr) {
    this->on_enter(expr);
    if (expr.owner) {
        expr.owner->accept(*this);
    }
    for (auto *item : expr.args) {
        item->accept(*this);
    }
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const LambdaCallExpr &expr) {
    this->on_enter(expr);
    for (auto *item : expr.args) {
        item->accept(*this);
    }
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const LambdaExpr &expr) {
    this->on_enter(expr);
    if (expr.returnType) {
        expr.returnType->accept(*this);
    }
    if (expr.body) {
        expr.body->accept(*this);
    }
    for (auto *item : expr.params) {
        item->accept(*this);
    }
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const ThisExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const BaseExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const ConstructorCallExpr &expr) {
    this->on_enter(expr);
    if (expr.type) {
        expr.type->accept(*this);
    }
    for (auto *item : expr.args) {
        item->accept(*this);
    }
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const NewExpr &expr) {
    this->on_enter(expr);
    expr.init->accept(*this);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const DeleteExpr &expr) {
    this->on_enter(expr);
    expr.arg->accept(*this);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const BracketExpr &expr) {
    this->on_enter(expr);
    expr.expr->accept(*this);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const NamedExpr &expr) {
    this->on_enter(expr);
    expr.expr->accept(*this);
    this->process(expr);
    this->on_exit(expr);
}

void PostOrderVisitor::visit(const UnknownExpr &expr) {
    this->on_enter(expr);
    this->process(expr);
    this->on_exit(expr);
}

// --------------------------------------------------
// Statements
// --------------------------------------------------

void PostOrderVisitor::visit(const TranslationUnit &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.classes) {
        item->accept(*this);
    }
    for (auto *item : stmt.interfaces) {
        item->accept(*this);
    }
    for (auto *item : stmt.functions) {
        item->accept(*this);
    }
    for (auto *item : stmt.globals) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const CompoundStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.stmts) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ReturnStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.val) {
        stmt.val->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ExprStmt &stmt) {
    this->on_enter(stmt);
    stmt.expr->accept(*this);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const IfStmt &stmt) {
    this->on_enter(stmt);
    stmt.cond->accept(*this);
    stmt.iftrue->accept(*this);
    if (stmt.iffalse) {
        stmt.iffalse->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const GotoLabelStmt &stmt) {
    this->on_enter(stmt);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const GotoStmt &stmt) {
    this->on_enter(stmt);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const CaseStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.exprs) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const DefaultCaseStmt &stmt) {
    this->on_enter(stmt);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const SwitchStmt &stmt) {
    this->on_enter(stmt);
    stmt.expr->accept(*this);
    stmt.body->accept(*this);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const WhileStmt &stmt) {
    this->on_enter(stmt);
    stmt.cond->accept(*this);
    stmt.body->accept(*this);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const DoWhileStmt &stmt) {
    this->on_enter(stmt);
    stmt.body->accept(*this);
    stmt.cond->accept(*this);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ForStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.init) {
        stmt.init->accept(*this);
    }
    if (stmt.cond) {
        stmt.cond->accept(*this);
    }
    if (stmt.step) {
        stmt.step->accept(*this);
    }
    stmt.body->accept(*this);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ForEachStmt &stmt) {
    this->on_enter(stmt);
    stmt.var->accept(*this);
    stmt.container->accept(*this);
    stmt.body->accept(*this);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ThrowStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.val) {
        stmt.val->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const CatchStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.param) {
        stmt.param->accept(*this);
    }
    stmt.body->accept(*this);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const TryStmt &stmt) {
    this->on_enter(stmt);
    stmt.body->accept(*this);
    for (auto *item : stmt.catches) {
        item->accept(*this);
    }
    if (stmt.finally) {
        stmt.finally->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const UnknownStmt &stmt) {
    this->on_enter(stmt);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const LocalVarDefStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.type) {
        stmt.type->accept(*this);
    }
    if (stmt.initializer) {
        stmt.initializer->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ParamVarDefStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.type) {
        stmt.type->accept(*this);
    }
    if (stmt.initializer) {
        stmt.initializer->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const MemberVarDefStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.type) {
        stmt.type->accept(*this);
    }
    if (stmt.initializer) {
        stmt.initializer->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const GlobalVarDefStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.type) {
        stmt.type->accept(*this);
    }
    if (stmt.initializer) {
        stmt.initializer->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const FunctionDefStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.params) {
        item->accept(*this);
    }
    if (stmt.retType) {
        stmt.retType->accept(*this);
    }
    if (stmt.body) {
        stmt.body->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const MultiVarDefStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.defs) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const MethodDefStmt &stmt) {
    this->on_enter(stmt);
    stmt.func->accept(*this);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const BaseInitializerStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.args) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const SelfInitializerStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.args) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const MemberInitializerStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.args) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ConstructorDefStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.params) {
        item->accept(*this);
    }
    for (auto *item : stmt.baseInit) {
        item->accept(*this);
    }
    for (auto *item : stmt.selfInitializers) {
        item->accept(*this);
    }
    for (auto *item : stmt.memberInitializers) {
        item->accept(*this);
    }
    if (stmt.body) {
        stmt.body->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const DestructorDefStmt &stmt) {
    this->on_enter(stmt);
    if (stmt.body) {
        stmt.body->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const GenericParam &stmt) {
    this->on_enter(stmt);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const InterfaceDefStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.methods) {
        item->accept(*this);
    }
    for (auto *item : stmt.tparams) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ClassDefStmt &stmt) {
    this->on_enter(stmt);
    for (auto *item : stmt.vars) {
        item->accept(*this);
    }
    for (auto *item : stmt.constructors) {
        item->accept(*this);
    }
    for (auto *item : stmt.destructors) {
        item->accept(*this);
    }
    for (auto *item : stmt.methods) {
        item->accept(*this);
    }
    for (auto *item : stmt.tparams) {
        item->accept(*this);
    }
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const ContinueStmt &stmt) {
    this->on_enter(stmt);
    this->process(stmt);
    this->on_exit(stmt);
}

void PostOrderVisitor::visit(const BreakStmt &stmt) {
    this->on_enter(stmt);
    this->process(stmt);
    this->on_exit(stmt);
}


} // namespace astfri
