
#include "ScopeContext.hpp"


namespace astfri::csharp {

Scope ScopeContext::make_scope() const
{
    if (namespace_stack.empty() && type_stack.empty())
        return {};

    std::vector<std::string> scope_str;
    for (std::string const& namespace_v : namespace_stack)
    {
        size_t const str_len = namespace_v.length();
        size_t start = 0;
        size_t end = namespace_v.find(".", start, str_len);
        while (end != std::string::npos)
        {
            scope_str.emplace_back(namespace_v.substr(start, end));
            start = end;
            end = namespace_v.find(".", start, str_len);
        }
    }

    for (ClassDefStmt const* class_def : type_stack)
    {
        scope_str.push_back(class_def->name_);
    }

    return {scope_str};
}

void ScopeContext::enter_namespace(std::string const& namespace_name)
{
    namespace_stack.push_back(namespace_name);
}

void ScopeContext::enter_class(ClassDefStmt* class_def)
{
    type_stack.push_back(class_def);
}

void ScopeContext::leave_namespace()
{
    namespace_stack.pop_back();
}

void ScopeContext::leave_class()
{
    type_stack.pop_back();
}

} // namespace astfri::csharp