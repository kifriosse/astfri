#ifndef LIBASTFRI_STATEMENT_FACTORY_HPP
#define LIBASTFRI_STATEMENT_FACTORY_HPP

#include <libastfri/inc/Stmt.hpp>

#include <memory>
#include <vector>

namespace astfri
{
/**
 * @brief TODO
 */
class StmtFactory
{
public:
    static StmtFactory& get_instance ();

public:
    LocalVarDefStmt* mk_local_var_def (
        std::string name,
        Type* type,
        Expr* initializer
    );
    ParamVarDefStmt* mk_param_var_def (
        std::string name,
        Type* type,
        Expr* initializer
    );
    MemberVarDefStmt* mk_member_var_def (
        std::string name,
        Type* type,
        Expr* initializer,
        AccessModifier access
    );
    GlobalVarDefStmt* mk_global_var_def (
        std::string name,
        Type* type,
        Expr* initializer
    );
    FunctionDefStmt* mk_function_def (
        std::string name,
        std::vector<ParamVarDefStmt*> params,
        Type* retType,
        CompoundStmt* body
    );
    MethodDefStmt* mk_method_def (
        ClassDefStmt* owner,
        FunctionDefStmt* func,
        AccessModifier access
    );
    ConstructorDefStmt* mk_constructor_def (
        ClassDefStmt* owner,
        std::vector<ParamVarDefStmt*> params,
        std::vector<BaseInitializerStmt*> baseInit,
        CompoundStmt* body,
        AccessModifier access
    );
    BaseInitializerStmt* mak_base_initializer (
        std::string base,
        std::vector<Expr*> args
    );
    DestructorDefStmt* mk_destructor_def (
        ClassDefStmt* owner,
        CompoundStmt* body
    );
    GenericParam* mk_generic_param (std::string constraint, std::string name);
    ClassDefStmt* mk_class_def (
        std::string name,
        std::vector<MemberVarDefStmt*> vars,
        std::vector<MethodDefStmt*> methods,
        std::vector<GenericParam*> tparams
    );
    CompoundStmt* mk_compound (std::vector<Stmt*> stmts);
    ReturnStmt* mk_return (Expr* val);
    ExprStmt* mk_expr (Expr* expr);
    IfStmt* mk_if (Expr* cond, Stmt* iftrue, Stmt* iffalse);
    CaseStmt* mk_case (Expr* expr, Stmt* body);
    SwitchStmt* mk_switch (Expr* expr, std::vector<CaseStmt*> cases);
    WhileStmt* mk_while (Expr* cond, CompoundStmt* body);
    DoWhileStmt* mk_do_while (Expr* cond, CompoundStmt* body);
    ForStmt* mk_for (Stmt* init, Expr* cond, Stmt* step, CompoundStmt* body);
    ThrowStmt* mk_throw (Expr* val);
    UnknownStmt* mk_uknown ();
    TranslationUnit* mk_translation_unit (
        std::vector<ClassDefStmt*> classes,
        std::vector<FunctionDefStmt*> functions,
        std::vector<GlobalVarDefStmt*> globals
    );

private:
    std::vector<std::unique_ptr<Stmt>> stmts_;

    UnknownStmt unknown_;
};
} // namespace astfri

#endif
