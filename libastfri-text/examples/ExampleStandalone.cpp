#include <astfri/Astfri.hpp>
#include <astfri-text/TextLibManager.hpp>

#include <iostream>


astfri::TranslationUnit prepareTU();


int main(int argc, char *argv[]) {
    astfri::TranslationUnit tu = prepareTU();
    astfri::text::Config conf = argc > 1
        ? astfri::text::Config::create_from_json(argv[1])
        : astfri::text::Config::create_default();
    astfri::text::TextLibManager manager;
    manager.process_and_export_ast(tu, &std::cout);
}


astfri::TranslationUnit prepareTU() {
    auto& stmt = astfri::StmtFactory::get_instance();
    auto& type = astfri::TypeFactory::get_instance();
    auto& expr = astfri::ExprFactory::get_instance();

    astfri::TranslationUnit tu;
    astfri::FunctionDefStmt *f = stmt.mk_function_def();
    f->name = "bubble";
    f->retType = type.mk_void();
    f->params = {stmt.mk_param_var_def("xs", type.mk_class("List", {}), nullptr)};
    f->body = stmt.mk_compound({});
    f->body->stmts.push_back(
        stmt.mk_local_var_def(
            "n",
            type.mk_int(),
            expr.mk_method_call(
                expr.mk_param_var_ref("xs"),
                "length",
                {})));
    f->body->stmts.push_back(
        stmt.mk_local_var_def(
            "swapped",
            type.mk_bool(),
            expr.mk_bool_literal(false)));
    f->body->stmts.push_back(
        stmt.mk_do_while(
            expr.mk_local_var_ref("swapped"),
            stmt.mk_for(
                stmt.mk_local_var_def(
                    "i",
                    type.mk_int(),
                    expr.mk_int_literal(1)),
                expr.mk_bin_on(
                    expr.mk_local_var_ref("i"),
                    astfri::BinOpType::Less,
                    expr.mk_local_var_ref("n")),
                stmt.mk_expr(
                    expr.mk_unary_op(
                        astfri::UnaryOpType::PreIncrement,
                        expr.mk_local_var_ref("i"))),
                stmt.mk_if(
                    expr.mk_bin_on(
                        expr.mk_method_call(
                            expr.mk_local_var_ref("xs"),
                            "get",
                            {expr.mk_bin_on(expr.mk_local_var_ref("i"), astfri::BinOpType::Subtract, expr.mk_int_literal(1))}),
                        astfri::BinOpType::Greater,
                        expr.mk_method_call(
                            expr.mk_local_var_ref("xs"),
                            "get",
                            {expr.mk_local_var_ref("i")})),
                    stmt.mk_compound({
                        stmt.mk_expr(expr.mk_function_call(
                            "swap",
                            {
                                expr.mk_method_call(
                                    expr.mk_local_var_ref("xs"),
                                    "get",
                                    {expr.mk_bin_on(expr.mk_local_var_ref("i"), astfri::BinOpType::Subtract, expr.mk_int_literal(1))}),
                                expr.mk_method_call(
                                    expr.mk_local_var_ref("xs"),
                                    "get",
                                    {expr.mk_local_var_ref("i")})})),
                        stmt.mk_expr(expr.mk_bin_on(
                            expr.mk_local_var_ref("swapped"),
                            astfri::BinOpType::Assign,
                            expr.mk_bool_literal(true)))}),
                    nullptr))));
    tu.functions.push_back(f);
    return tu;
}
