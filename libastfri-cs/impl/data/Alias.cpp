#include <libastfri-cs/impl/data/Alias.h>

namespace astfri::csharp
{
bool Alias::is_type(TypeBinding*& out) const
{
    if (const auto* nodeHandle = std::get_if<SymbolTree::ScopeNode*>(&target))
    {
        SymbolTree::ScopeNode* node = *nodeHandle;
        if (auto* type = std::get_if<TypeBinding>(&node->data))
        {
            out = type;
            return true;
        }
    }
    return false;
}

} // namespace astfri::csharp