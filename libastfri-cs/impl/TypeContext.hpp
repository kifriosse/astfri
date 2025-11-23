#ifndef CSHARP_TYPE_CONTEXT
#define CSHARP_TYPE_CONTEXT

#include <libastfri/inc/Astfri.hpp>

#include <optional>
#include <stack>

namespace astfri::csharp
{

class TypeContext
{
private:
    std::stack<UserTypeDefStmt*> type_stack_;

public:
    void enter_type(UserTypeDefStmt* def_stmt);
    void leave_type();
    std::optional<UserTypeDefStmt*> top();
};

} // namespace astfri::csharp

#endif // CSHARP_TYPE_CONTEXT