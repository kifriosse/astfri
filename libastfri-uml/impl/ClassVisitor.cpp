#include <libastfri-uml/impl/ClassVisitor.hpp>

#include <string>

namespace astfri::uml
{
void ClassVisitor::create_relation(std::string target, RelationType type)
{
    if (! this->find_class(target) && ! this->find_interface(target))
        return;

    RelationStruct r;
    r.from_ = this->currentClass_.name_;
    r.to_   = target;
    r.type_ = type;

    if (! this->find_relation(r))
        this->relations_.emplace(r.from_ + r.to_, r);
}

bool ClassVisitor::find_relation(RelationStruct const& rel)
{
    return this->relations_.contains(rel.from_ + rel.to_);
}

bool ClassVisitor::find_class(std::string name)
{
    return this->classes_.contains(name);
}

bool ClassVisitor::find_interface(std::string name)
{
    return this->interfaces_.contains(name);
}

void ClassVisitor::finish()
{
    if (this->config_->writeToFile_)
    {
        this->outputter_->write_to_file();
    }
    else
    {
        this->outputter_->write_to_console();
    }
}

void ClassVisitor::set_config(Config const& config)
{
    this->config_ = (Config*)&config;
}

void ClassVisitor::set_outputter(UMLOutputter const& outputter)
{
    this->outputter_ = (UMLOutputter*)&outputter;
}

void ClassVisitor::visit(astfri::IntType const& /*type*/)
{
    this->currentVariable_.type_ = this->config_->intTypeName_;
}

void ClassVisitor::visit(astfri::FloatType const& /*type*/)
{
    this->currentVariable_.type_ = this->config_->floatTypeName_;
}

void ClassVisitor::visit(astfri::CharType const& /*type*/)
{
    this->currentVariable_.type_ = this->config_->charTypeName_;
}

void ClassVisitor::visit(astfri::BoolType const& /*type*/)
{
    this->currentVariable_.type_ = this->config_->boolTypeName_;
}

void ClassVisitor::visit(astfri::VoidType const& /*type*/)
{
    this->currentVariable_.type_ = this->config_->voidTypeName_;
}

void ClassVisitor::visit(astfri::ClassType const& type)
{
    if (type.name_.compare(this->currentClass_.name_) != 0)
    {
        this->create_relation(type.name_, RelationType::ASSOCIATION);
    }
    this->currentVariable_.type_ = type.name_;
}

void ClassVisitor::visit(astfri::InterfaceType const& type)
{
    if (type.name_.compare(this->currentClass_.name_) != 0)
    {
        this->create_relation(type.name_, RelationType::ASSOCIATION);
    }
    this->currentVariable_.type_ = type.name_;
}

void ClassVisitor::visit(astfri::IndirectionType const& type)
{
    this->currentVariable_.isIndirect_ = true;
    type.indirect_->accept(*this);
}

void ClassVisitor::visit(astfri::ParamVarDefStmt const& stmt)
{
    stmt.type_->accept(*this);
    this->currentVariable_.name_ = stmt.name_;
    // TODO - if (stmt.initializer_) stmt.initializer_->accept(*this);
}

void ClassVisitor::visit(astfri::MemberVarDefStmt const& stmt)
{
    if (! this->config_->innerView_ && stmt.access_ == astfri::AccessModifier::Private)
        return;
    stmt.type_->accept(*this);
    this->currentVariable_.name_      = stmt.name_;
    this->currentVariable_.accessMod_ = stmt.access_;
    // TODO - if (stmt.initializer_) stmt.initializer_->accept(*this);
    this->outputter_->add_data_member(this->currentVariable_);
    this->currentVariable_.reset();
}

void ClassVisitor::visit(astfri::FunctionDefStmt const& stmt)
{
    stmt.retType_->accept(*this);
    this->currentMethod_.retType_          = this->currentVariable_.type_;
    this->currentMethod_.returnIsIndirect_ = this->currentVariable_.isIndirect_;
    this->currentMethod_.name_             = stmt.name_;
    for (astfri::ParamVarDefStmt* p : stmt.params_)
    {
        p->accept(*this);
        this->currentMethod_.params_.push_back(this->currentVariable_);
        this->currentVariable_.reset();
    }
}

void ClassVisitor::visit(astfri::MethodDefStmt const& stmt)
{
    if (! this->config_->innerView_ && stmt.access_ == astfri::AccessModifier::Private)
        return;
    stmt.func_->accept(*this);
    this->currentMethod_.accessMod_ = stmt.access_;
    this->outputter_->add_function_member(this->currentMethod_);
    this->currentMethod_.reset();
}

void ClassVisitor::visit(astfri::ConstructorDefStmt const& stmt)
{
    this->currentConstructor_.class_ = stmt.owner_->type_->name_;
    this->currentConstructor_.accessMod_ = stmt.access_;
    for (astfri::ParamVarDefStmt* p : stmt.params_)
    {
        p->accept(*this);
        this->currentConstructor_.params_.push_back(this->currentVariable_);
        this->currentVariable_.reset();
    }
    this->outputter_->add_constructor(currentConstructor_);
    this->currentConstructor_.reset();
}

void ClassVisitor::visit(astfri::DestructorDefStmt const& stmt)
{
    this->currentDestructor_.class_ = stmt.owner_->type_->name_;
    this->outputter_->add_destructor(currentDestructor_);
    this->currentDestructor_.reset();
}

void ClassVisitor::visit(astfri::GenericParam const& stmt)
{
    this->currentClass_.genericParams_.push_back(stmt.name_);
}

void ClassVisitor::visit(astfri::ClassDefStmt const& stmt)
{
    this->currentClass_.name_ = stmt.type_->name_;
    this->currentClass_.type_ = UserDefinedType::CLASS;

    for (astfri::GenericParam* gp : stmt.tparams_)
    {
        gp->accept(*this);
    }

    this->outputter_->open_user_type(this->currentClass_);

    for (astfri::ConstructorDefStmt* constructor : stmt.constructors_)
    {
        constructor->accept(*this);
    }

    for (astfri::DestructorDefStmt* destructor : stmt.destructors_)
    {
        destructor->accept(*this);
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
    this->outputter_->close_user_type();
}

void ClassVisitor::visit(astfri::InterfaceDefStmt const& stmt)
{
    this->currentClass_.name_ = stmt.name_;
    this->currentClass_.type_ = UserDefinedType::INTERFACE;

    for (astfri::GenericParam* gp : stmt.tparams_)
    {
        this->currentClass_.genericParams_.push_back(gp->name_);
    }

    this->outputter_->open_user_type(this->currentClass_);

    for (astfri::MethodDefStmt* method : stmt.methods_)
    {
        method->accept(*this);
    }

    this->currentClass_.reset();
    this->outputter_->close_user_type();
}

void ClassVisitor::visit(astfri::TranslationUnit const& stmt)
{
    // insert names of all classes in the TU into a set
    for (astfri::ClassDefStmt* c : stmt.classes_)
    {
        this->classes_.insert(c->type_->name_);
    }

    // insert names of all interfaces in the TU into a set
    for (astfri::InterfaceDefStmt* i : stmt.interfaces_)
    {
        this->interfaces_.insert(i->name_);
    }

    // go through every class in the TU and create realations for it's base classes and interfaces
    for (astfri::ClassDefStmt* c : stmt.classes_)
    {
        this->currentClass_.name_ = c->type_->name_;
        for (astfri::ClassDefStmt* base : c->bases_)
        {
            this->create_relation(base->type_->name_, RelationType::EXTENSION);
        }

        for (astfri::InterfaceDefStmt* i : c->interfaces_)
        {
            this->create_relation(i->name_, RelationType::IMPLEMENTATION);
        }
        this->currentClass_.reset();
    }

    // go through every interface in the TU and create relations for it's base interfaces
    for (astfri::InterfaceDefStmt* i : stmt.interfaces_)
    {
        this->currentClass_.name_ = i->name_;
        for (astfri::InterfaceDefStmt* base : i->bases_)
        {
            this->create_relation(base->name_, RelationType::EXTENSION);
        }
        this->currentClass_.reset();
    }

    for (astfri::ClassDefStmt* c : stmt.classes_)
    {
        c->accept(*this);
    }

    for (astfri::InterfaceDefStmt* i : stmt.interfaces_)
    {
        i->accept(*this);
    }

    for (auto& [key, rel] : this->relations_)
    {
        this->outputter_->add_relation(rel);
    }

    this->finish();
}
} // namespace astfri::uml
