#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/TypeTranslator.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

namespace astfri::csharp
{

TypeFactory& TypeTranslator::type_f_ = TypeFactory::get_instance();

TypeTranslator::TypeTranslator(SymbolTable& symbol_table) :
    symb_table_(symbol_table),
    lang_(tree_sitter_c_sharp())
{
}

void TypeTranslator::set_current_src(SourceCode* src)
{
    current_src_ = src;
}

void TypeTranslator::set_current_namespace(Scope scope)
{
    current_ns_ = std::move(scope);
}

Type* TypeTranslator::visit_predefined(TypeTranslator* self, const TSNode& node)
{
    const std::string name = util::extract_text(node, self->src_str());
    const auto result      = RegManager::get_type(name);
    return result ? *result : type_f_.mk_unknown();
}

Type* TypeTranslator::visit_identitifier(
    TypeTranslator* self,
    const TSNode& node
)
{
    const std::string name = util::extract_text(node, self->src_str());
    if (const auto type_opt = RegManager::get_type(name))
        return *type_opt;
    FileContext& file_context = self->src()->file_context;

    // Scope current = self->current_namespace_;
    // todo check aliases

    // current namespace and its parents
    const SymbolTree& symb_tree = self->symb_table_.symb_tree;
    if (const auto type = symb_tree.find_type(self->current_ns_, name, true))
        return type->type;

    // file scoped usings
    for (Scope& scope : file_context.usings)
    {
        if (const auto type = symb_tree.find_type(scope, name))
            return type->type;
    }
    // todo global aliases

    // global usings
    for (Scope& scope : self->symb_table_.glob_usings)
    {
        if (const auto type = symb_tree.find_type(scope, name))
            return type->type;
    }

    // global scope - root
    if (const auto type = symb_tree.find_type({}, name))
        return type->type;

    return type_f_.mk_unknown(); // todo make this incomplete type
}

Type* TypeTranslator::visit_qualified_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_f_.mk_unknown();
}

Type* TypeTranslator::visit_implicit(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_f_.mk_unknown(); // todo add deduced type
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
    return type_f_.mk_indirect(th(self, n_type));
}

Type* TypeTranslator::visit_array(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_f_.mk_unknown();
}

Type* TypeTranslator::visit_generic_name(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_f_.mk_unknown();
}

Type* TypeTranslator::visit_tuple(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_f_.mk_unknown();
}

Type* TypeTranslator::visit_func_pointer(
    [[maybe_unused]] TypeTranslator* self,
    [[maybe_unused]] const TSNode& node
)
{
    return type_f_.mk_unknown();
}

SourceCode* TypeTranslator::src() const
{
    return current_src_
             ? current_src_
             : throw std::logic_error("Current source code is not set");
}

std::string_view TypeTranslator::src_str() const
{
    return src()->file.content;
}

} // namespace astfri::csharp
