#ifndef CSHARP_SOURCE_CODE_VISITOR_HPP
#define CSHARP_SOURCE_CODE_VISITOR_HPP

#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/visitors/TypeTranslator.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <string_view>
#include <vector>

namespace astfri::csharp
{

class SrcCodeVisitor
{
private:
    static ExprFactory& exprFact_;
    static StmtFactory& stmtFact_;
    static TypeFactory& typeFact_;

    TypeTranslator typeTrs_;
    std::vector<SourceFile>& srcCodes_;
    SemanticContext& semanticContext_;
    SourceFile* currentSrc_{nullptr};
    const TSLanguage* lang_;

public:
    SrcCodeVisitor(
        std::vector<SourceFile>& srcCodes,
        SemanticContext& semanticContext,
        SymbolTable& symbTable
    );
    // compilation unit/translation unit
    void visit_comp_unit(TranslationUnit& trUnit);

    // Expressions
    // Literals
    // todo
    static Expr* visit_int_lit(SrcCodeVisitor* self, const TSNode& node);
    // todo
    static Expr* visit_float_lit(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_bool_lit(SrcCodeVisitor* self, const TSNode& node);
    // todo
    static Expr* visit_char_lit(SrcCodeVisitor* self, const TSNode& node);
    // todo
    static Expr* visit_str_lit(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_null_lit(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_this_expr(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_verbatim_str_lit(
        SrcCodeVisitor* self,
        const TSNode& node
    );
    static Expr* visit_raw_str_lit(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_interpolated_str_lit(
        SrcCodeVisitor* self,
        const TSNode& node
    ); // todo

    // Reference Expersions
    static Expr* visit_identifier(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_memb_access_expr(
        SrcCodeVisitor* self,
        const TSNode& node
    );
    static Expr* visit_invoc_expr(SrcCodeVisitor* self, const TSNode& node);

    // Operations
    static Expr* visit_prefix_unary_op_expr(
        SrcCodeVisitor* self,
        const TSNode& node
    );
    static Expr* visit_postfix_unary_op_expr(
        SrcCodeVisitor* self,
        const TSNode& node
    );
    static Expr* visit_ref_expr(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_binary_op_expr(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_ternary_expr(SrcCodeVisitor* self, const TSNode& node);

    static Expr* visit_parenthesized_expr(
        SrcCodeVisitor* self,
        const TSNode& node
    );

    // switch patterns
    static Expr* visit_const_pattern(SrcCodeVisitor* self, const TSNode& node);

    // Statements
    // Type Definitions
    static Stmt* visit_class_def_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_interface_def_stmt(
        SrcCodeVisitor* self,
        const TSNode& node
    );
    // Variable Definitions
    static Stmt* visit_memb_var_def_stmt(
        SrcCodeVisitor* self,
        const TSNode& node
    );
    static Stmt* visit_local_var_def_stmt(
        SrcCodeVisitor* self,
        const TSNode& node
    );
    static Stmt* visit_global_var_def_stmt(
        SrcCodeVisitor* self,
        const TSNode& node
    );
    static Stmt* visit_param_def_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_constr_def_stmt(
        SrcCodeVisitor* self,
        const TSNode& node
    ); // constructor def stmt
    static Stmt* visit_construct_init(
        SrcCodeVisitor* self,
        const TSNode& node
    ); // base initializer
    static Stmt* visit_destr_def_stmt(
        SrcCodeVisitor* self,
        const TSNode& node
    ); // destructor def stmt
    static Stmt* visit_method_def_stmt(
        SrcCodeVisitor* self,
        const TSNode& node
    ); // method def stmt

    static Stmt* visit_block_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_arrow_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_func_stmt(SrcCodeVisitor* self, const TSNode& node);

    // loops
    static Stmt* visit_while_loop(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_do_while_loop(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_for_loop(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_for_each_loop(SrcCodeVisitor* self, const TSNode& node);

    // branching statements
    static Stmt* visit_if_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_try_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_catch_clause(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_finally(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_catch_decl(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_switch_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_case_stmt(SrcCodeVisitor* self, const TSNode& node);

    // other
    static Stmt* visit_expr_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_continue(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_break(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_return(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_throw(SrcCodeVisitor* self, const TSNode& node);

private:
    Stmt* visit_var_def_stmt(
        const TSNode& node,
        util::VarDefType defType
    ); // general var def stmt handler

    Stmt* make_while_loop(const TSNode& node, bool isWhile);
    FunctionDefStmt* make_func_stmt(
        const TSNode& node,
        bool isMethod
    ); // todo move this

    /**
     * Turns an expression list into a chained comma operator expression
     * @param nStart node from which to start
     * @param nEnd node at which to end (exclusive). If nullptr, ends when
     * there aren't other siblings
     * @return returns the chained comma operator expression. If there is only
     * one expression, that expression is returned. If start and end are the
     * same, nullptr is returned.
     */
    Expr* expr_list_to_comma_op(const TSNode& nStart, const TSNode* nEnd);

    /**
     * Makes a list of parameter variable definition statements from the given
     * parameter list node
     * @param node parameter list node
     * @param makeShallow if true, makes shallow parameter definitions (without
     * default values), if false, makes full parameter definitions
     * @return vector of parameter variable definition statements
     */
    std::vector<ParamVarDefStmt*> make_param_list(
        const TSNode& node,
        bool makeShallow
    );
    std::vector<Expr*> visit_arg_list(const TSNode& node);
    Stmt* visit_for_init_var_def(const TSNode& node);

private:
    [[nodiscard]] std::string_view src_str() const;
    [[nodiscard]] SourceFile* src() const;
};

} // namespace astfri::csharp

#endif // CSHARP_SOURCE_CODE_VISITOR_HPP
