#ifndef LIBASTFRIUML_CLASS_VISITOR_HPP
#define LIBASTFRIUML_CLASS_VISITOR_HPP

#include <libastfri-uml/impl/ElementStructs.hpp>
#include <libastfri-uml/impl/UMLOutputter.hpp>

#include <map>
#include <set>

namespace astfri::uml {
class ClassVisitor : public astfri::ThrowingVisitorAdapter {
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
    bool find_relation(const RelationStruct& rel);
    bool find_class(std::string name);
    bool find_interface(std::string name);

    void finish();

public:
    void set_config(const Config& config);
    void set_outputter(const UMLOutputter& outputter);

    void visit(const astfri::IntType& type) override;
    void visit(const astfri::FloatType& type) override;
    void visit(const astfri::CharType& type) override;
    void visit(const astfri::BoolType& type) override;
    void visit(const astfri::VoidType& type) override;
    void visit(const astfri::ClassType& type) override;
    void visit(const astfri::InterfaceType& type) override;
    void visit(const astfri::IndirectionType& type) override;
    void visit(const astfri::IncompleteType& type) override;

    void visit(const astfri::ParamVarDefStmt& stmt) override;
    void visit(const astfri::MemberVarDefStmt& stmt) override;
    void visit(const astfri::FunctionDefStmt& stmt) override;
    void visit(const astfri::MethodDefStmt& stmt) override;
    void visit(const astfri::ConstructorDefStmt& stmt) override;
    void visit(const astfri::DestructorDefStmt& stmt) override;
    void visit(const astfri::GenericParam& stmt) override;
    void visit(const astfri::ClassDefStmt& stmt) override;
    void visit(const astfri::InterfaceDefStmt& stmt) override;
    void visit(const astfri::TranslationUnit& stmt) override;
};
} // namespace astfri::uml

#endif
