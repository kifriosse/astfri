#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/visitors/TypeTranslator.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <ranges>

namespace astfri::csharp
{

TypeFactory& TypeTranslator::typeFact_ = TypeFactory::get_instance();

ScopeNode TypeTranslator::extMarkNode_(ExternalMarker{}, nullptr);

TypeTranslator::TypeTranslator(SymbolTable& symbTable) :
    symbTable_(symbTable),
    lang_(tree_sitter_c_sharp())
{
}

void TypeTranslator::set_current_src(SourceFile* src)
{
    currentSrc_ = src;
}

void TypeTranslator::set_current_namespace(ScopeNode* node)
{
    currentScope_ = node;
}

Type* TypeTranslator::visit_predefined(TypeTranslator* self, const TSNode& node)
{
    const std::string name = util::extract_text(node, self->src_str());
    const auto result      = RegManager::get_type(name);
    return result ? *result : typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_identitifier(
    TypeTranslator* self,
    const TSNode& node
)
{
    // look up order form language specification
    // https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/basic-concepts#781-general

    // return typeFact_.mk_unknown();
    const ScopeNode* typeNode = self->resolve_qualif_name(
        node,
        util::SearchScope::UserTypeRef,
        self->currentScope_
    );

    if (typeNode)
    {
        util::Overloaded overloaded{
            [](const TypeBinding& b) -> Type* { return b.type; },
            [](const Nms&) -> Type* { return typeFact_.mk_unknown(); },
            [](const ExternalMarker&) -> Type*
            {
                return typeFact_.mk_unknown(); // todo change to incomplete type
            }
        };
        return std::visit(overloaded, typeNode->data());
    }

    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_qualified_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    const ScopeNode* typeNode = self->resolve_qualif_name(
        node,
        util::SearchScope::UserTypeRef,
        self->currentScope_
    );

    if (typeNode)
    {
        util::Overloaded overloaded{
            [](const TypeBinding& b) -> Type* { return b.type; },
            [](const Nms&) -> Type* { return typeFact_.mk_unknown(); },
            [](const ExternalMarker&) -> Type*
            {
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
)
{
    return typeFact_.mk_unknown(); // todo add deduced type
}

Type* TypeTranslator::visit_wrapper(TypeTranslator* self, const TSNode& node)
{
    const TSNode nType   = util::child_by_field_name(node, "type");
    const TypeHandler th = RegManager::get_type_handler(nType);
    return th(self, nType);
}

Type* TypeTranslator::visit_inderect(TypeTranslator* self, const TSNode& node)
{
    // todo add handling of readonly
    const TSNode nType   = util::child_by_field_name(node, "type");
    const TypeHandler th = RegManager::get_type_handler(nType);
    return typeFact_.mk_indirect(th(self, nType));
}

Type* TypeTranslator::visit_array(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_generic_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_tuple(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return typeFact_.mk_unknown();
}

Type* TypeTranslator::visit_func_pointer(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return typeFact_.mk_unknown();
}

ScopeNode* TypeTranslator::resolve_qualif_name(
    const TSNode& nQualif,
    const util::SearchScope searchScope,
    ScopeNode* start
    // todo might need src parameter
) const
{
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
    static const TSSymbol sQualifName
        = util::symbol_for_name("qualified_name", true);
    static const TSSymbol sAliasQualfName
        = util::symbol_for_name("alias_qualified_name", true);

    const std::string_view srcStr = src_str();
    TSNode nCurrent               = nQualif;
    std::vector<TSNode> nQualifs;

    while (ts_node_symbol(nCurrent) == sQualifName)
    {
        nQualifs.push_back(util::child_by_field_name(nCurrent, "name"));
        nCurrent = util::child_by_field_name(nCurrent, "qualifier");
    }

    SymbolTree& symbTree  = symbTable_.symbTree;
    ScopeNode* entryPoint = start;
    bool hasExplicitAlias = false;
    if (ts_node_symbol(nCurrent) == sAliasQualfName)
    {
        nQualifs.push_back(util::child_by_field_name(nCurrent, "name"));
        const TSNode nAlias = util::child_by_field_name(nCurrent, "alias");
        const std::string aliasStr = util::extract_text(nAlias, srcStr);
        hasExplicitAlias           = true;
        if (aliasStr == "global")
        {
            entryPoint = symbTree.root();
        }
        else
        {
            const Alias* alias = resolve_explicit_alias(
                aliasStr,
                searchScope,
                start,
                currentSrc_
            );
            if (! alias)
                return nullptr;

            if (ScopeNode* const* node = std::get_if<ScopeNode*>(alias))
                entryPoint = *node;
            else
            {
                extMarkNode_.data<ExternalMarker>().fqn
                    = util::extract_text(nQualif, srcStr);
                return &extMarkNode_;
            }
            // todo handling of unresolved aliases - types/namespaces
            // outside project
        }
    }
    else
    {
        nQualifs.push_back(nCurrent);
    }

    const std::string qualifStr = util::extract_text(nQualifs.back(), srcStr);
    nQualifs.pop_back();

    if (! hasExplicitAlias)
    {
        entryPoint = find_entry_point(qualifStr, searchScope, start, src());
    }

    ScopeNode* currentNode = entryPoint;
    for (auto& nCurrentQualif : std::views::reverse(nQualifs))
    {
        if (! currentNode)
        {
            extMarkNode_.data<ExternalMarker>().fqn
                = util::extract_text(nQualif, srcStr);
            return &extMarkNode_;
        }
        std::string qualifName = util::extract_text(nCurrentQualif, srcStr);
        currentNode            = currentNode->find_child(qualifName);
    }

    return currentNode;
}

const Alias* TypeTranslator::resolve_explicit_alias(
    const std::string_view aliasName,
    const util::SearchScope searchScope,
    ScopeNode* start,
    SourceFile* src
) const
{
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

    const ScopeNode* root = symbTable_.symbTree.root();
    ScopeNode* current = searchScope == UserTypeRef ? start : start->parent();

    while (current && current != root)
    {
        if (const Nms* nms = current->has_data<Nms>())
        {
            if (const Alias* a = nms->find_alias(aliasName, src))
                return a;
        }
        current = current->parent();
    }

    if (current && current == root)
    {
        if (const Nms* nms = current->has_data<Nms>())
        {
            if (const Alias* a = nms->find_alias(aliasName, src))
                return a;
        }
    }

    if (searchScope == UserTypeRef || start != root)
    {
        const auto it = symbTable_.globAliases.find(aliasName);
        if (it != symbTable_.globAliases.end())
            return &it->second;
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
) const
{
    ScopeNode* current = start;
    using enum util::SearchScope;
    switch (searchScope)
    {
    case UserTypeRef:
    {
        // bottom up search from current scope
        if (ScopeNode* node = bottom_up_search(qualif, start, src))
            return node;
        // check global aliasis
        const auto globAliasIt = symbTable_.globAliases.find(qualif);
        if (globAliasIt != symbTable_.globAliases.end())
        {
            const Alias* alias = &globAliasIt->second;
            if (ScopeNode* const* node = std::get_if<ScopeNode*>(alias))
                return *node;
        }
        // check global static usings for nested types
        for (const auto& b : symbTable_.globStaticUsings)
        {
            if (ScopeNode* node = b.treeNode->find_child(qualif))
                return node;
        }
        // check file scoped using directives
        for (const auto usingDirective : src->fileContext.usings)
        {
            if (ScopeNode* node = usingDirective->find_child(qualif))
                return node;
        }
        // check global using directives
        for (const auto usingDirective : symbTable_.globUsings)
        {
            if (ScopeNode* node = usingDirective->find_child(qualif))
                return node;
        }

        break;
    }
    case LocalAlias:
    case LocalStaticUsing:
    {
        // check current scope for types
        if (ScopeNode* child = current->find_child(qualif))
            return child;
        // bottom-up search from current scope
        if (ScopeNode* node = bottom_up_search(qualif, start, src))
            return node;
        // check global static usings for nested types
        for (const auto& b : symbTable_.globStaticUsings)
        {
            if (ScopeNode* node = b.treeNode->find_child(qualif))
                return node;
        }
        break;
    }
    case FileUsing:
    case GlobStaticUsing:
    case GlobAlias:
    case GlobUsing:
    {
        if (ScopeNode* node = symbTable_.symbTree.root()->find_child(qualif))
            return node;
        break;
    }
    }

    return nullptr;
}

ScopeNode* TypeTranslator::bottom_up_search(
    const std::string_view qualif,
    ScopeNode* start,
    SourceFile* src
) const
{
    ScopeNode* current = start;
    // bottom-up search from current namespace to global namespace
    while (current)
    {
        // look into siblings - types defined in the same namespace/type
        if (ScopeNode* child = current->find_child(qualif))
            return child;

        // if node contains type look into nested types in parent classes
        if (const TypeBinding* b = current->has_data<TypeBinding>())
        {
            if (ScopeNode* inheretedChild = search_parents(qualif, *b))
                return inheretedChild;
        }
        // if node is namespace
        const Nms* nms = current->has_data<Nms>();
        if (! nms)
        {
            current = current->parent();
            continue;
        }

        // check aliases in current namespace
        if (const Alias* alias = nms->find_alias(qualif, src))
        {
            ScopeNode* const* node = std::get_if<ScopeNode*>(alias);
            return node ? *node : nullptr;
            // todo return external marker instead of nullptr
        }

        // check nested classes in static usings
        for (const TypeBinding& statUsing : nms->get_static_usings(src))
        {
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
) const
{
    ClassDefStmt* current = nullptr;
    if (auto* classDef = as_a<ClassDefStmt>(start.def))
    {
        current = classDef;
    }

    while (current)
    {
        if (! current->bases_.empty())
        {
            current     = current->bases_.back();
            auto itMeta = symbTable_.userTypeMetadata.find(current);
            if (itMeta == symbTable_.userTypeMetadata.end())
                continue;

            ScopeNode* parent = itMeta->second.scope;
            if (ScopeNode* nestedType = parent->find_child(qualif))
                return nestedType;
        }
        else
        {
            current = nullptr;
        }
    }
    return nullptr;
}

SourceFile* TypeTranslator::src() const
{
    return currentSrc_
             ? currentSrc_
             : throw std::logic_error("Current source code is not set");
}

std::string_view TypeTranslator::src_str() const
{
    return src()->srcStr;
}

} // namespace astfri::csharp
