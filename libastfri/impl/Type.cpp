#include <libastfri/inc/Type.hpp>

namespace astfri
{

UserType::UserType(std::string name) :
    name_(std::move(name))
{
}

IndirectionType::IndirectionType(Type* const indirect) :
    indirect_(indirect)
{
}

ScopedType::ScopedType(std::string name, Scope scope) :
    name_(std::move(name)),
    scope_(std::move(scope))
{
}

ClassType::ClassType(std::string name, Scope scope, ClassDefStmt *def) :
    ScopedType(name, scope),
    m_def(def)
{
}

InterfaceType::InterfaceType(std::string name, Scope scope, InterfaceDefStmt *def) :
    ScopedType(name, scope),
    m_def(def)
{
}

LambdaType::LambdaType(std::string name, LambdaExpr *def) :
    m_name(std::move(name)),
    m_def(def)
{
}

} // namespace astfri
