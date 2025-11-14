
#ifndef CSHARP_SCOPE_CONTEXT_HPP
#define CSHARP_SCOPE_CONTEXT_HPP

#include <string>
#include <vector>

#include "libastfri/impl/Scope.hpp"
#include "libastfri/inc/Stmt.hpp"

namespace astfri::csharp
{
class ScopeContext
{
private:
    std::vector<std::string> namespace_stack;
    std::vector<ClassDefStmt*> type_stack;
public:
    Scope make_scope() const;
    void enter_namespace(std::string const& namespace_name);
    void enter_class(ClassDefStmt* class_def);
    void leave_namespace();
    void leave_class();
};

}

#endif // CSHARP_SCOPE_CONTEXT_HPP