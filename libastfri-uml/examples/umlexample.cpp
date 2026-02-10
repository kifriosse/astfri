#include <libastfri-uml/inc/UMLLibWrapper.hpp>

#include <vector>

int main(int argc, char** argv)
{
    auto& statements = astfri::StmtFactory::get_instance();
    // auto& expressions = astfri::ExprFactory::get_instance();
    auto& types                                   = astfri::TypeFactory::get_instance();

    astfri::Scope scope = astfri::mk_scope("Global");

    astfri::ClassDefStmt* classFoo                = statements.mk_class_def("Foo", scope);
    astfri::ClassDefStmt* classBar                = statements.mk_class_def("Bar", scope);
    astfri::ClassDefStmt* classParent             = statements.mk_class_def("Parent", scope);
    astfri::InterfaceDefStmt* interfaceIVisitable = statements.mk_interface_def("IVisitable", scope);

    astfri::ClassType* classTypeFoo = types.mk_class("Foo", scope);
    astfri::ClassType* classTypeBar = types.mk_class("Bar", scope);
    astfri::ClassType* classTypeParent = types.mk_class("Parent", scope);
    //astfri::InterfaceType* interfaceTypeIVisitable = types.mk_interface("IVisitable", scope);

    classFoo->type_ = classTypeFoo;
    classBar->type_ = classTypeBar;
    classParent->type_ = classTypeParent;
    //interfaceIVisitable->type_ = interfaceTypeIVisitable;

    std::vector<astfri::GenericParam*> genericParamsFoo;
    genericParamsFoo.push_back(statements.mk_generic_param("", "T"));
    genericParamsFoo.push_back(statements.mk_generic_param("", "E"));
    classFoo->tparams_ = genericParamsFoo;

    std::vector<astfri::ClassDefStmt*> basesBar;
    basesBar.push_back(classParent);
    classBar->bases_ = basesBar;

    std::vector<astfri::ConstructorDefStmt*> constructorsBar;
    auto cnstrBar     = statements.mk_constructor_def();
    cnstrBar->access_ = astfri::AccessModifier::Public;
    cnstrBar->owner_  = classBar;
    constructorsBar.push_back(cnstrBar);
    classBar->constructors_ = constructorsBar;

    std::vector<astfri::DestructorDefStmt*> descructorsBar;
    auto dstrctrBar = statements.mk_destructor_def(classBar, nullptr);
    descructorsBar.push_back(dstrctrBar);
    classBar->destructors_ = descructorsBar;

    std::vector<astfri::MemberVarDefStmt*> fieldsBar;
    auto memberBar = statements.mk_member_var_def(
        "nameStr_",
        types.mk_class("std::string", astfri::mk_scope()),
        nullptr,
        astfri::AccessModifier::Private
    );
    fieldsBar.push_back(memberBar);
    classBar->vars_ = fieldsBar;

    std::vector<astfri::InterfaceDefStmt*> interfacesFoo;
    interfacesFoo.push_back(interfaceIVisitable);
    classFoo->interfaces_ = interfacesFoo;

    std::vector<astfri::MemberVarDefStmt*> fieldsFoo;
    fieldsFoo.push_back(
        statements
            .mk_member_var_def("number_", types.mk_int(), nullptr, astfri::AccessModifier::Private)
    );
    fieldsFoo.push_back(statements.mk_member_var_def(
        "bar_",
        types.mk_indirect(types.mk_class("Bar", astfri::mk_scope())),
        nullptr,
        astfri::AccessModifier::Private
    ));
    classFoo->vars_ = fieldsFoo;

    auto func       = statements.mk_function_def();
    func->name_     = "foo";
    func->retType_  = types.mk_indirect(types.mk_class("Bar", astfri::mk_scope()));
    std::vector<astfri::ParamVarDefStmt*> params;
    params.push_back(statements.mk_param_var_def("number", types.mk_int(), nullptr));
    func->params_ = params;

    std::vector<astfri::MethodDefStmt*> methodsFoo;
    auto methodFooOne         = statements.mk_method_def();
    methodFooOne->access_     = astfri::AccessModifier::Public;
    methodFooOne->owner_      = (classFoo);
    methodFooOne->virtuality_ = astfri::Virtuality::NotVirtual;
    methodFooOne->func_       = func;
    methodsFoo.push_back(methodFooOne);
    classFoo->methods_ = methodsFoo;

    astfri::TranslationUnit tu;
    std::vector<astfri::ClassDefStmt*> classes;
    classes.push_back(classFoo);
    classes.push_back(classBar);
    classes.push_back(classParent);
    std::vector<astfri::InterfaceDefStmt*> interfaces;
    interfaces.push_back(interfaceIVisitable);

    tu.classes_    = classes;
    tu.interfaces_ = interfaces;

    std::string config_file;
    if (argc > 1)
    {
        config_file = argv[1];
    }
    else
    {
        config_file = "../libastfri-uml/examples/default_config.json";
    }

    astfri::uml::Config conf;
    if (! conf.parse_json(config_file.c_str()))
    {
        conf.use_default_values();
        std::cout << "Unable to parse JSON config. Using default values.\n";
    }
    // config can be changed at any point before calling run
    // either by directly accessing its member variables
    // or using its parse_json method
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op);
    uml.run(tu);
}
