#include <libastfri-cs/impl/data/SymbolTree.hpp>
#include <astfri/Astfri.hpp>

#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <variant>

namespace astfri::csharp {

Nms::Nms(std::string name) :
    name_(std::move(name)) {
}

void Nms::add_static_using(SourceFile* src, const TypeBinding type) {
    if (src) {
        auto [it, inserted] = staticUsings_.try_emplace(src);
        it->second.push_back(type);
    }
}

void Nms::add_alias(std::string aliasName, SourceFile* src, Alias alias) {
    if (src) {
        auto [idIt, _] = aliases_.try_emplace(std::move(aliasName));
        idIt->second.try_emplace(src, alias);
    }
}

const Alias* Nms::find_alias(const std::string_view aliasName, SourceFile* src) const {
    if (src) {
        const auto alisesIt = aliases_.find(aliasName);
        if (alisesIt != aliases_.end()) {
            const auto aliasIt = alisesIt->second.find(src);
            if (aliasIt != alisesIt->second.end())
                return &aliasIt->second;
        }
    }
    return nullptr;
}

std::span<const TypeBinding> Nms::get_static_usings(SourceFile* src) const {
    if (src) {
        const auto it = staticUsings_.find(src);
        if (it != staticUsings_.end())
            return it->second;
    }
    return {};
}

const std::string& Nms::get_name() const {
    return name_;
}

ScopeNode::ScopeNode(NodeData content, ScopeNode* parent) :
    data_(std::move(content)),
    parent_(parent) {
    if (auto* b = std::get_if<TypeBinding>(&data_))
        b->treeNode = this;
}

ScopeNode* ScopeNode::parent() const {
    return parent_;
}

ScopeNode* ScopeNode::find_child(const std::string_view childName) const {
    const auto it = children_.find(childName);
    return it != children_.end() ? it->second.get() : nullptr;
}

ScopeNode* ScopeNode::add_child(std::string name, NodeData content) {
    auto [it, inserted] = children_.try_emplace(std::move(name));
    if (inserted) {
        it->second = std::make_unique<ScopeNode>(std::move(content), this);
    }
    return it->second.get();
}

const ScopeNode::NodeData& ScopeNode::data() const {
    return data_;
}

SymbolTree::SymbolTree() :
    root_(std::make_unique<ScopeNode>(Nms({}))) {
}

ScopeNode* SymbolTree::root() const {
    return root_.get();
}

ScopeNode* SymbolTree::add_scope(const Scope& scope) {
    ScopeNode* current = root();
    for (const std::string& str : scope.names) {
        current = current->add_child(str, Nms(str));
    }
    return current;
}

ScopeNode* SymbolTree::add_type(const Scope& scope, const TypeBinding& tb) {
    ScopeNode* last = add_scope(scope);
    return last->add_child(tb.type->name, tb);
}

ScopeNode* SymbolTree::add_primitive(const std::string& name, CSPrimitiveType primitive) {
    static const Scope systemScope = mk_scope("System");
    ScopeNode* last                = add_scope(systemScope);
    return last->add_child(name, primitive);
}

ScopeNode* SymbolTree::find_node(const Scope& scope) const {
    return find_node({}, scope);
}

ScopeNode* SymbolTree::find_node(const Scope& start, const Scope& end) const {
    ScopeNode* current = root_.get();
    auto process       = [&current](const Scope& scope) -> bool {
        for (const std::string_view qualif : scope.names) {
            ScopeNode* child = current->find_child(qualif);
            if (! child)
                return false;

            current = child;
        }
        return true;
    };

    if (! process(start) || ! process(end))
        return nullptr;

    return current;
}

TypeBinding* type_from_alias(const Alias& alias) {
    if (const auto* nodeHandle = std::get_if<ScopeNode*>(&alias)) {
        ScopeNode* node = *nodeHandle;
        if (auto* binding = node->is_a<TypeBinding>()) {
            return binding;
        }
    }
    return nullptr;
}

} // namespace astfri::csharp