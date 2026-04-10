#include <astfri/Astfri.hpp>

#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/visitors/TypeTranslator.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <ranges>

namespace astfri::csharp {

TypeFactory& TypeTranslator::typeFact_ = TypeFactory::get_instance();

ScopeNode TypeTranslator::extMarkNode_(ExternalMarker{}, nullptr);

TypeTranslator::TypeTranslator(SymbolTable& symbTable) :
    symbTable_(symbTable),
    lang_(tree_sitter_c_sharp()) {
}

void TypeTranslator::set_current_src(SourceFile* src) {
    currentSrc_ = src;
}

void TypeTranslator::set_current_namespace(ScopeNode* node) {
    currentScope_ = node;
}

Type* TypeTranslator::visit_predefined(TypeTranslator* self, const TSNode& node) {
    const std::string name = util::extract_text(node, self->src_str());
    const auto result      = MapManager::get_primitive_type(name);
    return result ? result : typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_identitifier(TypeTranslator* self, const TSNode& node) {
    const auto primitive
        = MapManager::get_primitive_type(util::extract_text(node, self->src_str()));
    if (primitive)
        return primitive;

    // look up order form language specification
    // https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/basic-concepts#781-general

    const ScopeNode* typeNode
        = self->resolve_qualif_name(node, util::SearchScope::UserTypeRef, self->currentScope_);

    if (typeNode) {
        util::Overloaded overloaded{
            [](const TypeBinding& b) -> Type* { return b.type; },
            [](const CSPrimitiveType& p) -> Type* { return p.primitiveType; },
            [](const Nms&) -> Type* { return typeFact_.mk_unknown(); },
            [](const ExternalMarker&) -> Type* {
                return typeFact_.mk_unknown(); // todo change to incomplete type
            },
        };
        return std::visit(overloaded, typeNode->data());
    }

    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_qualified_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
) {
    const ScopeNode* typeNode
        = self->resolve_qualif_name(node, util::SearchScope::UserTypeRef, self->currentScope_);

    if (typeNode) {
        util::Overloaded overloaded{
            [](const TypeBinding& b) -> Type* { return b.type; },
            [](const CSPrimitiveType& p) -> Type* { return p.primitiveType; },
            [](const Nms&) -> Type* { return typeFact_.mk_unknown(); },
            [](const ExternalMarker&) -> Type* {
                return typeFact_.mk_unknown(); // todo change to incomplete type
            }
        };
        return std::visit(overloaded, typeNode->data());
    }

    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_implicit(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
) {
    return typeFact_.mk_unknown(); // todo add deduced type
}

Type* TypeTranslator::visit_wrapper(TypeTranslator* self, const TSNode& node) {
    const TSNode nType  = util::child_by_field_name(node, "type");
    const TypeMapper th = MapManager::get_type_mapper(nType);
    return th(self, nType);
}

Type* TypeTranslator::visit_indirect(TypeTranslator* self, const TSNode& node) {
    // todo add handling of readonly
    const TSNode nType  = util::child_by_field_name(node, "type");
    const TypeMapper th = MapManager::get_type_mapper(nType);
    return typeFact_.mk_indirect(th(self, nType));
}

Type* TypeTranslator::visit_array(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
) {
    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_generic_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
) {
    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_tuple(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
) {
    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_func_pointer(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
) {
    return typeFact_.mk_unknown();
}

ScopeNode* TypeTranslator::resolve_qualif_name(
    const TSNode& nQualif,
    const util::SearchScope searchScope,
    ScopeNode* start
) const {
    /*
     * UserTypeRef
     * * types in current scope
     * * nested types in inhereted classes
     * * aliases in current scope
     * * static usings in current scope
     * * go to parent
     * * repeat
     * Local static using and local aliases
     * * types in current namespace
     * * types in parent namespace
     * * aliases in parent namespace
     * * static usings in parent namespace - loking for types
     * Global lookup (including file usings)
     * * Checks only external aliases
     */
    static const TSSymbol sQualifName = MapManager::get_symbol(NodeType::QualifName);

    const std::string_view srcStr     = src_str();
    TSNode nCurrent                   = nQualif;
    std::vector<TSNode> nQualifs;

    TSSymbol sCurrent = ts_node_symbol(nCurrent);
    while (sCurrent == sQualifName) {
        nQualifs.push_back(util::child_by_field_name(nCurrent, "name"));
        nCurrent = util::child_by_field_name(nCurrent, "qualifier");
        sCurrent = ts_node_symbol(nCurrent);
    }

    const SymbolTree& symbTree = symbTable_.symb_tree();
    ScopeNode* entryPoint      = start;
    bool hasExplicitAlias      = false;
    if (sCurrent == MapManager::get_symbol(NodeType::AliasQualifName)) {
        nQualifs.push_back(util::child_by_field_name(nCurrent, "name"));
        const TSNode nAlias        = util::child_by_field_name(nCurrent, "alias");
        const std::string aliasStr = util::extract_text(nAlias, srcStr);
        hasExplicitAlias           = true;
        if (aliasStr == "global") {
            entryPoint = symbTree.root();
        }
        else {
            const Alias* alias = resolve_explicit_alias(aliasStr, searchScope, start, currentSrc_);
            if (! alias)
                return nullptr;

            if (ScopeNode* const* node = std::get_if<ScopeNode*>(alias))
                entryPoint = *node;
            else {
                extMarkNode_.data<ExternalMarker>().qualifiedName
                    = util::extract_text(nQualif, srcStr);
                return &extMarkNode_;
            }
        }
    }
    else {
        nQualifs.push_back(nCurrent);
    }

    const std::string qualifStr = util::extract_text(nQualifs.back(), srcStr);
    nQualifs.pop_back();

    if (! hasExplicitAlias) {
        entryPoint = find_entry_point(qualifStr, searchScope, start, src());
    }

    ScopeNode* currentNode = entryPoint;
    for (auto& nCurrentQualif : std::views::reverse(nQualifs)) {
        if (! currentNode) {
            extMarkNode_.data<ExternalMarker>().qualifiedName = util::extract_text(nQualif, srcStr);
            return &extMarkNode_;
        }
        std::string qualifName = util::extract_text(nCurrentQualif, srcStr);
        currentNode            = currentNode->find_child(qualifName);
    }

    return currentNode;
}

TypeBinding* TypeTranslator::get_type(const std::string_view name, const Scope& scope) const {
    const ScopeNode* node = symbTable_.symb_tree().find_node(scope);
    if (ScopeNode* typeNode = node->find_child(name)) {
        return typeNode->is_a<TypeBinding>();
    }
    return nullptr;
}

const Alias* TypeTranslator::resolve_explicit_alias(
    const std::string_view aliasName,
    const util::SearchScope searchScope,
    ScopeNode* start,
    SourceFile* src
) const {
    /*
     * Scoped type
     * * aliases in current namespace
     * Local static using and local aliases
     * * aliases in parent namespace
     * * global aliases - if we didn't start in global namespace
     * Global lookup (including file usings)
     * * Checks only external aliases
     */
    using enum util::SearchScope;

    if (aliasName == "global")
        return nullptr;

    const ScopeNode* root = symbTable_.symb_tree().root();
    ScopeNode* current    = searchScope == UserTypeRef ? start : start->parent();

    while (current && current != root) {
        if (const Nms* nms = current->is_a<Nms>()) {
            if (const Alias* a = nms->find_alias(aliasName, src))
                return a;
        }
        current = current->parent();
    }

    if (current && current == root) {
        if (const Nms* nms = current->is_a<Nms>()) {
            if (const Alias* a = nms->find_alias(aliasName, src))
                return a;
        }
    }

    if (searchScope == UserTypeRef || start != root) {
        return symbTable_.get_glob_alias(aliasName);
    }

    // todo handling of unresolved aliases - types/namespaces outside project
    // todo external aliases

    return nullptr;
}

ScopeNode* TypeTranslator::find_entry_point(
    const std::string_view qualif,
    const util::SearchScope searchScope,
    ScopeNode* start,
    SourceFile* src
) const {
    if (! start)
        return nullptr;

    ScopeNode* current = start;
    using enum util::SearchScope;
    switch (searchScope) {
    case UserTypeRef: {
        // bottom up search from current scope
        if (ScopeNode* node = bottom_up_search(qualif, start, src))
            return node;
        // check global aliasis
        if (const auto* alias = symbTable_.get_glob_alias(qualif)) {
            if (ScopeNode* const* node = std::get_if<ScopeNode*>(alias))
                return *node;
        }
        // check global static usings for nested types
        for (const auto& b : symbTable_.get_glob_static_usings()) {
            if (ScopeNode* node = b.treeNode->find_child(qualif))
                return node;
        }
        // check file scoped using directives
        for (const auto* usingDirective : src->usings) {
            if (ScopeNode* node = usingDirective->find_child(qualif))
                return node;
        }
        // check global using directives
        for (const auto* usingDirective : symbTable_.get_glob_usings()) {
            if (ScopeNode* node = usingDirective->find_child(qualif))
                return node;
        }

        break;
    }
    case LocalAlias:
    case LocalStaticUsing: {
        // check current scope for types
        if (ScopeNode* child = current->find_child(qualif))
            return child;
        // bottom-up search from current scope
        if (ScopeNode* node = bottom_up_search(qualif, start, src))
            return node;
        // check global static usings for nested types
        for (const auto& b : symbTable_.get_glob_static_usings()) {
            if (ScopeNode* node = b.treeNode->find_child(qualif))
                return node;
        }
        break;
    }
    case FileUsing:
    case GlobStaticUsing:
    case GlobAlias:
    case GlobUsing: {
        auto [names] = util::mk_scope(qualif);
        current = symbTable_.symb_tree().root();
        for (const std::string_view name: names) {
            current = current->find_child(name);
            if (! current)
                break;
        }
        return current;
    }
    }

    return nullptr;
}

ScopeNode* TypeTranslator::bottom_up_search(
    const std::string_view qualif,
    ScopeNode* start,
    SourceFile* src
) const {
    ScopeNode* current = start;
    // bottom-up search from current namespace to global namespace
    while (current) {
        // look into siblings - types defined in the same namespace/type
        if (ScopeNode* child = current->find_child(qualif))
            return child;

        // if node contains type look into nested types in parent classes
        if (const TypeBinding* b = current->is_a<TypeBinding>()) {
            if (ScopeNode* inheretedChild = search_parents(qualif, *b))
                return inheretedChild;
        }
        // if node is namespace
        const Nms* nms = current->is_a<Nms>();
        if (! nms) {
            current = current->parent();
            continue;
        }

        // check aliases in current namespace
        if (const Alias* alias = nms->find_alias(qualif, src)) {
            ScopeNode* const* node = std::get_if<ScopeNode*>(alias);
            return node ? *node : nullptr;
            // todo return external marker instead of nullptr
        }

        // check nested classes in static usings
        for (const TypeBinding& statUsing : nms->get_static_usings(src)) {
            if (ScopeNode* node = statUsing.treeNode->find_child(qualif))
                return node;
        }
        current = current->parent();
    }
    return nullptr;
}

ScopeNode* TypeTranslator::search_parents(
    const std::string_view qualif,
    const TypeBinding& start
) const {
    ClassDefStmt* current = nullptr;
    if (auto* classDef = as_a<ClassDefStmt>(start.def)) {
        current = classDef;
    }

    while (current) {
        if (! current->bases.empty()) {
            current             = current->bases.back();
            const auto metadata = symbTable_.get_type_metadata(current);
            if (! metadata)
                continue;

            const ScopeNode* scope = metadata->type_binding().treeNode;
            if (ScopeNode* nestedType = scope->find_child(qualif))
                return nestedType;
        }
        else {
            current = nullptr;
        }
    }
    return nullptr;
}

std::string_view TypeTranslator::src_str() const {
    return src()->srcStr;
}

SourceFile* TypeTranslator::src() const {
    return currentSrc_ ? currentSrc_ : throw std::logic_error("Current source code is not set");
}

} // namespace astfri::csharp
