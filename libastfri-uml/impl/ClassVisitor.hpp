#ifndef LIBASTFRIUML_CLASS_VISITOR_HPP
#define LIBASTFRIUML_CLASS_VISITOR_HPP

#include <libastfri-uml/impl/ElementStructs.hpp>
#include <libastfri-uml/impl/UMLOutputter.hpp>

#include <map>
#include <set>

namespace astfri::uml
{
class ClassVisitor : public astfri::ThrowingVisitorAdapter
{
private:
    UMLOutputter* outputter_;
    Config* config_;

    ClassStruct currentClass_;
    MethodStruct currentMethod_;
    ConstructorStruct currentConstructor_;
    DestructorStruct currentDestructor_;
    VarStruct currentVariable_;

    std::map<std::string, RelationStruct> relations_;
    std::set<std::string> classes_;
    std::set<std::string> interfaces_;

    void create_relation(std::string target, RelationType type);
    bool find_relation(RelationStruct const& rel);
    bool find_class(std::string name);
    bool find_interface(std::string name);

    void finish();

public:
    void set_config(Config const& config);
    void set_outputter(UMLOutputter const& outputter);

    void visit(astfri::IntType const& type) override;
    void visit(astfri::FloatType const& type) override;
    void visit(astfri::CharType const& type) override;
    void visit(astfri::BoolType const& type) override;
    void visit(astfri::VoidType const& type) override;
    void visit(astfri::ClassType const& type) override;
    void visit(astfri::InterfaceType const& type) override;
    void visit(astfri::IndirectionType const& type) override;
    void visit(astfri::IncompleteType const& type) override;

    void visit(astfri::ParamVarDefStmt const& stmt) override;
    void visit(astfri::MemberVarDefStmt const& stmt) override;
    void visit(astfri::FunctionDefStmt const& stmt) override;
    void visit(astfri::MethodDefStmt const& stmt) override;
    void visit(astfri::ConstructorDefStmt const& stmt) override;
    void visit(astfri::DestructorDefStmt const& stmt) override;
    void visit(astfri::GenericParam const& stmt) override;
    void visit(astfri::ClassDefStmt const& stmt) override;
    void visit(astfri::InterfaceDefStmt const& stmt) override;
    void visit(astfri::TranslationUnit const& stmt) override;
};
} // namespace astfri::uml

#endif
