#ifndef LIBASTFRI_TEXT_CXX_CODE_VISITOR
#define LIBASTFRI_TEXT_CXX_CODE_VISITOR

#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

namespace astfri::text {
class CxxCodeVisitor : public AbstractCodeVisitor {
public:
    static CxxCodeVisitor& get_instance();
    CxxCodeVisitor(const CxxCodeVisitor&)            = delete;
    CxxCodeVisitor(CxxCodeVisitor&&)                 = delete;
    CxxCodeVisitor& operator=(const CxxCodeVisitor&) = delete;
    CxxCodeVisitor& operator=(CxxCodeVisitor&&)      = delete;

private:
    explicit CxxCodeVisitor();
    ~CxxCodeVisitor() = default;

public:
    void visit(const DynamicType& type) override;
    void visit(const FloatType& type) override;
    void visit(const BoolType& type) override;
    void visit(const IndirectionType& type) override;
    void visit(const LambdaType& type) override;
    // -----
    void visit(const NullLiteralExpr& expr) override;
    void visit(const MemberVarRefExpr& expr) override;
    void visit(const GlobalVarRefExpr& expr) override;
    void visit(const FunctionCallExpr& expr) override;
    void visit(const MethodCallExpr& expr) override;
    void visit(const LambdaCallExpr& expr) override;
    void visit(const LambdaExpr& expr) override;
    void visit(const DeleteExpr& expr) override;
    // -----
    void visit(const TranslationUnit& stmt) override;
    void visit(const MemberVarDefStmt& stmt) override;
    void visit(const GlobalVarDefStmt& stmt) override;
    void visit(const FunctionDefStmt& stmt) override;
    void visit(const DefStmt& stmt) override;
    void visit(const MethodDefStmt& stmt) override;
    void visit(const BaseInitializerStmt& stmt) override;
    void visit(const ConstructorDefStmt& stmt) override;
    void visit(const DestructorDefStmt& stmt) override;
    void visit(const GenericParam& stmt) override;
    void visit(const InterfaceDefStmt& stmt) override;
    void visit(const ClassDefStmt& stmt) override;
};
} // namespace astfri::text

#endif