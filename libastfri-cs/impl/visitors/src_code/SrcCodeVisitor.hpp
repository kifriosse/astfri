#ifndef CSHARP_SOURCE_CODE_VISITOR_HPP
#define CSHARP_SOURCE_CODE_VISITOR_HPP

#include <libastfri-cs/impl/data/SourceFile.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/visitors/TypeTranslator.hpp>
#include <astfri/Astfri.hpp>

#include <tree_sitter/api.h>

#include <string_view>
#include <vector>

namespace astfri::csharp {

/**
 * @brief Visitor class for constructing AST from tree-sitter tree.
 */
class SrcCodeVisitor {
private:
    friend maps::Mappers;

    static ExprFactory& exprFact_;
    static StmtFactory& stmtFact_;
    static TypeFactory& typeFact_;

    TypeTranslator typeTrs_;
    SemanticContext& semContext_;
    SourceFile* currentSrc_{nullptr};
    const TSLanguage* lang_;

public:
    /**
     * @brief Constructor for SrcCodeVisitor
     * @param semanticContext semantic context of the project
     * @param symbTable symbol table of the project (should be same as the used
     * inside semantic context)
     */
    SrcCodeVisitor(SemanticContext& semanticContext, SymbolTable& symbTable);
    /**
     * @brief Visits the root node of a tree-sitter tree and fills up
     * TranslationUnit argument with top level definitions and statements.
     * @param trUnit
     */
    void visit_comp_unit(TranslationUnit& trUnit);

private:
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
    static Expr* visit_base_expr(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_verbatim_str_lit(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_raw_str_lit(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_interpolated_str_lit(SrcCodeVisitor* self,
                                            const TSNode& node); // todo

    // Reference Expersions
    static Expr* visit_identifier(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_memb_access(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_invoc(SrcCodeVisitor* self, const TSNode& node);

    // Operations
    static Expr* visit_prefix_unary_opr(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_postfix_unary_opr(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_ref_expr(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_binary_opr(SrcCodeVisitor* self, const TSNode& node);
    static Expr* visit_ternary_expr(SrcCodeVisitor* self, const TSNode& node);

    static Expr* visit_parenthesized_expr(SrcCodeVisitor* self, const TSNode& node);

    // switch patterns
    static Expr* visit_const_pattern(SrcCodeVisitor* self, const TSNode& node);

    // Statements
    // Type Definitions
    static Stmt* visit_class_def(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_interface_def(SrcCodeVisitor* self, const TSNode& node);
    // Variable Definitions
    static Stmt* visit_memb_var_def(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_local_var_def(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_global_var_def_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_param_def(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_constr_def(SrcCodeVisitor* self,
                                  const TSNode& node); // constructor def stmt
    static Stmt* visit_constr_init(SrcCodeVisitor* self,
                                   const TSNode& node); // base initializer
    static Stmt* visit_destr_def(SrcCodeVisitor* self,
                                 const TSNode& node); // destructor def stmt
    static Stmt* visit_method_def(SrcCodeVisitor* self,
                                  const TSNode& node); // method def stmt

    static Stmt* visit_block(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_arrow_body(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_func_stmt(SrcCodeVisitor* self, const TSNode& node);

    // loops
    static Stmt* visit_while(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_do_while(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_for_loop(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_for_each(SrcCodeVisitor* self, const TSNode& node);

    // branching statements
    static Stmt* visit_if(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_try(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_catch(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_finally(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_catch_decl(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_switch(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_case_stmt(SrcCodeVisitor* self, const TSNode& node);

    // other
    static Stmt* visit_expr_stmt(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_continue(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_break(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_return(SrcCodeVisitor* self, const TSNode& node);
    static Stmt* visit_throw(SrcCodeVisitor* self, const TSNode& node);

    /**
     * @brief General visit function for variable definition statements.
     * @param node varaible definition node
     * @param defType type of variable definition
     * @return variable definition statement.
     */
    Stmt* visit_var_def_stmt(const TSNode& node, util::VarDefType defType);
    /**
     * @brief General visit function for while and do-while loops
     * @param node loop node
     * @param isWhile true if it's a while loop, false if it's a do-while loop
     * @return while or do-while statement depending on \code isWhile\endcode
     * parameter
     */
    Stmt* make_while_loop(const TSNode& node, bool isWhile);
    FunctionDefStmt* make_func_stmt(const TSNode& node, bool isMethod);

    /**
     * @brief Turns an expression list into a chained comma operator expression
     * @param nStart node from which to start
     * @param nEnd node at which to end (exclusive). If nullptr, ends when
     * there aren't other siblings
     * @return returns the chained comma operator expression. If there is only
     * one expression, that expression is returned. If start and end are the
     * same, nullptr is returned.
     */
    Expr* expr_list_to_comma_op(const TSNode& nStart, const TSNode* nEnd);

    /**
     * @brief Makes a list of parameter variable definition statements from the
     * given parameter list node
     * @param node parameter list node
     * @param makeShallow if true, makes shallow parameter definitions (without
     * default values), if false, makes full parameter definitions
     * @return vector of parameter variable definition statements
     */
    std::vector<ParamVarDefStmt*> make_param_list(const TSNode& node, bool makeShallow);
    /**
     * @brief Makes a list of parameter defintion expressions from the given
     * argument list node
     * @param node argument list node
     * @return vector of parameter defintion expressions
     */
    std::vector<Expr*> visit_arg_list(const TSNode& node);
    /**
     * @brief Visit function for for-loop initializer variable definition.
     * @param node for-loop initializer variable definition node (first part of
     * for-loop)
     * @return variable definition statement or chained comma operator
     * expression
     */
    Stmt* visit_for_init_var_def(const TSNode& node);

    /**
     * @brief Gets the source code currently being visited.
     * @return string view of current source code
     * @throws std::logic_error if source file is not set
     */
    [[nodiscard]] std::string_view src_str() const;
    /**
     * @brief Gets the source file currently being visited.
     * @return pointer to current source file
     * @throws std::logic_error if source file is not set
     */
    [[nodiscard]] SourceFile* src() const;
};

} // namespace astfri::csharp

#endif // CSHARP_SOURCE_CODE_VISITOR_HPP
