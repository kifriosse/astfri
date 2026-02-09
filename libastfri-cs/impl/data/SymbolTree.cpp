#include <libastfri-cs/impl/data/SymbolTree.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <string_view>
#include <variant>

namespace astfri::csharp
{

Nms::Nms(std::string name) : name_(std::move(name))
{
}

SymbolNode::SymbolNode(Content content, SymbolNode* parent) :
    content_(std::move(content)),
    parent_(parent)
{
}

SymbolNode* SymbolNode::parent()
{
    return parent_;
}

SymbolNode* SymbolNode::find_child(const std::string_view childName)
{
    const auto it = children_.find(childName);
    return it != children_.end() ? it->second.get() : nullptr;
}

SymbolNode* SymbolNode::try_add_child(
    std::string name,
    Content content,
    SymbolNode* parent
)
{
    auto [it, inserted] = children_.try_emplace(std::move(name));
    if (inserted)
    {
        it->second = std::make_unique<SymbolNode>(std::move(content), parent);
    }
    return it->second.get();
}

SymbolTree::SymbolTree() :
    root_(std::make_unique<SymbolNode>(Nms({})))
{
}

SymbolNode* SymbolTree::root()
{
    return root_.get();
}

SymbolNode* SymbolTree::add_scope(const Scope& scope)
{
    SymbolNode* current = root();
    for (const std::string& str : scope.names_)
    {
        current = current->try_add_child(str, Nms(str), current);
    }
    return current;
}

SymbolNode* SymbolTree::add_type(const Scope& scope, TypeBinding typeBinding)
{
    SymbolNode* last = add_scope(scope);
    return last->try_add_child(typeBinding.type->name_, typeBinding, last);
}

TypeBinding* SymbolTree::find_type(
    const Scope& scope,
    const std::string_view typeName,
    const bool searchParents
) const
{
    return find_type({}, scope, typeName, searchParents);
}

TypeBinding* SymbolTree::find_type(
    const Scope& start,
    const Scope& end,
    const std::string_view typeName,
    const bool searchParents
) const
{
    SymbolTreeCursor cursor(*root_);
    auto process = [&cursor](const Scope& scope) -> bool
    {
        for (const std::string_view qualif : scope.names_)
        {
            if (! cursor.go_to_child(qualif))
                return false;
        }
        return true;
    };

    if (! process(start) || ! process(end))
        return nullptr;

    if (searchParents)
        return find_type(*cursor.current(), typeName);

    SymbolNode* childNode = cursor.current()->find_child(typeName);
    return childNode->is_content<TypeBinding>();
}

TypeBinding* SymbolTree::find_type(
    SymbolNode& start,
    const std::string_view typeName
)
{
    SymbolTreeCursor cursor(start);
    do
    {
        if (auto* node = cursor.current()->find_child(typeName))
            return node->is_content<TypeBinding>();

    } while (cursor.go_to_parent());

    return nullptr;
}

SymbolTreeCursor::SymbolTreeCursor(SymbolNode& root) : current_(&root)
{
}

SymbolNode* SymbolTreeCursor::current()
{
    return current_;
}

bool SymbolTreeCursor::go_to_parent()
{
    if (current_->parent())
    {
        current_ = current_->parent();
        return true;
    }
    return false;
}

bool SymbolTreeCursor::go_to_child(const std::string_view childName)
{
    if (SymbolNode* child = current_->find_child(childName))
    {
        current_ = child;
        return true;
    }
    return false;
}

TypeBinding* type_from_alias(const Alias& alias)
{
    if (const auto* nodeHandle = std::get_if<SymbolNode*>(&alias))
    {
        SymbolNode* node = *nodeHandle;
        if (auto* binding = node->is_content<TypeBinding>())
        {
            return binding;
        }
    }
    return nullptr;
}

} // namespace astfri::csharp