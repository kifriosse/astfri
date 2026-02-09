#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/visitors/TypeTranslator.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

namespace astfri::csharp
{

TypeFactory& TypeTranslator::typeFact_ = TypeFactory::get_instance();

TypeTranslator::TypeTranslator(SymbolTable& symbTable) :
    symbTable_(symbTable),
    lang_(tree_sitter_c_sharp())
{
}

void TypeTranslator::set_current_src(SourceFile* src)
{
    currentSrc_ = src;
}

void TypeTranslator::set_current_namespace(SymbolNode* node)
{
    currentNmsNode_ = node;
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

    // todo generic types - generic parameter identifiers

    // todo nested types of inherited types - doesn't include interfaces

    // todo incorect finding of aliases and types
    const std::string name = util::extract_text(node, self->src_str());
    if (const auto typeOpt = RegManager::get_type(name))
        return *typeOpt;
    FileContext& fileContext = self->src()->fileContext;

    // todo check aliases

    // current namespace and its parents
    const SymbolTree& symbTree = self->symbTable_.symbTree;
    if (const auto type = SymbolTree::find_type(*self->currentNmsNode_, name))
        return type->type;

    // file scoped usings
    for (Scope& scope : fileContext.usings)
    {
        if (const auto type = symbTree.find_type(scope, name))
            return type->type;
    }

    // global usings
    for (Scope& scope : self->symbTable_.globUsings)
    {
        if (const auto type = symbTree.find_type(scope, name))
            return type->type;
    }
    // todo global aliases
    const auto itGlobAlias = self->symbTable_.globAliases.find(name);
    if (itGlobAlias != self->symbTable_.globAliases.end())
    {
        const Alias& alias = itGlobAlias->second;
        if (const TypeBinding* typeBinding = type_from_alias(alias))
            return typeBinding->type;

        return typeFact_.mk_unknown();
    }
    // global scope - root

    return typeFact_.mk_unknown(); // todo make this incomplete type
}

Type* TypeTranslator::visit_qualified_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
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
