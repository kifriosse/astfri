#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <libastfri-uml/inc/UMLLibWrapper.hpp>
#include "libastfri/inc/Stmt.hpp"

int main()
{
    auto& statements = astfri::StmtFactory::get_instance();
    //auto& expressions = astfri::ExprFactory::get_instance();
    auto& types = astfri::TypeFactory::get_instance();

    astfri::ClassDefStmt* classFoo = statements.mk_class_def("Foo");
    astfri::ClassDefStmt* classBar = statements.mk_class_def("Bar");
    astfri::InterfaceDefStmt* interfaceIVisitable = statements.mk_interface_def("IVisitable");

    std::vector<astfri::GenericParam*> genericParamsFoo;
    genericParamsFoo.push_back(statements.mk_generic_param("", "T"));
    genericParamsFoo.push_back(statements.mk_generic_param("", "E"));

    std::vector<astfri::MemberVarDefStmt*> fieldsFoo;
    fieldsFoo.push_back(statements.mk_member_var_def(
        "number_",
        types.mk_int(),
        nullptr,
        astfri::AccessModifier::Private)
    );
    fieldsFoo.push_back(statements.mk_member_var_def(
        "bar_",
        types.mk_indirect(types.mk_user("Bar")),
        nullptr,
        astfri::AccessModifier::Private)
    );
    classFoo->vars_ = fieldsFoo;

    auto func = statements.mk_function_def();
    func->name_ = "foo";
    func->retType_ = types.mk_indirect(types.mk_user("RetType"));
    std::vector<astfri::ParamVarDefStmt*> params;
    params.push_back(statements.mk_param_var_def("number", types.mk_int(), nullptr));
    func->params_ = params;

    std::vector<astfri::MethodDefStmt*> methodsFoo;
    auto methodFooOne = statements.mk_method_def();
    methodFooOne->access_ = astfri::AccessModifier::Public;
    methodFooOne->owner_ = (classFoo);
    methodFooOne->virtuality_ = astfri::Virtuality::NotVirtual;
    methodFooOne->func_ = func;
    methodsFoo.push_back(methodFooOne);
    classFoo->methods_ = methodsFoo;

    astfri::TranslationUnit tu;
    std::vector<astfri::ClassDefStmt*> classes;
    classes.push_back(classFoo);
    classes.push_back(classBar);
    std::vector<astfri::InterfaceDefStmt*> interfaces;
    interfaces.push_back(interfaceIVisitable);

    tu.classes_ = classes;
    tu.interfaces_ = interfaces;

    astfri::uml::Config conf;
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::TypeBeforeConvention tc;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op, tc);
    uml.run(tu);
}