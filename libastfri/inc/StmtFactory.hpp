#ifndef LIBASTFRI_STATEMENT_FACTORY_HPP
#define LIBASTFRI_STATEMENT_FACTORY_HPP

#include <libastfri/inc/Stmt.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <map>
#include <memory>
#include <vector>

namespace astfri
{

class TypeFactory;

/**
 * @brief TODO
 */
class StmtFactory
{
public:
    static StmtFactory& get_instance();

public:
    LocalVarDefStmt* mk_local_var_def(std::string name, Type* type, Expr* initializer);

    ParamVarDefStmt* mk_param_var_def(std::string name, Type* type, Expr* initializer);

    MemberVarDefStmt* mk_member_var_def(
        std::string name,
        Type* type,
        Expr* initializer,
        AccessModifier access
    );

    GlobalVarDefStmt* mk_global_var_def(std::string name, Type* type, Expr* initializer);

    DefStmt* mk_def();
    DefStmt* mk_def(std::vector<VarDefStmt*> defs);

    FunctionDefStmt* mk_function_def();
    FunctionDefStmt* mk_function_def(
        std::string name,
        std::vector<ParamVarDefStmt*> params,
        Type* retType,
        CompoundStmt* body
    );

    MethodDefStmt* mk_method_def();
    MethodDefStmt* mk_method_def(
        UserTypeDefStmt* owner,
        FunctionDefStmt* func,
        AccessModifier access,
        Virtuality virtuality
    );

    ConstructorDefStmt* mk_constructor_def();
    ConstructorDefStmt* mk_constructor_def(
        ClassDefStmt* owner,
        std::vector<ParamVarDefStmt*> params,
        std::vector<BaseInitializerStmt*> baseInit,
        CompoundStmt* body,
        AccessModifier access
    );

    /**
     * @deprecated
     */
    BaseInitializerStmt* mak_base_initializer(std::string base, std::vector<Expr*> args);

    /**
     * TODO ako so scopom base? nemal by tu byť pointer na base class type alebo def?
     */
    BaseInitializerStmt* mk_base_initializer(std::string base, std::vector<Expr*> args);

    DestructorDefStmt* mk_destructor_def(ClassDefStmt* owner, CompoundStmt* body);

    GenericParam* mk_generic_param(std::string constraint, std::string name);

    /**
     * @deprecated
     */
    InterfaceDefStmt* mk_interface_def();

    /**
     * @deprecated
     */
    InterfaceDefStmt* mk_interface_def(std::string name);

    /**
     * @brief TODO
     */
    InterfaceDefStmt* mk_interface_def(std::string name, Scope scope);

    /**
     * @brief TODO
     */
    ClassDefStmt* mk_class_def(std::string name, Scope scope);

    CompoundStmt* mk_compound(std::vector<Stmt*> stmts);

    ReturnStmt* mk_return(Expr* val);

    ExprStmt* mk_expr(Expr* expr);

    IfStmt* mk_if(Expr* cond, Stmt* iftrue, Stmt* iffalse);

    CaseStmt* mk_case(Expr* expr, Stmt* body);
    CaseStmt* mk_case(std::vector<Expr*> exprs, Stmt* body);

    DefaultCaseStmt* mk_default_case(Stmt* body);

    SwitchStmt* mk_switch(Expr* expr, std::vector<CaseBaseStmt*> cases);

    WhileStmt* mk_while(Expr* cond, Stmt* body);

    DoWhileStmt* mk_do_while(Expr* cond, Stmt* body);

    ForStmt* mk_for(Stmt* init, Expr* cond, Stmt* step, Stmt* body);

    ForEachStmt* mk_for_each(Stmt *var, Expr *container, Stmt *body);

    ThrowStmt* mk_throw(Expr* val);

    CatchStmt *mk_catch(ParamVarDefStmt *param, Stmt *body);

    TryStmt *mk_try(Stmt *body, Stmt *finally, std::vector<CatchStmt*> catches);

    ContinueStmt* mk_continue();

    BreakStmt* mk_break();

    UnknownStmt* mk_uknown();

    TranslationUnit* mk_translation_unit();

    /**
     * @deprecated
     */
    TranslationUnit* mk_translation_unit(
        std::vector<ClassDefStmt*> classes,
        std::vector<InterfaceDefStmt*> interfaces,
        std::vector<FunctionDefStmt*> functions,
        std::vector<GlobalVarDefStmt*> globals
    );

private:
    StmtFactory();
    StmtFactory(const StmtFactory &) = delete;
    StmtFactory(StmtFactory&&) = delete;
    StmtFactory& operator=(const StmtFactory &) = delete;
    StmtFactory& operator=(StmtFactory &&) = delete;

private:
    TypeFactory *m_types;
    std::vector<std::unique_ptr<Stmt>> stmts_;
    std::map<std::string, InterfaceDefStmt> m_interfaces;
    std::map<std::string, ClassDefStmt> m_classes;
    ContinueStmt continue_;
    BreakStmt break_;
    UnknownStmt unknown_;
};

} // namespace astfri

#endif
