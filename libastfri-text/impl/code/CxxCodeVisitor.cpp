#include <libastfri-text/inc/code/CxxCodeBuilder.hpp>
#include <libastfri-text/inc/code/CxxCodeVisitor.hpp>

using namespace astfri::text;

CxxCodeVisitor& CxxCodeVisitor::get_instance() {
    static CxxCodeVisitor visitor;
    return visitor;
}

CxxCodeVisitor::CxxCodeVisitor() :
    AbstractCodeVisitor(CxxCodeBuilder::get_instatnce()) {
}

//
// -----
//

void CxxCodeVisitor::visit(const DynamicType& /*type*/) {
}

void CxxCodeVisitor::visit(const FloatType& /*type*/) {
}

void CxxCodeVisitor::visit(const BoolType& /*type*/) {
}

void CxxCodeVisitor::visit(const IndirectionType& /*type*/) {
}

void CxxCodeVisitor::visit(const LambdaType& /*type*/) {
}

// -----

void CxxCodeVisitor::visit(const NullLiteralExpr& /*expr*/) {
}

void CxxCodeVisitor::visit(const MemberVarRefExpr& /*expr*/) {
}

void CxxCodeVisitor::visit(const GlobalVarRefExpr& /*expr*/) {
}

void CxxCodeVisitor::visit(const FunctionCallExpr& /*expr*/) {
}

void CxxCodeVisitor::visit(const MethodCallExpr& /*expr*/) {
}

void CxxCodeVisitor::visit(const LambdaCallExpr& /*expr*/) {
}

void CxxCodeVisitor::visit(const LambdaExpr& /*expr*/) {
}

void CxxCodeVisitor::visit(const DeleteExpr& /*expr*/) {
}

// -----

void CxxCodeVisitor::visit(const TranslationUnit& /*stmt*/) {
}

void CxxCodeVisitor::visit(const MemberVarDefStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const GlobalVarDefStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const FunctionDefStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const DefStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const MethodDefStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const BaseInitializerStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const ConstructorDefStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const DestructorDefStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const GenericParam& /*stmt*/) {
}

void CxxCodeVisitor::visit(const InterfaceDefStmt& /*stmt*/) {
}

void CxxCodeVisitor::visit(const ClassDefStmt& /*stmt*/) {
}