#include <libastfri/ExpressionFactory.hpp>
#include <libastfri/impl/Utils.hpp>

namespace astfri
{
IntLiteralExpr* ExpressionFactory::mk_int_literal(const int val)
{
    return details::emplace_get<IntLiteralExpr>(val, ints_, val);
}

FloatLiteralExpr* ExpressionFactory::mk_float_literal(float val)
{
    auto* e = new FloatLiteralExpr(val);
    exprs_.push_back(e);
    return e;
}

CharLiteralExpr* ExpressionFactory::mk_char_literal(const char val)
{
    return details::emplace_get<CharLiteralExpr>(val, chars_, val);
}

StringLiteralExpr* ExpressionFactory::mk_string_literal(const std::string& val)
{
    return details::emplace_get<StringLiteralExpr>(val, strings_, val);
}

BoolLiteralExpr* ExpressionFactory::mk_bool_literal(const bool val)
{
    return val ? &true_ : &false_;
}

NullLiteralExpr* ExpressionFactory::mk_null_literal()
{
    return &null_;
}



} // namespace astfri
