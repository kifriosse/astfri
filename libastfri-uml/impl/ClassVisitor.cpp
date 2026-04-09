#include <libastfri-uml/impl/ClassVisitor.hpp>

#include <string>

namespace astfri::uml {
void ClassVisitor::create_relation(std::string target, RelationType type) {
    if (! this->find_class(target) && ! this->find_interface(target))
        return;

    RelationStruct r;
    r.from_ = this->currentClass_.name_;
    r.to_   = target;
    r.type_ = type;

    if (! this->find_relation(r))
        this->relations_.emplace(r.from_ + r.to_, r);
}

bool ClassVisitor::find_relation(const RelationStruct& rel) {
    return this->relations_.contains(rel.from_ + rel.to_);
}

bool ClassVisitor::find_class(std::string name) {
    return this->classes_.contains(name);
}

bool ClassVisitor::find_interface(std::string name) {
    return this->interfaces_.contains(name);
}

void ClassVisitor::finish() {
    if (this->config_->writeToFile_) {
        this->outputter_->write_to_file();
    }
    else {
        this->outputter_->write_to_console();
    }
}

void ClassVisitor::set_config(const Config& config) {
    this->config_ = (Config*)&config;
}

void ClassVisitor::set_outputter(const UMLOutputter& outputter) {
    this->outputter_ = (UMLOutputter*)&outputter;
}

void ClassVisitor::visit(const astfri::IntType& /*type*/) {
    this->currentVariable_.type_ = this->config_->intTypeName_;
}

void ClassVisitor::visit(const astfri::FloatType& /*type*/) {
    this->currentVariable_.type_ = this->config_->floatTypeName_;
}

void ClassVisitor::visit(const astfri::CharType& /*type*/) {
    this->currentVariable_.type_ = this->config_->charTypeName_;
}

void ClassVisitor::visit(const astfri::BoolType& /*type*/) {
    this->currentVariable_.type_ = this->config_->boolTypeName_;
}

void ClassVisitor::visit(const astfri::VoidType& /*type*/) {
    this->currentVariable_.type_ = this->config_->voidTypeName_;
}

void ClassVisitor::visit(const astfri::ClassType& type) {
    if (type.name.compare(this->currentClass_.name_) != 0) {
        this->create_relation(type.name, RelationType::ASSOCIATION);
    }
    this->currentVariable_.type_ = type.name;
}

void ClassVisitor::visit(const astfri::InterfaceType& type) {
    if (type.name.compare(this->currentClass_.name_) != 0) {
        this->create_relation(type.name, RelationType::ASSOCIATION);
    }
    this->currentVariable_.type_ = type.name;
}

void ClassVisitor::visit(const astfri::IndirectionType& type) {
    this->currentVariable_.isIndirect_ = true;
    type.indirect->accept(*this);
}

void ClassVisitor::visit(const astfri::IncompleteType& type) {
    this->currentVariable_.type_ = type.name;
}

void ClassVisitor::visit(const astfri::ParamVarDefStmt& stmt) {
    stmt.type->accept(*this);
    this->currentVariable_.name_ = stmt.name;
    // TODO - if (stmt.initializer_) stmt.initializer_->accept(*this);
}

void ClassVisitor::visit(const astfri::MemberVarDefStmt& stmt) {
    if (! this->config_->innerView_ && stmt.access == astfri::AccessModifier::Private)
        return;
    stmt.type->accept(*this);
    this->currentVariable_.name_      = stmt.name;
    this->currentVariable_.accessMod_ = stmt.access;
    // TODO - if (stmt.initializer_) stmt.initializer_->accept(*this);
    this->outputter_->add_data_member(this->currentVariable_);
    this->currentVariable_.reset();
}

void ClassVisitor::visit(const astfri::FunctionDefStmt& stmt) {
    stmt.retType->accept(*this);
    this->currentMethod_.retType_          = this->currentVariable_.type_;
    this->currentMethod_.returnIsIndirect_ = this->currentVariable_.isIndirect_;
    this->currentMethod_.name_             = stmt.name;
    for (astfri::ParamVarDefStmt* p : stmt.params) {
        p->accept(*this);
        this->currentMethod_.params_.push_back(this->currentVariable_);
        this->currentVariable_.reset();
    }
}

void ClassVisitor::visit(const astfri::MethodDefStmt& stmt) {
    if (! this->config_->innerView_ && stmt.access == astfri::AccessModifier::Private)
        return;
    stmt.func->accept(*this);
    this->currentMethod_.accessMod_ = stmt.access;
    this->outputter_->add_function_member(this->currentMethod_);
    this->currentMethod_.reset();
}

void ClassVisitor::visit(const astfri::ConstructorDefStmt& stmt) {
    this->currentConstructor_.class_     = stmt.owner->type->name;
    this->currentConstructor_.accessMod_ = stmt.access;
    for (astfri::ParamVarDefStmt* p : stmt.params) {
        p->accept(*this);
        this->currentConstructor_.params_.push_back(this->currentVariable_);
        this->currentVariable_.reset();
    }
    this->outputter_->add_constructor(currentConstructor_);
    this->currentConstructor_.reset();
}

void ClassVisitor::visit(const astfri::DestructorDefStmt& stmt) {
    this->currentDestructor_.class_ = stmt.owner->type->name;
    this->outputter_->add_destructor(currentDestructor_);
    this->currentDestructor_.reset();
}

void ClassVisitor::visit(const astfri::GenericParam& stmt) {
    this->currentClass_.genericParams_.push_back(stmt.name);
}

void ClassVisitor::visit(const astfri::ClassDefStmt& stmt) {
    this->currentClass_.name_ = stmt.type->name;
    if (this->config_->handleNamespaces_) {
        this->currentClass_.namespace_
            = astfri::mk_fqn(stmt.type->scope, this->currentClass_.name_);
    }
    this->currentClass_.type_ = UserDefinedType::CLASS;

    for (astfri::GenericParam* gp : stmt.tparams) {
        gp->accept(*this);
    }

    this->outputter_->open_user_type(this->currentClass_);

    for (astfri::ConstructorDefStmt* constructor : stmt.constructors) {
        constructor->accept(*this);
    }

    for (astfri::DestructorDefStmt* destructor : stmt.destructors) {
        destructor->accept(*this);
    }

    for (astfri::MemberVarDefStmt* var : stmt.vars) {
        var->accept(*this);
    }

    for (astfri::MethodDefStmt* method : stmt.methods) {
        method->accept(*this);
    }

    this->currentClass_.reset();
    this->outputter_->close_user_type();
}

void ClassVisitor::visit(const astfri::InterfaceDefStmt& stmt) {
    this->currentClass_.name_ = stmt.type->name;
    this->currentClass_.type_ = UserDefinedType::INTERFACE;

    for (astfri::GenericParam* gp : stmt.tparams) {
        this->currentClass_.genericParams_.push_back(gp->name);
    }

    this->outputter_->open_user_type(this->currentClass_);

    for (astfri::MethodDefStmt* method : stmt.methods) {
        method->accept(*this);
    }

    this->currentClass_.reset();
    this->outputter_->close_user_type();
}

void ClassVisitor::visit(const astfri::TranslationUnit& stmt) {
    // insert names of all classes in the TU into a set
    for (astfri::ClassDefStmt* c : stmt.classes) {
        this->classes_.insert(c->type->name);
    }

    // insert names of all interfaces in the TU into a set
    for (astfri::InterfaceDefStmt* i : stmt.interfaces) {
        this->interfaces_.insert(i->type->name);
    }

    // go through every class in the TU and create realations for it's base classes and interfaces
    for (astfri::ClassDefStmt* c : stmt.classes) {
        this->currentClass_.name_ = c->type->name;
        for (astfri::ClassDefStmt* base : c->bases) {
            this->create_relation(base->type->name, RelationType::EXTENSION);
        }

        for (astfri::InterfaceDefStmt* i : c->interfaces) {
            this->create_relation(i->type->name, RelationType::IMPLEMENTATION);
        }
        this->currentClass_.reset();
    }

    // go through every interface in the TU and create relations for it's base interfaces
    for (astfri::InterfaceDefStmt* i : stmt.interfaces) {
        this->currentClass_.name_ = i->type->name;
        for (astfri::InterfaceDefStmt* base : i->bases) {
            this->create_relation(base->type->name, RelationType::EXTENSION);
        }
        this->currentClass_.reset();
    }

    for (astfri::ClassDefStmt* c : stmt.classes) {
        c->accept(*this);
    }

    for (astfri::InterfaceDefStmt* i : stmt.interfaces) {
        i->accept(*this);
    }

    for (auto& [key, rel] : this->relations_) {
        this->outputter_->add_relation(rel);
    }

    this->finish();
}
} // namespace astfri::uml
