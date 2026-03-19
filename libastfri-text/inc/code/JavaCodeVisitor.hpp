#ifndef LIBASTFRI_TEXT_JAVA_CODE_VISITOR
#define LIBASTFRI_TEXT_JAVA_CODE_VISITOR

#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

namespace astfri::text {
class JavaCodeVisitor : public AbstractCodeVisitor {
public:
    static JavaCodeVisitor& get_instance();
    JavaCodeVisitor(const JavaCodeVisitor&)            = delete;
    JavaCodeVisitor(JavaCodeVisitor&&)                 = delete;
    JavaCodeVisitor& operator=(const JavaCodeVisitor&) = delete;
    JavaCodeVisitor& operator=(JavaCodeVisitor&&)      = delete;

private:
    explicit JavaCodeVisitor();
    ~JavaCodeVisitor() = default;

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

    void visit(const LambdaExpr& /*expr*/) override {
    }

    void visit(const DeleteExpr& /*expr*/) override {
    }

    // -----
    void visit(const TranslationUnit& stmt) override;
    void visit(const MemberVarDefStmt& stmt) override;
    void visit(const GlobalVarDefStmt& stmt) override;
    void visit(const FunctionDefStmt& stmt) override;
    void visit(const DefStmt& stmt) override;
    void visit(const MethodDefStmt& stmt) override;
    void visit(const BaseInitializerStmt& stmt) override;
    void visit(const ConstructorDefStmt& stmt) override;

    void visit(const DestructorDefStmt& /*stmt*/) override {
    }

    void visit(const GenericParam& stmt) override;
    void visit(const InterfaceDefStmt& stmt) override;
    void visit(const ClassDefStmt& stmt) override;

private:
    void process_unclassified(
        const std::vector<FunctionDefStmt*>& functions,
        const std::vector<GlobalVarDefStmt*>& vars
    );
    void process_package(const Scope& scope);
    void process_access_mod(const AccessModifier& mod);
    // -----
    template<v_astfri_supertypes Vector>
    void process_supertypes(const Vector& types, bool useExtends, bool isClass);
};

//
// -----
//

template<v_astfri_supertypes Vector>
void JavaCodeVisitor::process_supertypes(const Vector& types, bool useExtends, bool isClass) {
    if (types.empty()) {
        return;
    }
    if (useExtends) {
        builder_->append_text("extends ");
        if (isClass && types.size() > 1) {
            builder_->append_text("MULTIPLE INHERITANCE NOT ALLOWED ");
            return;
        }
    }
    else {
        builder_->append_text("implements ");
    }
    for (size_t i = 0; i < types.size(); ++i) {
        // builder_->append_text(types.at(i)->name_);
        // builder_->append_text(types.at(i)->type_->name_); // TODO check
        builder_->append_text("TEMPORARY SOLUTION"); // TODO ask for change
        if (i < types.size() - 1) {
            builder_->append_text(", ");
        }
    }
    builder_->append_space();
}
} // namespace astfri::text

#endif