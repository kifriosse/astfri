#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

namespace my_ast_trees
{
    template <typename TreeVisitor>
    void load_ast_tree_1(TreeVisitor& /*tv*/) {
        // AST
    }

    template <typename TreeVisitor>
    void load_ast_tree_2(TreeVisitor& tv) {
        //class TestClass {
        //private:
        //    int a;
        //    int b = 1;
        //    std::string s = "textik";
        //public:
        //    TestClass(int cislo1, int cislo2 = 5) : a(cislo1), b(cislo2) {}
        //    ~TestClass();
        //    int getCislo() const { return a * b; }
        //};
        astfri::ExprFactory& expressions = astfri::ExprFactory::get_instance();
        astfri::StmtFactory& statements = astfri::StmtFactory::get_instance();
        astfri::TypeFactory& types = astfri::TypeFactory::get_instance();
        astfri::ClassDefStmt* cds = statements.mk_class_def("TestClass");
        std::vector<astfri::MemberVarDefStmt*> atributes{
            statements.mk_member_var_def("a", types.mk_int(), nullptr, astfri::AccessModifier::Protected),
            statements.mk_member_var_def("b", types.mk_int(), expressions.mk_int_literal(1), astfri::AccessModifier::Private),
            statements.mk_member_var_def("s", types.mk_user("string"), expressions.mk_string_literal("textik"), astfri::AccessModifier::Public)
        };
        std::vector<astfri::ParamVarDefStmt*> constructorParams{
            statements.mk_param_var_def("cislo1", types.mk_int(), nullptr),
            statements.mk_param_var_def("cislo2", types.mk_int(), expressions.mk_int_literal(5))
        };
        // std::vector<astfri::MethodDefStmt*> methods{
        //     statements.mk_method_def(cds, statements.mk_function_def(cds->name_, constructorParams, types.mk_user(cds->name_), statements.mk_compound({
        //         statements.mk_expr(expressions.mk_assign(expressions.mk_member_var_ref("a"), expressions.mk_param_var_ref("cislo1"))),
        //         statements.mk_expr(expressions.mk_assign(expressions.mk_member_var_ref("b"), expressions.mk_param_var_ref("cislo2")))
        //     })), astfri::AccessModifier::Public),
        //     statements.mk_method_def(cds, statements.mk_function_def("~TestClass", {}, types.mk_unknown(), {}), astfri::AccessModifier::Public),
        //     statements.mk_method_def(cds, statements.mk_function_def("getCislo", {}, types.mk_int(), statements.mk_compound({
        //         statements.mk_return(expressions.mk_bin_on(expressions.mk_member_var_ref("a"), astfri::BinOpType::Multiply, expressions.mk_member_var_ref("b")))
        //     })), astfri::AccessModifier::Private)
        // };
        cds = statements.mk_class_def(cds->name_);
        cds->vars_ = atributes;
        cds->accept(tv);
    }

    template <typename TreeVisitor>
    void load_ast_tree_3(TreeVisitor& /*tv*/) {
        // AST
    }
}