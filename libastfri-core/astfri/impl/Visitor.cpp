#include <astfri/impl/Visitor.hpp>

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


} // namespace astfri
