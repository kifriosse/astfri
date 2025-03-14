#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

namespace my_ast_trees
{
    template <typename TreeVisitor>
    void load_ast_tree_1(TreeVisitor& tv) {
        //Example of creating output from "stack.h"
        using namespace astfri;
        StmtFactory& sfc = StmtFactory::get_instance();
        ExprFactory& efc = ExprFactory::get_instance();
        TypeFactory& tfc = TypeFactory::get_instance();
        TranslationUnit* tu = std::move(sfc.mk_translation_unit());
        //---------------------------------------------------------
        InterfaceDefStmt* idsStack = std::move(sfc.mk_interface_def(std::move("Stack")));
        idsStack->tparams_ = {std::move(sfc.mk_generic_param(std::move(""), std::move("T")))};
        idsStack->bases_ = {std::move(sfc.mk_interface_def(std::move("ADT")))};
        idsStack->methods_ = {
            std::move(sfc.mk_method_def(
                {},
                std::move(sfc.mk_function_def(
                    std::move("push"),
                    {
                        std::move(sfc.mk_param_var_def(
                            std::move("element"),
                            std::move(tfc.mk_user(std::move("T"))),
                            {}
                        ))
                    },
                    std::move(tfc.mk_void()),
                    {}
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            )),
            std::move(sfc.mk_method_def(
                {},
                std::move(sfc.mk_function_def(
                    std::move("peek"),
                    {},
                    std::move(tfc.mk_indirect(std::move(tfc.mk_user(std::move("T"))))),
                    {}
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            )),
            std::move(sfc.mk_method_def(
                {},
                std::move(sfc.mk_function_def(
                    std::move("pop"),
                    {},
                    std::move(tfc.mk_user(std::move("T"))),
                    {}
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            ))
        };
        //------------------------------------------------
        ClassDefStmt* cdsImplStack = std::move(sfc.mk_class_def(std::move("ImplicitStack")));
        cdsImplStack->tparams_ = {std::move(sfc.mk_generic_param(std::move(""), std::move("T")))};
        cdsImplStack->interfaces_.push_back(idsStack);
        ClassDefStmt* ext = std::move(sfc.mk_class_def(std::move("ADS")));
        ext->tparams_ = {
            std::move(sfc.mk_generic_param(std::move(""), std::move("T")))
        };
        cdsImplStack->bases_.push_back(std::move(ext));
        cdsImplStack->constructors_ = {
            std::move(sfc.mk_constructor_def(
                cdsImplStack,
                {},
                {
                    std::move(sfc.mak_base_initializer(
                        std::move("ADS<T>"),
                        {
                            std::move(efc.mk_new(std::move(efc.mk_constructor_call(
                                std::move(tfc.mk_user(std::move("amt::IS<T>"))),
                                {}
                            ))))
                        }
                    ))
                },
                {},
                std::move(AccessModifier::Public)
            )),
            std::move(sfc.mk_constructor_def(
                cdsImplStack,
                {
                    std::move(sfc.mk_param_var_def(
                        std::move("other"),
                        std::move(tfc.mk_indirect(std::move(tfc.mk_user(std::move("ImplicitStack"))))),
                        {}
                    ))
                },
                {
                    std::move(sfc.mak_base_initializer(
                        std::move("ADS<T>"),
                        {
                            std::move(efc.mk_new(std::move(efc.mk_constructor_call(
                                std::move(tfc.mk_user(std::move("amt::IS<T>"))),
                                {}
                            )))),
                            efc.mk_param_var_ref(std::move("other"))
                        }
                    ))
                },
                {},
                std::move(AccessModifier::Public)
            ))
        };
        cdsImplStack->methods_ = {
            std::move(sfc.mk_method_def(
                cdsImplStack,
                std::move(sfc.mk_function_def(
                    std::move("push"),
                    {
                        std::move(sfc.mk_param_var_def(
                            std::move("element"),
                            std::move(tfc.mk_user(std::move("T"))),
                            {}
                        ))
                    },
                    std::move(tfc.mk_void()),
                    std::move(sfc.mk_compound({
                        std::move(sfc.mk_expr(std::move(efc.mk_bin_on(
                            std::move(efc.mk_member_var_ref(
                                std::move(efc.mk_method_call(
                                    std::move(efc.mk_method_call(
                                        std::move(efc.mk_this()),
                                        std::move("getSequence"),
                                        {}
                                    )),
                                    std::move("insertLast"),
                                    {}
                                )),
                                std::move("data_")
                            )),
                            std::move(BinOpType::Assign),
                            std::move(efc.mk_param_var_ref(std::move("element")))
                        ))))
                    }))
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            )),
            std::move(sfc.mk_method_def(
                cdsImplStack,
                std::move(sfc.mk_function_def(
                    std::move("peek"),
                    {},
                    std::move(tfc.mk_indirect(std::move(tfc.mk_user(std::move("T"))))),
                    std::move(sfc.mk_compound({
                        std::move(sfc.mk_if(
                            std::move(efc.mk_method_call(
                                std::move(efc.mk_this()),
                                std::move("isEmpty"),
                                {}
                            )),
                            std::move(sfc.mk_throw(std::move(efc.mk_function_call(
                                std::move("out_of_range"),
                                {
                                    std::move(efc.mk_string_literal(std::move("Stack is empty!")))
                                }
                            )))),
                            {}
                        )),
                        std::move(sfc.mk_return(std::move(efc.mk_member_var_ref(
                            std::move(efc.mk_method_call(
                                std::move(efc.mk_method_call(
                                    std::move(efc.mk_this()),
                                    std::move("getSequence"),
                                    {}
                                )),
                                std::move("accessLast"),
                                {}
                            )),
                            std::move("data_")
                        ))))
                    }))
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            )),
            std::move(sfc.mk_method_def(
                cdsImplStack,
                std::move(sfc.mk_function_def(
                    std::move("pop"),
                    {},
                    std::move(tfc.mk_user(std::move("T"))),
                    std::move(sfc.mk_compound({
                        std::move(sfc.mk_if(
                            std::move(efc.mk_method_call(
                                std::move(efc.mk_this()),
                                std::move("isEmpty"),
                                {}
                            )),
                            std::move(sfc.mk_throw(std::move(efc.mk_function_call(
                                std::move("out_of_range"),
                                {
                                    std::move(efc.mk_string_literal(std::move("Stack is empty!")))
                                }
                            )))),
                            {}
                        )),
                        std::move(sfc.mk_local_var_def(
                            std::move("result"),
                            std::move(tfc.mk_user(std::move("T"))),
                            std::move(efc.mk_member_var_ref(
                                std::move(efc.mk_method_call(
                                    std::move(efc.mk_method_call(
                                        std::move(efc.mk_this()),
                                        std::move("getSequence"),
                                        {}
                                    )),
                                    std::move("accessLast"),
                                    {}
                                )),
                                std::move("data_")
                            ))
                        )),
                        std::move(sfc.mk_expr(std::move(efc.mk_method_call(
                            std::move(efc.mk_method_call(
                                std::move(efc.mk_this()),
                                std::move("getSequence"),
                                {}
                            )),
                            std::move("removeLast"),
                            {}
                        )))),
                        std::move(sfc.mk_return(std::move(efc.mk_local_var_ref(std::move("result")))))
                    }))
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            )),
            std::move(sfc.mk_method_def(
                cdsImplStack,
                std::move(sfc.mk_function_def(
                    std::move("getSequence"),
                    {},
                    std::move(tfc.mk_indirect(std::move(tfc.mk_user(std::move("amt::IS<T>"))))),
                    std::move(sfc.mk_compound({
                        std::move(sfc.mk_return(std::move(efc.mk_unknown())))
                    }))
                )),
                std::move(AccessModifier::Private),
                std::move(Virtuality::NotVirtual)
            ))
        };
        //---------------------------------------------------
        ClassDefStmt* cdsExplStack = std::move(sfc.mk_class_def(std::move("ExplicitStack")));
        cdsExplStack->tparams_ = {std::move(sfc.mk_generic_param(std::move(""), std::move("T")))};
        cdsExplStack->interfaces_.push_back(idsStack);
        ClassDefStmt* ext2 = std::move(sfc.mk_class_def(std::move("ADS")));
        ext2->tparams_ = {
            std::move(sfc.mk_generic_param(std::move(""), std::move("T")))
        };
        cdsExplStack->bases_.push_back(std::move(ext2));
        cdsExplStack->constructors_ = {
            std::move(sfc.mk_constructor_def(
                cdsExplStack,
                {},
                {
                    std::move(sfc.mak_base_initializer(
                        std::move("ADS<T>"),
                        {
                            std::move(efc.mk_new(std::move(efc.mk_constructor_call(
                                std::move(tfc.mk_user(std::move("amt::SinglyLS<T>"))),
                                {}
                            ))))
                        }
                    ))
                },
                {},
                std::move(AccessModifier::Public)
            )),
            std::move(sfc.mk_constructor_def(
                cdsExplStack,
                {
                    std::move(sfc.mk_param_var_def(
                        std::move("other"),
                        std::move(tfc.mk_indirect(std::move(tfc.mk_user(std::move("ExplicitStack"))))),
                        {}
                    ))
                },
                {
                    std::move(sfc.mak_base_initializer(
                        std::move("ADS<T>"),
                        {
                            std::move(efc.mk_new(std::move(efc.mk_constructor_call(
                                std::move(tfc.mk_user(std::move("amt::SinglyLS<T>"))),
                                {}
                            )))),
                            efc.mk_param_var_ref(std::move("other"))
                        }
                    ))
                },
                {},
                std::move(AccessModifier::Public)
            ))
        };
        cdsExplStack->methods_ = {
            std::move(sfc.mk_method_def(
                cdsExplStack,
                std::move(sfc.mk_function_def(
                    std::move("push"),
                    {
                        std::move(sfc.mk_param_var_def(
                            std::move("element"),
                            std::move(tfc.mk_user(std::move("T"))),
                            {}
                        ))
                    },
                    std::move(tfc.mk_void()),
                    std::move(sfc.mk_compound({
                        std::move(sfc.mk_expr(std::move(efc.mk_bin_on(
                            std::move(efc.mk_member_var_ref(
                                std::move(efc.mk_method_call(
                                    std::move(efc.mk_method_call(
                                        std::move(efc.mk_this()),
                                        std::move("getSequence"),
                                        {}
                                    )),
                                    std::move("insertFirst"),
                                    {}
                                )),
                                std::move("data_")
                            )),
                            std::move(BinOpType::Assign),
                            std::move(efc.mk_param_var_ref(std::move("element")))
                        ))))
                    }))
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            )),
            std::move(sfc.mk_method_def(
                cdsExplStack,
                std::move(sfc.mk_function_def(
                    std::move("peek"),
                    {},
                    std::move(tfc.mk_indirect(std::move(tfc.mk_user(std::move("T"))))),
                    std::move(sfc.mk_compound({
                        std::move(sfc.mk_if(
                            std::move(efc.mk_method_call(
                                std::move(efc.mk_this()),
                                std::move("isEmpty"),
                                {}
                            )),
                            std::move(sfc.mk_throw(std::move(efc.mk_function_call(
                                std::move("out_of_range"),
                                {
                                    std::move(efc.mk_string_literal(std::move("Stack is empty!")))
                                }
                            )))),
                            {}
                        )),
                        std::move(sfc.mk_return(std::move(efc.mk_member_var_ref(
                            std::move(efc.mk_method_call(
                                std::move(efc.mk_method_call(
                                    std::move(efc.mk_this()),
                                    std::move("getSequence"),
                                    {}
                                )),
                                std::move("accessFirst"),
                                {}
                            )),
                            std::move("data_")
                        ))))
                    }))
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            )),
            std::move(sfc.mk_method_def(
                cdsExplStack,
                std::move(sfc.mk_function_def(
                    std::move("pop"),
                    {},
                    std::move(tfc.mk_user(std::move("T"))),
                    std::move(sfc.mk_compound({
                        std::move(sfc.mk_if(
                            std::move(efc.mk_method_call(
                                std::move(efc.mk_this()),
                                std::move("isEmpty"),
                                {}
                            )),
                            std::move(sfc.mk_throw(std::move(efc.mk_function_call(
                                std::move("out_of_range"),
                                {
                                    std::move(efc.mk_string_literal(std::move("Stack is empty!")))
                                }
                            )))),
                            {}
                        )),
                        std::move(sfc.mk_local_var_def(
                            std::move("result"),
                            std::move(tfc.mk_user(std::move("T"))),
                            std::move(efc.mk_member_var_ref(
                                std::move(efc.mk_method_call(
                                    std::move(efc.mk_method_call(
                                        std::move(efc.mk_this()),
                                        std::move("getSequence"),
                                        {}
                                    )),
                                    std::move("accessFirst"),
                                    {}
                                )),
                                std::move("data_")
                            ))
                        )),
                        std::move(sfc.mk_expr(std::move(efc.mk_method_call(
                            std::move(efc.mk_method_call(
                                std::move(efc.mk_this()),
                                std::move("getSequence"),
                                {}
                            )),
                            std::move("removeFirst"),
                            {}
                        )))),
                        std::move(sfc.mk_return(std::move(efc.mk_local_var_ref(std::move("result")))))
                    }))
                )),
                std::move(AccessModifier::Public),
                std::move(Virtuality::Virtual)
            )),
            std::move(sfc.mk_method_def(
                cdsExplStack,
                std::move(sfc.mk_function_def(
                    std::move("getSequence"),
                    {},
                    std::move(tfc.mk_indirect(std::move(tfc.mk_user(std::move("amt::SinglyLS<T>"))))),
                    std::move(sfc.mk_compound({
                        std::move(sfc.mk_return(std::move(efc.mk_unknown())))
                    }))
                )),
                std::move(AccessModifier::Private),
                std::move(Virtuality::NotVirtual)
            ))
        };
        //---------------------------------------------------
        tu->interfaces_.push_back(std::move(idsStack));
        tu->classes_.push_back(std::move(cdsImplStack));
        tu->classes_.push_back(std::move(cdsExplStack));
        tv.visit(std::move(*tu));
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
        std::vector<astfri::MethodDefStmt*> methods {
            statements.mk_method_def(
                cds,
                statements.mk_function_def(
                    cds->name_,
                    constructorParams,
                    types.mk_void(),
                    statements.mk_compound(
                        {
                            statements.mk_expr(expressions.mk_bin_on(
                                expressions.mk_member_var_ref(
                                    expressions.mk_class_ref(cds->name_),
                                    "a"
                                ),
                                astfri::BinOpType::Assign,
                                expressions.mk_param_var_ref("cislo1")
                                )
                            ),
                            statements.mk_expr(
                                expressions.mk_bin_on(
                                    expressions.mk_member_var_ref(
                                        expressions.mk_class_ref(cds->name_),
                                        "b"
                                    ),
                                    astfri::BinOpType::Assign,
                                    expressions.mk_param_var_ref("cislo2")
                                )
                            )
                        }
                    )
                ),
                astfri::AccessModifier::Public,
                astfri::Virtuality::NotVirtual
            ),
            statements.mk_method_def(
                cds,
                statements.mk_function_def(
                    "~TestClass",
                    {},
                    types.mk_unknown(),
                    {}
                ),
                astfri::AccessModifier::Public,
                astfri::Virtuality::NotVirtual
            ),
            statements.mk_method_def(
                cds,
                statements.mk_function_def(
                    "getCislo",
                    {},
                    types.mk_int(),
                    statements.mk_compound({
                        statements.mk_return(
                            expressions.mk_bin_on(
                                expressions.mk_member_var_ref(expressions.mk_class_ref(cds->name_), "a"),
                                astfri::BinOpType::Multiply,
                                expressions.mk_member_var_ref(expressions.mk_class_ref(cds->name_), "b")
                            )
                        )
                    })
                ),
                astfri::AccessModifier::Private,
                astfri::Virtuality::NotVirtual
            )
        };
        cds = statements.mk_class_def(cds->name_);
        cds->vars_ = atributes;
        cds->methods_ = methods;
        cds->accept(tv);
    }
}