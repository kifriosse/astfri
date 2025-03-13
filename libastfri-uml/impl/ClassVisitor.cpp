#include <libastfri-uml/inc/ClassVisitor.hpp>
#include <string>

namespace astfri::uml {
        void ClassVisitor::create_relation(std::string target, RelationType type) {
            RelationStruct r;
            r.from_ = this->currentClass_.name_;
            r.to_ = target;
            r.type_ = type;

            RelationStruct* found = this->find_relation(r);
            if (!found) this->relations_.push_back(r);
        }

        RelationStruct* ClassVisitor::find_relation(RelationStruct const& rel) {
        for (size_t i = 0; i < this->relations_.size(); ++i) {
            if ((this->relations_[i].from_.compare(rel.from_) == 0) &&
                (this->relations_[i].to_.compare(rel.to_) == 0)) {
                    return &this->relations_[i];
            }
        }
        return nullptr;
    }

    bool ClassVisitor::find_class(std::string name) {
        for (std::string c : this->classes_) {
            if (c.compare(name) == 0) return true;
        }
        return false;
    }
    
    bool ClassVisitor::find_interface(std::string name) {
        for (std::string i : this->interfaces_) {
            if (i.compare(name) == 0) return true;
        }
        return false;
    }

    void ClassVisitor::finish() {
        if (this->config_->writeToFile_) {
            this->outputter_->write_to_file();
        } else {
            this->outputter_->write_to_console();
        }
    }

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
            this->create_relation(type.name_, RelationType::ASSOCIATION);
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
        this->currentMethod_.returnIsIndirect_ = this->currentVariable_.isIndirect_;
        this->currentMethod_.name_ = stmt.func_->name_;
        this->currentMethod_.accessMod_ = stmt.access_;
        for (astfri::ParamVarDefStmt* p : stmt.func_->params_) {
            p->accept(*this);
            // TODO - if (stmt.initializer_) stmt.initializer_->accept(*this);
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

        for (astfri::ClassDefStmt* base : stmt.bases_) {
            this->create_relation(base->name_, RelationType::EXTENTION);
        }

        for (astfri::InterfaceDefStmt* interface : stmt.interfaces_) {
            this->create_relation(interface->name_, RelationType::IMPLEMENTATION);
        }

        for (astfri::ConstructorDefStmt* constructor : stmt.constructors_) {
            this->currentMethod_.accessMod_ = constructor->access_;
            this->currentMethod_.name_ = constructor->owner_->name_;
            for (astfri::ParamVarDefStmt* p : constructor->params_) {
                p->accept(*this);
                this->currentMethod_.params_.push_back(this->currentVariable_);
                this->currentVariable_.reset();
            }
            this->outputter_->add_function_member(this->currentMethod_);
            this->currentMethod_.reset();
        }

        for (astfri::DestructorDefStmt* descructor : stmt.destructors_) {
            this->currentMethod_.accessMod_ = AccessModifier::Public;
            this->currentMethod_.name_ = this->config_->destructorIndicator_ + descructor->owner_->name_;
            this->outputter_->add_function_member(this->currentMethod_);
            this->currentMethod_.reset();
        }

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

    void ClassVisitor::visit(astfri::InterfaceDefStmt const& stmt) {
        this->currentClass_.name_ = stmt.name_;
        for (astfri::GenericParam* gp : stmt.tparams_) {
            this->currentClass_.genericParams_.push_back(gp->name_);
        }
        this->outputter_->open_interface(this->currentClass_);

        for (astfri::InterfaceDefStmt* base : stmt.bases_) {
            this->create_relation(base->name_, RelationType::EXTENTION);
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
            this->classes_.push_back(c->name_);
        }

        for (astfri::InterfaceDefStmt* i : stmt.interfaces_) {
            this->interfaces_.push_back(i->name_);
        }
        
        for (astfri::ClassDefStmt* c : stmt.classes_) {
            c->accept(*this);
        }

        for (astfri::InterfaceDefStmt* i : stmt.interfaces_) {
            i->accept(*this);
        }

        for (RelationStruct r : this->relations_) {
            this->outputter_->add_relation(r);
        }

        this->finish();
    }
} // namespace astfri::uml