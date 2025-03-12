#pragma once

#include <libastfri/inc/Visitor.hpp>
#include <libastfri/inc/Type.hpp>
#include <libastfri/inc/Stmt.hpp>
#include <libastfri/inc/Expr.hpp>

#include <libastfri-uml/inc/UMLOutputter.hpp>

#include <libastfri-uml/inc/ElementStructs.hpp>

namespace astfri::uml {
    class ClassVisitor : public astfri::VisitorAdapter
    {
    private:
        UMLOutputter* outputter_;
        Config* config_;

        ClassStruct currentClass_;
        MethodStruct currentMethod_;
        VarStruct currentVariable_;

        std::vector<RelationStruct> relations_;
        std::vector<std::string> classes_;
        std::vector<std::string> interfaces_;

        RelationStruct* findRelation(RelationStruct const& rel);
        bool findClass(std::string name);
        bool findInterface(std::string name);

    public:
        void set_config(Config const& config);
        void set_outputter(UMLOutputter const& outputter);

        void visit (astfri::IntType const& type) override;
        void visit (astfri::FloatType const& type) override;
        void visit (astfri::CharType const& type) override;
        void visit (astfri::BoolType const& type) override;
        void visit (astfri::VoidType const& type) override;
        void visit (astfri::UserType const& type) override;
        void visit (astfri::IndirectionType const& type) override;
        
        void visit (astfri::ParamVarDefStmt const& stmt) override;

        void visit (astfri::MemberVarDefStmt const& stmt) override;

        void visit (astfri::GlobalVarDefStmt const& stmt) override;

        void visit (astfri::FunctionDefStmt const& stmt) override;

        void visit (astfri::MethodDefStmt const& stmt) override;

        void visit (astfri::ClassDefStmt const& stmt) override;

        void visit (astfri::TranslationUnit const& stmt) override;

        void finish();
    };
} // namespace astfri::uml