#ifndef TRANSFORMER_CLASS_HPP
#define TRANSFORMER_CLASS_HPP

#include <string>

#include "libastfri/inc/ExprFactory.hpp"
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/StmtFactory.hpp"
#include "libastfri/inc/TypeFactory.hpp"
#include "NodeGetter.hpp"
#include "NodeMapper.hpp"
#include "TreeMaker.hpp"

class Transformer
{
private:
    TreeMaker* treeMaker;
    NodeGetter* nodeGetter;
    NodeMapper* nodeMapper;
    astfri::StmtFactory& stmtFactory;
    astfri::TypeFactory& typeFactory;
    astfri::ExprFactory& exprFactory;
    std::vector<astfri::Stmt*> transform_member_vars (std::vector<Member_variable_node> memberVarNodes);
    std::vector<astfri::Stmt*> transform_local_vars (std::vector<Local_variable_node> localVarNodes);
    std::vector<astfri::Stmt*> transform_formal_params (std::vector<Parameter_node> formalParamNodes);
    std::vector<astfri::Stmt*> transform_methods (std::vector<Method_node> methodNodes);
    std::vector<astfri::Stmt*> transform_classes (std::vector<Class_node> classNodes);

public:
    Transformer(std::string const& path);
};

#endif // TRANSFORMER_CLASS_HPP