#include <libastfri-cs/impl/data/Alias.h>

namespace astfri::csharp
{
bool is_type_alias(const Alias& alias, TypeBinding*& out)
{
    if (const auto* nodeHandle = std::get_if<SymbolTree::ScopeNode*>(&alias))
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