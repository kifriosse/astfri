#ifndef CSHARP_TS_TREE_VISITOR_HPP
#define CSHARP_TS_TREE_VISITOR_HPP

#include <libastfri-cs/impl/TypeContext.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <functional>
#include <string>

namespace astfri::csharp
{

class CSharpTSTreeVisitor
{
public:
    using ExprHandler
        = std::function<Expr*(CSharpTSTreeVisitor* self, const TSNode*)>;
    using StmtHandler
        = std::function<Stmt*(CSharpTSTreeVisitor* self, const TSNode*)>;

private:
    static ExprFactory& expr_factory_;
    static StmtFactory& stmt_factory_;
    static TypeFactory& type_factory_;

    std::string source_code_;
    const TSLanguage* language_;
    TypeContext type_context_;

public:
    CSharpTSTreeVisitor(std::string source_code, const TSLanguage* language);
    // compilation unit/translation unit
    void handle_comp_unit_stmt(TranslationUnit& tr_unit, const TSNode* node);

    /**
     * @param self instance of CSharpTSTreeVisitor
     * @param node name node of the type (for example class or interface name)
     * @return instance of a Type
     */
    static Type* make_type(const CSharpTSTreeVisitor* self, const TSNode& node);

    // Expressions
    // Literals
    static Expr* handle_int_lit(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_float_lit(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_bool_lit(CSharpTSTreeVisitor* self, const TSNode* node);
    static Expr* handle_char_lit(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_str_lit(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_null_literal(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_this_expr(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_verbatim_str_lit(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_interpolated_str_lit(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo

    // Reference Expersions
    static Expr* handle_param_var_ref_expr(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_local_var_ref_expr(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo

    // Operations
    static Expr* handle_prefix_unary_op_expr(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_postfix_unary_op_expr(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_binary_op_expr(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_ternary_expr(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // todo

    // Statements
    // Type Definitions
    static Stmt* handle_class_def_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_interface_def_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    // Variable Definitions
    static Stmt* handle_memb_var_def_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_param_def_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );

    static Stmt* handle_constr_def_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // constructor def stmt
    static Stmt* handle_base_init_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // base initializer
    static Stmt* handle_destr_def_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // destructor def stmt
    static Stmt* handle_method_def_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    ); // method def stmt

    static Stmt* handle_block_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_arrow_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_expr_stmt(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_while_loop(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_do_while_loop(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );

    Scope create_scope(const TSNode* node) const;

    Stmt* make_while_loop(const TSNode* node, bool is_do_while);

    static std::vector<ParamVarDefStmt*> handle_param_list(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
    static std::vector<Expr*> handle_argument_list(
        CSharpTSTreeVisitor* self,
        const TSNode* node
    );
};

} // namespace astfri::csharp

#endif // CSHARP_TS_TREE_VISITOR_HPP