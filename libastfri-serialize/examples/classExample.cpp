#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <algorithm>
#include <iostream>

/*
void printChar(char ch){
    for(int i = 0; i < 10; ++i)
    {
        std::cout << ch << " ";
    }
}
class Example{
    public:
    void print(){
        printChar(symbol);
    }
    void printNumbers(){
        for(int i = 0 ; i < numbers.size(); ++i){
            std::cout << number << " ";
        }
    }
    private:
    std::vector<int> numbers{1,2,3,4};
    const char symbol{'*'};

};*/

int main()
{
    auto& statements  = astfri::StmtFactory::get_instance();
    auto& expressions = astfri::ExprFactory::get_instance();
    auto& types       = astfri::TypeFactory::get_instance();

    std::vector<astfri::MemberVarDefStmt*> attributes{
        statements.mk_member_var_def(
            "symbol",
            types.mk_char(),
            expressions.mk_char_literal('*'),
            astfri::AccessModifier::Private
        ),
        statements.mk_member_var_def(
            "numbers",
            types.mk_user("std::vector"),
            expressions.mk_function_call(
                "std:vector",
                std::vector<astfri::Expr*>{
                    expressions.mk_int_literal(1),
                    expressions.mk_int_literal(2),
                    expressions.mk_int_literal(3),
                    expressions.mk_int_literal(4)
                }
            ),
            astfri::AccessModifier::Private
        )
    };

    astfri::ClassDefStmt* classDefinition = statements.mk_class_def(
        "Example",
        std::move(attributes),
        std::vector<astfri::MethodDefStmt*>{
            statements.mk_method_def(
                classDefinition,
                statements.mk_function_def(
                    "print",
                    std::vector<astfri::ParamVarDefStmt*>{},
                    types.mk_void(),
                    statements.mk_compound(
                        std::vector<astfri::Stmt*>{statements.mk_expr(expressions.mk_function_call(
                            "printChar",
                            std::vector<astfri::Expr*>{expressions.mk_local_var_ref("symbol")}
                        ))}
                    )
                ),
                astfri::AccessModifier::Public
            ),
            statements.mk_method_def(
                classDefinition,
                statements.mk_function_def(
                    "printNumbers",
                    std::vector<astfri::ParamVarDefStmt*>{},
                    types.mk_void(),
                    statements.mk_compound(std::vector<astfri::Stmt*>{statements.mk_for(
                        statements
                            .mk_local_var_def("i", types.mk_int(), expressions.mk_int_literal(0)),
                        expressions.mk_bin_on(
                            expressions.mk_local_var_ref("i"),
                            astfri::BinOpType::Less,
                            expressions.mk_method_call(
                                expressions.mk_member_var_ref("numbers"),
                                "size",
                                std::vector<astfri::Expr*>{}
                            )
                        ),
                        statements.mk_expr(expressions.mk_unary_op(
                            astfri::UnaryOpType::Plus,
                            expressions.mk_local_var_ref("i")
                        )),
                        statements.mk_compound(std::vector<astfri::Stmt*>{})
                    )})
                ),
                astfri::AccessModifier::Public
            )
        },
        std::vector<astfri::GenericParam*>{}
    );

    std::cout << "Class Definition: " << std::endl;

    return 0;
}
