#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <vector>

#include <libastfri-uml/inc/ClassVisitor.hpp>
#include <libastfri-uml/inc/PlantUMLOutputter.hpp>
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
    //auto& expressions = astfri::ExprFactory::get_instance();
    auto& types = astfri::TypeFactory::get_instance();

    auto varNumber = statements.mk_member_var_def(
        "number",
        types.mk_int(),
        nullptr
    );

    std::vector<astfri::MemberVarDefStmt*> memberVariables;
    memberVariables.push_back(varNumber);

    std::vector<astfri::MethodDefStmt*> methods;
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

    uml::ClassVisitor cv;
    uml::Config conf;
    uml::PlantUMLOutputter op;
    op.set_config(&conf);
    cv.set_config(&conf);
    cv.set_outputter(&op);

    classDS->accept(cv);
    op.write_to_console();
}