#include <libastfri-cs/impl/TypeContext.hpp>

namespace astfri::csharp
{

void TypeContext::enter_type(UserTypeDefStmt* def_stmt)
{
    type_stack_.push(def_stmt);
}

void TypeContext::leave_type()
{
    type_stack_.pop();
}

std::optional<UserTypeDefStmt*> TypeContext::top()
{
    if (type_stack_.empty())
        return {};
    return type_stack_.top();
}

} // namespace astfri::csharp
