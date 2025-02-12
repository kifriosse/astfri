
#include <iostream>
#include <libastfri/inc/Visitor.hpp>

class JsonDeSerialiser : astfri::VisitorAdapter{
    std::ostream& output_;
public:
 JsonDeSerialiser(std::ostream& output)
    :output_(output){}
 void visit(const astfri::CharType& charType) override {
    this->output_ << "\"Type\" : \"char\",";
}
void visit(const astfri::IntType& intType) override {
    this->output_ << "\"Type\" : \"int\",";
} 
void visit(const astfri::FunctionDefStmt& functDef) override {
    this->output_ << "\"Name\" : \"" << functDef.name_ << "\"\n";
    this->visit(*functDef.retType_);
    this->output_ << std::endl;
    this->output_ << "Parameters : [\n";
    for (auto param : functDef.params_)
    {
        this->visit(*param);
    }
    this->output_ << "]\n";
    astfri::VisitorAdapter::visit(*functDef.body_);

}
void visit(const astfri::ParamVarDefStmt& param) override {
    this->output_ << "{ " << "\"Name\" : \"" << param.name_ << "\", " ;
    this->visit(*param.type_);
    astfri::VisitorAdapter::visit(*param.initializer_);
    this->output_ <<"}\n";

}
}

