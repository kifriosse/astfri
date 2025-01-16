#include <libastfri/inc/Visitor.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <iostream>
#include <vector>
#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/Type.hpp"

class ClassVisitor : public astfri::VisitorAdapter
{
    void visit (astfri::IntType const& type) override
    {
        std::cout << "int";
    }
    
    void visit (astfri::ParamVarDefStmt const& /*stmt*/) override { }

    void visit (astfri::MemberVarDefStmt const& stmt) override
    {
        stmt.type_->accept(*this);
        std::cout << " " << stmt.name_;
        if (stmt.initializer_) stmt.initializer_->accept(*this);
        std::cout << std::endl;
    }

    void visit (astfri::GlobalVarDefStmt const& /*stmt*/) override { }

    void visit (astfri::FunctionDefStmt const& /*stmt*/) override { }

    void visit (astfri::MethodDefStmt const& /*stmt*/) override { }

    void visit (astfri::ClassDefStmt const& stmt) override
    {
        std::cout << "Class " << stmt.name_ << std::endl;
        std::cout << "Member variables: " << std::endl;
        for (astfri::MemberVarDefStmt* var : stmt.vars_)
        {
            var->accept(*this);
        }
    }
};

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

    auto classDS = statements.mk_class_def(
        "ExampleClass",
        memberVariables,
        methods,
        genericParams //GenericParam is TODO
    );

    ClassVisitor cv;
    classDS->accept(cv);
}