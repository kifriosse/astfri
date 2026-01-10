
#include <libastfri-cs/impl/data/FileContext.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri/impl/Scope.hpp>
#include <libastfri/inc/Astfri.hpp>
#include <libastfri/inc/StmtFactory.hpp>

#include <iostream>
#include <stack>

namespace astfri::csharp
{

StmtFactory& FileContext::stmt_factory_ = StmtFactory::get_instance();

void FileContext::add_using_directive(
    const TSNode& node,
    [[maybe_unused]] const TSLanguage* lang,
    const std::string_view src_code
)
{
    // const static TSSymbol qualified_name_symb
    //     = util::symbol_for_name(lang, "qualified_name", true);
    // const static TSSymbol generic_name_symb
    //     = util::symbol_for_name(lang, "generic_name", true);

    const TSNode name_node = util::child_by_field_name(node, "name");
    // util::print_child_nodes_types(node, src_code, true);

    if (ts_node_is_null(name_node))
    {
        const TSNode qualifier_node = ts_node_named_child(node, 0);
        std::cout << ts_node_type(qualifier_node) << std::endl;
        util::print_child_nodes_types(qualifier_node, src_code);
        const std::string namespace_str
            = util::extract_node_text(qualifier_node, src_code);
        Scope current_scope;
        std::stack<std::string> scope_stack;
        util::split_namespace(scope_stack, namespace_str);
        while (! scope_stack.empty())
        {
            current_scope.names_.push_back(scope_stack.top());
            scope_stack.pop();
            namespaces_.push_back(current_scope);
        }
    }
    else
    {
        // todo aliases not implemented
        // const TSNode qualif_name_node = ts_node_named_child(node, 1);
        // std::cout << ts_node_type(qualif_name_node) << std::endl;
        // const std::string alias_name = util::extract_node_text(name_node, src_code);
        // if (ts_node_symbol(qualif_name_node) == qualified_name_symb)
        // {
        //     Scope scope = mk_scope(util::extract_node_text(qualif_name_node));
        // } else
        // {
        //
        // }

        // std::stack<std::string> scope_stack;
        // util::print_child_nodes_types(qualif_name_node, src_code);
        // util::split_namespace(scope_stack, qualifier);


        // if (ClassDefStmt* class_def = stmt_factory_.get_class_def(, ))
    }

}

Type* FileContext::make_type(const TSNode& node, Scope current_scope)
{
    return nullptr;
}

} // namespace astfri::csharp