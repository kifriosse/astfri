#include <libastfri-uml/inc/ClassVisitor.hpp>
#include <string>

namespace uml {
    void ClassVisitor::set_config(Config const& config) {
        this->config_ = (Config*)&config;
    }

    void ClassVisitor::set_outputter(UMLOutputter const& outputter) {
        this->outputter_ = (UMLOutputter*)&outputter;
    }

    void ClassVisitor::visit (astfri::IntType const& /*type*/) {
        this->currentVariable_.type_ = this->config_->intTypeName_;
    }

    void ClassVisitor::visit (astfri::FloatType const& /*type*/) {
        this->currentVariable_.type_ = this->config_->floatTypeName_;
    }

    void ClassVisitor::visit (astfri::CharType const& /*type*/) {
        this->currentVariable_.type_ = this->config_->charTypeName_;
    }

    void ClassVisitor::visit (astfri::BoolType const& /*type*/) {
        this->currentVariable_.type_ = this->config_->boolTypeName_;
    }

    void ClassVisitor::visit (astfri::VoidType const& /*type*/) {
        this->currentVariable_.type_ = this->config_->voidTypeName_;
    }
        
    void ClassVisitor::visit (astfri::ParamVarDefStmt const& stmt) {
        stmt.type_->accept(*this);
        this->currentVariable_.name_ = stmt.name_;
        //if (stmt.initializer_) stmt.initializer_->accept(*this);
        this->currentMethod_.param_ = this->currentVariable_;
    }

    void ClassVisitor::visit (astfri::MemberVarDefStmt const& stmt) {
        stmt.type_->accept(*this);
        this->currentVariable_.name_ = stmt.name_;
        this->currentVariable_.accessMod_ = stmt.access_;
        //if (stmt.initializer_) stmt.initializer_->accept(*this);
        this->outputter_->add_data_member(this->currentVariable_);
    }

    void ClassVisitor::visit (astfri::GlobalVarDefStmt const& stmt) {

    }

    void ClassVisitor::visit (astfri::FunctionDefStmt const& stmt) {

    }

    void ClassVisitor::visit (astfri::MethodDefStmt const& stmt) {
        stmt.func_->retType_->accept(*this);
        this->currentMethod_.retType_ = this->currentVariable_.type_;
        this->currentMethod_.name_ = stmt.func_->name_;
        this->currentMethod_.accessMod_ = stmt.access_;
        if (stmt.func_->params_.size() > 0) {
            stmt.func_->params_[0]->accept(*this);
        }
        else {
            this->currentVariable_.name_ = "";
            this->currentVariable_.type_ = "";
            this->currentVariable_.init_ = "";
        }
        this->currentMethod_.param_ = this->currentVariable_;
        this->outputter_->add_function_member(this->currentMethod_);
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