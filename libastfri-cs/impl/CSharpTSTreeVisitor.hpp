#ifndef CSHARP_TSTREE_VISITOR_HPP
#define CSHARP_TSTREE_VISITOR_HPP
#include <libastfri/impl/StmtFwd.hpp>

#include <functional>
#include <string>
#include <tree_sitter/api.h>
#include <unordered_map>
#include <utility>

#include "libastfri/inc/ExprFactory.hpp"

namespace astfri::csharp
{

class CSharpTSTreeVisitor
{
public:
    using ExprHandler = std::function<Expr*(CSharpTSTreeVisitor *self, const TSNode*)>;
    using StmtHandler = std::function<Stmt*(CSharpTSTreeVisitor *self, const TSNode*)>;
private:
    using ExprFactory = astfri::ExprFactory;

    std::string source_code_;
    std::unordered_map<std::string, StmtHandler> stmt_handlers_;
    std::unordered_map<std::string, ExprHandler> expr_handlers_ = {
        {"integer_literal", handle_int_literal},
        {"real_literal", handle_float_literal},
        {"boolean_literal", handle_bool_literal},
        {"character_literal", handle_char_literal},
        {"string_literal", handle_string_literal},
        {"null_literal", handle_null_literal},
        {"verbatim_string_literal", handle_verbatim_string_literal},
        {"this_expression", handle_this_expr},
    };
public:
    explicit CSharpTSTreeVisitor(std::string  source_code)
        : source_code_(std::move(source_code))
    { }

    Expr* handler_expr(const TSNode* node);
    static Expr* handle_int_literal(CSharpTSTreeVisitor *self, const TSNode* node); //todo
    static Expr* handle_float_literal(CSharpTSTreeVisitor *self, const TSNode* node); //todo
    static Expr* handle_bool_literal(CSharpTSTreeVisitor *self, const TSNode* node);
    static Expr* handle_char_literal(CSharpTSTreeVisitor *self, const TSNode* node); //todo
    static Expr* handle_string_literal(CSharpTSTreeVisitor *self, const TSNode* node); //todo
    static Expr* handle_null_literal(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_this_expr(CSharpTSTreeVisitor* self, TSNode const* node);
    static Expr* handle_verbatim_string_literal(CSharpTSTreeVisitor* self, TSNode const* node); //todo
    static Expr* handle_interpolated_string_literal(CSharpTSTreeVisitor* self, TSNode const* node); //todo

private:
    static std::string extract_node_text(TSNode const& node, const std::string& source_code);
};

} // namespace astfri

#endif // CSHARP_TSTREE_VISITOR_HPP