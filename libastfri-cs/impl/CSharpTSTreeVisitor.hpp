#ifndef CSHARP_TS_TREE_VISITOR_HPP
#define CSHARP_TS_TREE_VISITOR_HPP

#include <functional>
#include <string>
#include <tree_sitter/api.h>
#include <utility>

#include "libastfri/inc/ExprFactory.hpp"
#include "libastfri/inc/StmtFactory.hpp"
#include "libastfri/inc/TypeFactory.hpp"

namespace astfri::csharp
{

class CSharpTSTreeVisitor
{
public:
    using ExprHandler = std::function<Expr*(CSharpTSTreeVisitor* self, TSNode const*)>;
    using StmtHandler = std::function<Stmt*(CSharpTSTreeVisitor* self, TSNode const*)>;
    using TypeMaker   = std::function<Type*()>;

private:
    static ExprFactory& expr_factory_;
    static StmtFactory& stmt_factory_;
    static TypeFactory& type_factory_;

    std::string source_code_;
    TSLanguage const* language_;

public:
    CSharpTSTreeVisitor(std::string source_code, TSLanguage const* language) :
        source_code_(std::move(source_code)),
        language_(language)
    {
    }

    void handle_comp_unit_stmt(TranslationUnit& tr_unit, TSNode const* node);

    static Type* make_type(CSharpTSTreeVisitor const* self, TSNode const* node);

    // Expressions
    // Literals
    static Expr* handle_int_lit(CSharpTSTreeVisitor* self, TSNode const* node);   // todo
    static Expr* handle_float_lit(CSharpTSTreeVisitor* self, TSNode const* node); // todo
    static Expr* handle_bool_lit(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_char_lit(CSharpTSTreeVisitor* self, TSNode const* node); // todo
    static Expr* handle_str_lit(CSharpTSTreeVisitor* self, TSNode const* node);  // todo
    static Expr* handle_null_literal(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_this_expr(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_verbatim_str_lit(CSharpTSTreeVisitor* self, TSNode const* node);     // todo
    static Expr* handle_interpolated_str_lit(CSharpTSTreeVisitor* self, TSNode const* node); // todo

    // Reference Expersions
    static Expr* handle_param_var_ref_expr(CSharpTSTreeVisitor* self, TSNode const* node); // todo
    static Expr* handle_local_var_ref_expr(CSharpTSTreeVisitor* self, TSNode const* node); // todo

    // Operations
    static Expr* handle_prefix_unary_op_expr(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_postfix_unary_op_expr(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_binary_op_expr(CSharpTSTreeVisitor* self, TSNode const* node); // todo
    static Expr* handle_ternary_expr(CSharpTSTreeVisitor* self, TSNode const* node);   // todo

    // Statements
    // Type Definitions
    static Stmt* handle_class_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node);
    // Variable Definitions
    static Stmt* handle_memb_var_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node); // todo
    // static Stmt* handle_param_var_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node); //todo
    // static Stmt* handle_member_var_def_stmt(CSharpTSTreeVisitor* self, TSNode const* node);
    // //todo

private:
    Scope create_scope(TSNode const* node) const;
};

} // namespace astfri::csharp

#endif // CSHARP_TS_TREE_VISITOR_HPP