#include <libastfri-uml/inc/ClassVisitor.hpp>

namespace uml {
    void ClassVisitor::set_config(Config* config) {
        this->config_ = config;
    }

    void ClassVisitor::set_outputter(UMLOutputter* outputter) {
        this->outputter_ = outputter;
    }

    void ClassVisitor::visit (astfri::IntType const& type) {
        this->currentVariable_.type_ = this->config_->intTypeName_;
    }
        
    void ClassVisitor::visit (astfri::ParamVarDefStmt const& stmt) {

    }

    void ClassVisitor::visit (astfri::MemberVarDefStmt const& stmt) {
        stmt.type_->accept(*this);
        this->currentVariable_.name_ = stmt.name_;
        if (stmt.initializer_) stmt.initializer_->accept(*this);
        this->outputter_->add_data_member(this->currentVariable_);
    }

    void ClassVisitor::visit (astfri::GlobalVarDefStmt const& stmt) {

    }

    void ClassVisitor::visit (astfri::FunctionDefStmt const& stmt) {

    }

    void ClassVisitor::visit (astfri::MethodDefStmt const& stmt) {

    }

    void ClassVisitor::visit (astfri::ClassDefStmt const& stmt) {
        this->currentClass_.name_ = stmt.name_;
        this->currentClass_.genericParam_ = stmt.tparams_[0] ? stmt.tparams_[0]->name_ : "";
        this->outputter_->open_class(this->currentClass_);

        for (astfri::MemberVarDefStmt* var : stmt.vars_)
        {
            var->accept(*this);
        }

        for (astfri::MethodDefStmt* method : stmt.methods_)
        {
            method->accept(*this);
        }

        this->outputter_->close_class();
    }

} // namespace uml