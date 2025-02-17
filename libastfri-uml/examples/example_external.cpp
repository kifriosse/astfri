#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <vector>

#include <libastfri-uml/inc/ClassVisitor.hpp>
#include <libastfri-uml/inc/PlantUMLOutputter.hpp>
#include "libastfri-uml/inc/TypeConvention.hpp"
#include "libastfri/inc/Stmt.hpp"


template <typename T>
class ExampleClass
{
private:
    int number;
public:


};

int main()
{
    auto& statements = astfri::StmtFactory::get_instance();
    auto& expressions = astfri::ExprFactory::get_instance();
    auto& types = astfri::TypeFactory::get_instance();

    std::vector<astfri::ParamVarDefStmt *> params = {
        statements.mk_param_var_def("lowerBound", types.mk_int(), nullptr),
        statements.mk_param_var_def("upperBound", types.mk_int(), nullptr)
    };
    
    astfri::Expr* forCond = expressions.mk_if(
        expressions.mk_bin_on(
            expressions.mk_local_var_ref("i"),
            astfri::BinOpType::LessEqual,
            expressions.mk_param_var_ref("upperBound")
        ),
        nullptr,
        nullptr
    );
    
    astfri::Stmt* forStep = statements.mk_expr(expressions.mk_int_literal(1));

    astfri::Stmt* forInit = statements.mk_expr(
        expressions.mk_assign(
            expressions.mk_local_var_ref("i"),
            expressions.mk_param_var_ref("lowerBound")
        )
    );
      
    astfri::CompoundStmt* forBody = statements.mk_compound({
        statements.mk_if(
            expressions.mk_bin_on(
            expressions.mk_bin_on(
                expressions.mk_local_var_ref("i"),
                astfri::BinOpType::Modulo,
                expressions.mk_int_literal(2)
            ),
            astfri::BinOpType::Equal, expressions.mk_int_literal(0)
            ),
            statements.mk_expr(expressions.mk_assign(expressions.mk_local_var_ref("result"), expressions.mk_local_var_ref("i"))),
            nullptr)
    });
    
    astfri::CompoundStmt* body = statements.mk_compound({
        statements.mk_local_var_def("result", types.mk_int(), nullptr),
        statements.mk_for(forInit, forCond, forStep, forBody),
        statements.mk_return(expressions.mk_local_var_ref("result"))
    });
    
    astfri::FunctionDefStmt* func = statements.mk_function_def(
        "largest_even_number",
        std::move(params),
        types.mk_int(),
        body
    );

    auto varNumber = statements.mk_member_var_def(
        "number",
        types.mk_int(),
        nullptr,
        astfri::AccessModifier::Private
    );

    std::vector<astfri::MemberVarDefStmt*> memberVariables;
    memberVariables.push_back(varNumber);

    std::vector<astfri::MethodDefStmt*> methods;
    astfri::MethodDefStmt* method = statements.mk_method_def(nullptr, func, astfri::AccessModifier::Public);
    methods.push_back(method);
    std::vector<astfri::GenericParam*> genericParams;
    //Statement factory has no mk_generic_param
    astfri::GenericParam gp = astfri::GenericParam("", "T");
    genericParams.push_back(&gp);

    auto classDS = statements.mk_class_def(
        "ExampleClass",
        memberVariables,
        methods,
        genericParams //GenericParam is TODO
    );

    //astfri::MethodDefStmt* method = statements.mk_method_def(classDS, func);
    //methods.push_back(method);

    uml::ClassVisitor cv;
    uml::Config conf;
    uml::PlantUMLOutputter op;

    uml::TypeBeforeConvention tc;
    conf.typeConvention_ = &tc;
    conf.separator_ = ' ';

    op.set_config(&conf);
    cv.set_config(&conf);
    cv.set_outputter(&op);

    classDS->accept(cv);
    op.write_to_console();
}