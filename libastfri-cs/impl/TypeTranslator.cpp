#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/TypeTranslator.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

namespace astfri::csharp
{

TypeFactory& TypeTranslator::type_factory_ = TypeFactory::get_instance();

TypeTranslator::TypeTranslator(SymbolTable& symbol_table) :
    symb_table_(symbol_table)
{
}

void TypeTranslator::set_current_src(SourceCode* src)
{
    current_src = src;
}

void TypeTranslator::set_current_namespace(Scope scope)
{
    current_namespace_ = std::move(scope);
}

Type* TypeTranslator::visit_predefined(TypeTranslator* self, const TSNode& node)
{
    const std::string name = util::extract_node_text(node, self->get_src_str());
    const auto result      = RegManager::get_type(name);
    return result ? *result : type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_identitifier(
    TypeTranslator* self,
    const TSNode& node
)
{
    std::string t_name = util::extract_node_text(node, self->get_src_str());
    if (const auto res = RegManager::get_type(t_name))
        return *res;
    FileContext& file_context = self->get_src()->file_context;

    // Scope current = self->current_namespace_;
    // todo check aliases

    // todo check current namespace and its parents
    const SymbolTree& symb_tree = self->symb_table_.symb_tree;
    if (const auto type
        = symb_tree.find_type(self->current_namespace_, t_name, true))
        return type->type;

    // todo file scoped usings
    for (Scope& scope : file_context.usings)
    {
        if (const auto type = symb_tree.find_type(scope, t_name))
            return type->type;
    }
    // todo global aliases

    // todo global usings
    for (Scope& scope : self->symb_table_.glob_usings)
    {
        if (const auto type = symb_tree.find_type(scope, t_name))
            return type->type;
    }
    // todo global scope - root
    if (const auto type = symb_tree.find_type({}, t_name))
        return type->type;

    return type_factory_.mk_unknown(); // todo make this incomplete type
}

Type* TypeTranslator::visit_qualified_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_implicit(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown(); // todo add deduced type
}

Type* TypeTranslator::visit_wrapper(TypeTranslator* self, const TSNode& node)
{
    const TSNode n_type  = util::child_by_field_name(node, "type");
    const TypeHandler th = RegManager::get_type_handler(n_type);
    return th(self, n_type);
}

Type* TypeTranslator::visit_inderect(TypeTranslator* self, const TSNode& node)
{
    // todo add handling of readonly
    const TSNode n_type  = util::child_by_field_name(node, "type");
    const TypeHandler th = RegManager::get_type_handler(n_type);
    return type_factory_.mk_indirect(th(self, n_type));
}

Type* TypeTranslator::visit_array(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_generic_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_tuple(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

Type* TypeTranslator::visit_func_pointer(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_factory_.mk_unknown();
}

std::string_view TypeTranslator::get_src_str() const
{
    return current_src
             ? current_src->file.content
             : throw std::logic_error("Current source code is not set");
}

// todo remove this and replace with one lang variable
const TSLanguage* TypeTranslator::get_lang() const
{
    return current_src && current_src->tree
             ? ts_tree_language(current_src->tree)
             : throw std::logic_error("Current source code is not set");
}

SourceCode* TypeTranslator::get_src() const
{
    return current_src
             ? current_src
             : throw std::logic_error("Current source code is not set");
}

} // namespace astfri::csharp