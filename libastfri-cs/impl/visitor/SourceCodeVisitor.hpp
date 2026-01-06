#ifndef CSHARP_SOURCE_CODE_VISITOR_HPP
#define CSHARP_SOURCE_CODE_VISITOR_HPP

#include <libastfri-cs/impl/data/Source.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <string_view>
#include <vector>

namespace astfri::csharp
{

class SourceCodeVisitor
{
private:
    static ExprFactory& expr_factory_;
    static StmtFactory& stmt_factory_;
    static TypeFactory& type_factory_;

    std::vector<SourceCode>& src_codes;
    SemanticContext& semantic_context_;
    SourceCode* current_src{nullptr};

public:
    SourceCodeVisitor(
        std::vector<SourceCode>& source_codes,
        SemanticContext& semantic_context
    );
    // compilation unit/translation unit
    void handle_comp_unit_stmt(TranslationUnit& tr_unit);

    // Expressions
    // Literals
    static Expr* handle_int_lit(
        SourceCodeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_float_lit(
        SourceCodeVisitor* self,
        const TSNode* node
    ); // todo
    static Expr* handle_bool_lit(SourceCodeVisitor* self, const TSNode* node);
    // todo
    static Expr* handle_char_lit(SourceCodeVisitor* self, const TSNode* node);
    // todo
    static Expr* handle_str_lit(SourceCodeVisitor* self, const TSNode* node);
    static Expr* handle_null_lit(SourceCodeVisitor* self, const TSNode* node);
    static Expr* handle_this_expr(SourceCodeVisitor* self, const TSNode* node);
    static Expr* handle_verbatim_str_lit(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_raw_str_lit(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_interpolated_str_lit(
        SourceCodeVisitor* self,
        const TSNode* node
    ); // todo

    // Reference Expersions
    static Expr* handle_identifier(SourceCodeVisitor* self, const TSNode* node);
    static Expr* handle_memb_access_expr(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_invoc_expr(SourceCodeVisitor* self, const TSNode* node);

    // Operations
    static Expr* handle_prefix_unary_op_expr(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_postfix_unary_op_expr(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_binary_op_expr(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Expr* handle_ternary_expr(
        SourceCodeVisitor* self,
        const TSNode* node
    );

    static Expr* handle_parenthesized_expr(
        SourceCodeVisitor* self,
        const TSNode* node
    );

    // switch patterns
    static Expr* handle_const_pattern(
        SourceCodeVisitor* self,
        const TSNode* node
    );

    // Statements
    // Type Definitions
    static Stmt* handle_class_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_interface_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    // Variable Definitions
    static Stmt* handle_memb_var_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_local_var_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_global_var_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_var_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node,
        util::VarDefType def_type
    ); // general var def stmt handler
    static Stmt* handle_param_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_constr_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    ); // constructor def stmt
    static Stmt* handle_construct_init(
        SourceCodeVisitor* self,
        const TSNode* node
    ); // base initializer
    static Stmt* handle_destr_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    ); // destructor def stmt
    static Stmt* handle_method_def_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    ); // method def stmt

    static Stmt* handle_block_stmt(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_arrow_stmt(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_local_func_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static FunctionDefStmt* handle_function_stmt(
        SourceCodeVisitor* self,
        const TSNode* node,
        bool is_method
    );

    // loops
    static Stmt* handle_while_loop(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_do_while_loop(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_for_loop(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_for_each_loop(
        SourceCodeVisitor* self,
        const TSNode* node
    );

    // branching statements
    static Stmt* handle_if_stmt(SourceCodeVisitor* self, const TSNode* node);

    static Stmt* handle_try_stmt(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_catch_clause(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_finally_clause(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_catch_decl(SourceCodeVisitor* self, const TSNode* node);

    static Stmt* handle_switch_stmt(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_case_stmt(SourceCodeVisitor* self, const TSNode* node);

    // other
    static Stmt* handle_expr_stmt(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_continue(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_break(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_return(SourceCodeVisitor* self, const TSNode* node);
    static Stmt* handle_throw(SourceCodeVisitor* self, const TSNode* node);

private:
    Stmt* make_while_loop(const TSNode* node, bool is_do_while);

    /**
     * Turns an expression list into a chained comma operator expression
     * @param start_node node from which to start
     * @param end_node node at which to end (exclusive). If nullptr, ends when
     * there aren't other siblings
     * @return returns the chained comma operator expression. If there is only
     * one expression, that expression is returned. If start and end are the
     * same, nullptr is returned.
     */
    Expr* expr_list_to_comma_op(
        const TSNode& start_node,
        const TSNode* end_node
    );

    /**
     * Makes a list of parameter variable definition statements from the given
     * parameter list node
     * @param self instance of SourceCodeVisitor
     * @param node parameter list node
     * @param make_shallow if true, makes shallow parameter definitions (without
     * default values), if false, makes full parameter definitions
     * @return vector of parameter variable definition statements
     */
    static std::vector<ParamVarDefStmt*> make_param_list(
        SourceCodeVisitor* self,
        const TSNode* node,
        bool make_shallow
    );
    static std::vector<Expr*> handle_argument_list(
        SourceCodeVisitor* self,
        const TSNode* node
    );
    static Stmt* handle_for_init_var_def(
        SourceCodeVisitor* self,
        const TSNode* node
    );

private:
    [[nodiscard]] std::string_view get_src_code() const;
    [[nodiscard]] const TSLanguage* get_lang() const;
};

} // namespace astfri::csharp

#endif // CSHARP_SOURCE_CODE_VISITOR_HPP