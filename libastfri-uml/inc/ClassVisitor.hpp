#pragma once

#include <libastfri/inc/Visitor.hpp>
#include <libastfri/inc/Type.hpp>
#include <libastfri/inc/Stmt.hpp>
#include <libastfri/inc/Expr.hpp>

#include <libastfri-uml/inc/UMLOutputter.hpp>

#include <libastfri-uml/inc/ElementStructs.hpp>

namespace uml {
    class ClassVisitor : public astfri::VisitorAdapter
    {
    private:
        UMLOutputter* outputter_;
        Config* config_;

        ClassStruct currentClass_;
        MethodStruct currentMethod_;
        VarStruct currentVariable_;

    public:
        void set_config(Config* config);
        void set_outputter(UMLOutputter* outputter);

        void visit (astfri::IntType const& type) override;
        
        void visit (astfri::ParamVarDefStmt const& stmt) override;

        void visit (astfri::MemberVarDefStmt const& stmt) override;

        void visit (astfri::GlobalVarDefStmt const& stmt) override;

        void visit (astfri::FunctionDefStmt const& stmt) override;

        void visit (astfri::MethodDefStmt const& stmt) override;

        void visit (astfri::ClassDefStmt const& stmt) override;
    };
} // namespace uml