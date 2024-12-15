#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/Visitor.hpp>
#include <iostream>

// int add(int a, int b) {
//   return a + b;
// }

struct CodeVisitor : astfri::VisitorAdapter
{
    void visit (astfri::FunctionDefStmt const& stmt) override
    {
        std::cout << "func " << stmt.name_ << "(";
        for (astfri::ParamVarDefStmt* param : stmt.params_)
        {
            this->visit(*param);
        }
        std::cout << ") {\n";
        this->visit(*stmt.body_);
        std::cout << "}\n";
    }

    void visit (astfri::ParamVarDefStmt const& param) override
    {
        param.type_->accept(*this);
        std::cout << " " << param.name_ << ", ";
    }

    void visit (astfri::IntType const& /*type*/) override
    {
        std::cout << "int";
    }

    void visit (astfri::CompoundStmt const& /*stmt*/) override
    {
        std::cout << "body\n";
    }
};

int main() {
  auto& statements = astfri::StmtFactory::get_instance();
  auto& expressions = astfri::ExprFactory::get_instance();
  auto& types = astfri::TypeFactory::get_instance();

  std::vector<astfri::ParamVarDefStmt *> params = {
    statements.mk_param_var_def("a", types.mk_int(), nullptr),
    statements.mk_param_var_def("b", types.mk_int(), nullptr)
  };

  astfri::CompoundStmt* body = statements.mk_compound({
    statements.mk_return(
      expressions.mk_bin_on(
        expressions.mk_param_var_ref("a"),
        astfri::BinOpType::Add,
        expressions.mk_param_var_ref("b")
      )
    )
  });

  astfri::FunctionDefStmt* func = statements.mk_function_def(
    "add",
    std::move(params),
    types.mk_int(),
    body
  );

  CodeVisitor cv;
  cv.visit(*func);
}