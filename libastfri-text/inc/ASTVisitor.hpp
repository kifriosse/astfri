#ifndef LIBASTFRI_TEXT_INC_AST_VISITOR
#define LIBASTFRI_TEXT_INC_AST_VISITOR

#include <libastfri/inc/Stmt.hpp>
#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <libastfri-text/inc/HtmlFileExporter.hpp>

using namespace astfri;

class ASTVisitor : public IVisitor {
    std::shared_ptr<TextConfigurator> configurator_;
    std::unique_ptr<Exporter> exporter_;
    std::unique_ptr<std::stringstream> currClassName_;
    std::unique_ptr<std::stringstream> currInterfName_;
    std::unique_ptr<std::vector<std::stringstream>> currGenParams_;
    bool isMethodCall_;
    bool isConstructorCall_;
public:
    ASTVisitor();
    void write_file();
public:
    void visit(DynamicType         const& type) override;
    void visit(IntType             const& type) override;
    void visit(FloatType           const& type) override;
    void visit(CharType            const& type) override;
    void visit(BoolType            const& type) override;
    void visit(VoidType            const& type) override;
    void visit(UserType            const& type) override;
    void visit(IndirectionType     const& type) override;
    void visit(UnknownType         const& type) override;
    void visit(IntLiteralExpr      const& expr) override;
    void visit(FloatLiteralExpr    const& expr) override;
    void visit(CharLiteralExpr     const& expr) override;
    void visit(StringLiteralExpr   const& expr) override;
    void visit(BoolLiteralExpr     const& expr) override;
    void visit(NullLiteralExpr     const& expr) override;
    void visit(IfExpr              const& expr) override;
    void visit(BinOpExpr           const& expr) override;
    void visit(UnaryOpExpr         const& expr) override;
    void visit(ParamVarRefExpr     const& expr) override;
    void visit(LocalVarRefExpr     const& expr) override;
    void visit(MemberVarRefExpr    const& expr) override;
    void visit(GlobalVarRefExpr    const& expr) override;
    void visit(ClassRefExpr        const& expr) override;
    void visit(FunctionCallExpr    const& expr) override;
    void visit(MethodCallExpr      const& expr) override;
    void visit(LambdaExpr          const& expr) override;
    void visit(ThisExpr            const& expr) override;
    void visit(ConstructorCallExpr const& expr) override;
    void visit(NewExpr             const& expr) override;
    void visit(DeleteExpr          const& expr) override;
    void visit(UnknownExpr         const& expr) override;
    void visit(TranslationUnit     const& stmt) override;
    void visit(CompoundStmt        const& stmt) override;
    void visit(ReturnStmt          const& stmt) override;
    void visit(ExprStmt            const& stmt) override;
    void visit(IfStmt              const& stmt) override;
    void visit(CaseStmt            const& stmt) override;
    void visit(DefaultCaseStmt     const& stmt) override;
    void visit(SwitchStmt          const& stmt) override;
    void visit(WhileStmt           const& stmt) override;
    void visit(DoWhileStmt         const& stmt) override;
    void visit(ForStmt             const& stmt) override;
    void visit(ThrowStmt           const& stmt) override;
    void visit(UnknownStmt         const& stmt) override;
    void visit(LocalVarDefStmt     const& stmt) override;
    void visit(ParamVarDefStmt     const& stmt) override;
    void visit(MemberVarDefStmt    const& stmt) override;
    void visit(GlobalVarDefStmt    const& stmt) override;
    void visit(FunctionDefStmt     const& stmt) override;
    void visit(DefStmt             const& stmt) override;
    void visit(MethodDefStmt       const& stmt) override;
    void visit(BaseInitializerStmt const& stmt) override;
    void visit(ConstructorDefStmt  const& stmt) override;
    void visit(DestructorDefStmt   const& stmt) override;
    void visit(GenericParam        const& stmt) override;
    void visit(InterfaceDefStmt    const& stmt) override;
    void visit(ClassDefStmt        const& stmt) override;
    void visit(ContinueStmt        const& stmt) override;
    void visit(BreakStmt           const& stmt) override;
private:
    void write_open_curl_bracket();
    void write_arrow();
    void write_initialization(const VarDefStmt* init);
    void write_member_vars(std::vector<MemberVarDefStmt*>& vars);
    void write_constructors(std::vector<ConstructorDefStmt*>& constr);
    void write_methods(std::vector<MethodDefStmt*>& meth);
    void write_return_type(Type* type);
    void write_body(Stmt* body);
    void write_cond(Expr* cond);
    void write_gen_params(const std::vector<GenericParam*>& params);
    void write_implementations(const std::vector<InterfaceDefStmt*>& interfaces);
    template<typename Parameter>
    void write_parameters(const std::vector<Parameter*>& params);
    template<typename Member>
    bool has_acc_mod(const std::vector<Member*>& stmts, std::vector<Member*>& found, AccessModifier acc);
};

template<typename Parameter>
void ASTVisitor::write_parameters(const std::vector<Parameter*>& params) {
    exporter_->write_round_bracket(std::move("("));
    for (size_t i = 0; i < params.size(); ++i) {
        params.at(i) ? params.at(i)->accept(*this) : exporter_->write_invalid_word();
        (i < params.size() - 1) ? (exporter_->write_separator_sign(std::move(",")), exporter_->write_space()) : void();
    }
    exporter_->write_round_bracket(std::move(")"));
}

template<typename Member>
bool ASTVisitor::has_acc_mod(const std::vector<Member*>& stmts, std::vector<Member*>& found, AccessModifier acc) {
    bool hasMod = false;
    for (size_t i = 0; i < stmts.size(); ++i) {
        if (stmts.at(i)->access_ == acc) {
            found.push_back(stmts.at(i));
            hasMod = true;
        }
    }
    return hasMod;
}

#endif