#include <libastfri-cs/impl/CSharpTSTreeVisitor.hpp>

#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>

#include <algorithm>

namespace astfri::csharp
{

ExprFactory& CSharpTSTreeVisitor::expr_factory_ = ExprFactory::get_instance();
StmtFactory& CSharpTSTreeVisitor::stmt_factory_ = StmtFactory::get_instance();
TypeFactory& CSharpTSTreeVisitor::type_factory_ = TypeFactory::get_instance();

CSharpTSTreeVisitor::CSharpTSTreeVisitor(std::string source_code, TSLanguage const* language) :
    source_code_(std::move(source_code)),
    language_(language)
{
}

void CSharpTSTreeVisitor::handle_comp_unit_stmt(TranslationUnit& tr_unit, TSNode const* node)
{
    static std::string const type_decl_query = R"(
        (namespace_declaration
            body: (declaration_list
                (class_declaration) @class))
        (namespace_declaration
            body: (declaration_list
                (interface_declaration) @interface))
        (compilation_unit
            (class_declaration) @class)
        (compilation_unit
            (interface_declaration) @interface)
    )";

    std::vector<TSNode> const type_nodes     = find_nodes(*node, language_, type_decl_query);

    for (auto const& type_node : type_nodes)
    {
        StmtHandler handler = NodeRegistry::get_stmt_handler(type_node);
        Stmt* stmt          = handler(this, &type_node);
        // todo handle partial class
        if (is_a<ClassDefStmt>(stmt))
            tr_unit.classes_.push_back(as_a<ClassDefStmt>(stmt));
        else if (is_a<InterfaceDefStmt>(stmt))
            tr_unit.interfaces_.push_back(as_a<InterfaceDefStmt>(stmt));
    }
}

Type* CSharpTSTreeVisitor::make_type(CSharpTSTreeVisitor const* self, TSNode const& node)
{
    std::string type_name          = extract_node_text(node, self->source_code_);
    char const last_char           = type_name[type_name.length() - 1];
    bool const is_indirection_type = last_char == '*' || last_char == '&';
    if (is_indirection_type)
    {
        type_name.pop_back();
    }

    std::erase_if(type_name, isspace);
    auto const res = NodeRegistry::get_type(type_name);
    // todo implement scope
    Type* type = res.has_value() ? *res : type_factory_.mk_class(type_name, {});

    return is_indirection_type ? type_factory_.mk_indirect(type) : type;
}

} // namespace astfri::csharp