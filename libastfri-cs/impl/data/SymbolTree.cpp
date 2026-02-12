#include <libastfri-cs/impl/data/SymbolTree.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <variant>

namespace astfri::csharp
{

Nms::Nms(std::string name) :
    name_(std::move(name))
{
}

void Nms::add_static_using(SourceFile* src, const TypeBinding type)
{
    if (src)
    {
        auto [it, inserted] = staticUsings_.try_emplace(src);
        it->second.push_back(type);
    }
}

void Nms::add_alias(std::string aliasName, SourceFile* src, Alias alias)
{
    if (src)
    {
        auto [idIt, _] = aliases_.try_emplace(std::move(aliasName));
        idIt->second.try_emplace(src, alias);
    }
}

const Alias* Nms::find_alias(
    const std::string_view aliasName,
    SourceFile* src
) const
{
    if (src)
    {
        const auto alisesIt = aliases_.find(aliasName);
        if (alisesIt != aliases_.end())
        {
            const auto aliasIt = alisesIt->second.find(src);
            if (aliasIt != alisesIt->second.end())
                return &aliasIt->second;
        }
    }
    return nullptr;
}

std::span<const TypeBinding> Nms::get_static_usings(SourceFile* src) const
{
    if (src)
    {
        const auto it = staticUsings_.find(src);
        if (it != staticUsings_.end())
            return it->second;
    }
    return {};
}

ScopeNode::ScopeNode(NodeData content, ScopeNode* parent) :
    data_(std::move(content)),
    parent_(parent)
{
    if (auto* b = std::get_if<TypeBinding>(&data_))
        b->treeNode = this;
}

ScopeNode* ScopeNode::parent() const
{
    return parent_;
}

ScopeNode* ScopeNode::find_child(const std::string_view childName)
{
    const auto it = children_.find(childName);
    return it != children_.end() ? it->second.get() : nullptr;
}

ScopeNode* ScopeNode::try_add_child(
    std::string name,
    NodeData content,
    ScopeNode* parent
)
{
    auto [it, inserted] = children_.try_emplace(std::move(name));
    if (inserted)
    {
        it->second = std::make_unique<ScopeNode>(std::move(content), parent);
    }
    return it->second.get();
}

const ScopeNode::NodeData& ScopeNode::data() const
{
    return data_;
}

SymbolTree::SymbolTree() :
    root_(std::make_unique<ScopeNode>(Nms({})))
{
}

ScopeNode* SymbolTree::root() const
{
    return root_.get();
}

ScopeNode* SymbolTree::add_scope(const Scope& scope)
{
    ScopeNode* current = root();
    for (const std::string& str : scope.names_)
    {
        current = current->try_add_child(str, Nms(str), current);
    }
    return current;
}

ScopeNode* SymbolTree::add_type(
    const Scope& scope,
    ScopedType* type,
    UserTypeDefStmt* def
)
{
    TypeBinding binding{
        .type = type,
        .def  = def,
    };
    ScopeNode* last    = add_scope(scope);
    ScopeNode* newNode = last->try_add_child(type->name_, binding, last);
    return newNode;
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
    ScopeNode* node = find_node(start, end);
    if (! node)
        return nullptr;

    if (searchParents)
        return find_type(*node, typeName);

    ScopeNode* childNode = node->find_child(typeName);
    return childNode->has_data<TypeBinding>();
}

TypeBinding* SymbolTree::find_type(
    ScopeNode& start,
    const std::string_view typeName
)
{
    SymbolTreeCursor cursor(start);
    do
    {
        if (auto* node = cursor.current()->find_child(typeName))
            return node->has_data<TypeBinding>();

    } while (cursor.go_to_parent());

    return nullptr;
}

ScopeNode* SymbolTree::find_node(const Scope& scope) const
{
    return find_node({}, scope);
}

ScopeNode* SymbolTree::find_node(const Scope& start, const Scope& end) const
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

    return cursor.current();
}

SymbolTreeCursor::SymbolTreeCursor(ScopeNode& root) :
    current_(&root)
{
}

ScopeNode* SymbolTreeCursor::current()
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
    if (ScopeNode* child = current_->find_child(childName))
    {
        current_ = child;
        return true;
    }
    return false;
}

TypeBinding* type_from_alias(const Alias& alias)
{
    if (const auto* nodeHandle = std::get_if<ScopeNode*>(&alias))
    {
        ScopeNode* node = *nodeHandle;
        if (auto* binding = node->has_data<TypeBinding>())
        {
            return binding;
        }
    }
    return nullptr;
}

} // namespace astfri::csharp