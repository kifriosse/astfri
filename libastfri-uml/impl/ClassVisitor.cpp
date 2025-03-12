#include <libastfri-uml/inc/ClassVisitor.hpp>
#include <string>
#include "libastfri-uml/inc/ElementStructs.hpp"
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/Type.hpp"

namespace astfri::uml {
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

    void ClassVisitor::visit (astfri::UserType const& type) {
        if (type.name_.compare(this->currentClass_.name_) != 0) {
            RelationStruct r;
            r.from_ = this->currentClass_.name_;
            r.to_ = type.name_;
            r.type_ = RelationType::ASSOCIATION;

            bool duplicate = false;
            for (RelationStruct rs : this->relations_) {
                if ((rs.from_.compare(r.from_) == 0 ) && rs.to_.compare(r.to_) == 0) {
                    duplicate = true;
                }
            }

            if (!duplicate) this->relations_.push_back(r);
        }
        this->currentVariable_.type_ = type.name_;
    }

    void ClassVisitor::visit (astfri::IndirectionType const& type) {
        this->currentVariable_.isIndirect_ = true;
        type.indirect_->accept(*this);
    }
        
    void ClassVisitor::visit (astfri::ParamVarDefStmt const& stmt) {
        stmt.type_->accept(*this);
        this->currentVariable_.name_ = stmt.name_;
        //if (stmt.initializer_) stmt.initializer_->accept(*this);
    }

    void ClassVisitor::visit (astfri::MemberVarDefStmt const& stmt) {
        stmt.type_->accept(*this);
        this->currentVariable_.name_ = stmt.name_;
        this->currentVariable_.accessMod_ = stmt.access_;
        //if (stmt.initializer_) stmt.initializer_->accept(*this);
        this->outputter_->add_data_member(this->currentVariable_);
        this->currentVariable_.reset();
    }

    void ClassVisitor::visit (astfri::GlobalVarDefStmt const& /*stmt*/) {

    }

    void ClassVisitor::visit (astfri::FunctionDefStmt const& /*stmt*/) {

    }

    void ClassVisitor::visit (astfri::MethodDefStmt const& stmt) {
        stmt.func_->retType_->accept(*this);
        this->currentMethod_.retType_ = this->currentVariable_.type_;
        this->currentMethod_.name_ = stmt.func_->name_;
        this->currentMethod_.accessMod_ = stmt.access_;
        for (astfri::ParamVarDefStmt* p : stmt.func_->params_) {
            p->type_->accept(*this);
            this->currentVariable_.name_ = p->name_;
            //if (stmt.initializer_) stmt.initializer_->accept(*this);
            this->currentMethod_.params_.push_back(this->currentVariable_);
            this->currentVariable_.reset();
        }
        this->outputter_->add_function_member(this->currentMethod_);
        this->currentMethod_.reset();
    }

    void ClassVisitor::visit (astfri::ClassDefStmt const& stmt) {
        this->currentClass_.name_ = stmt.name_;
        for (astfri::GenericParam* gp : stmt.tparams_) {
            this->currentClass_.genericParams_.push_back(gp->name_);
        }
        this->outputter_->open_class(this->currentClass_);

        for (astfri::MemberVarDefStmt* var : stmt.vars_)
        {
            var->accept(*this);
        }

        for (astfri::MethodDefStmt* method : stmt.methods_)
        {
            method->accept(*this);
        }

        this->currentClass_.reset();
        this->outputter_->close_class();
    }

    void ClassVisitor::visit(astfri::TranslationUnit const& stmt) {
        for (astfri::ClassDefStmt* c : stmt.classes_) {
            c->accept(*this);
        }

        for (RelationStruct r : this->relations_) {
            this->outputter_->add_relation(r);
        }
    }

    void ClassVisitor::finish() {
        if (this->config_->writeToFile_) {
            this->outputter_->write_to_file();
        } else {
            this->outputter_->write_to_console();
        }
    }

} // namespace astfri::uml