
#include "TypeResolver.hpp"

#include <filesystem>
#include <fstream>
#include <stack>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include "utils.hpp"

namespace astfri::csharp
{

void TypeRegistry::add_type(TypeInfo const& info)
{
    auto const& [names_] = info.type_->scope_;
    NamespaceNode* current = &global_namespace_;
    for (auto& namespace_name : names_)
    {
        auto const it = current->namespaces_.find(namespace_name);
        if (it != current->namespaces_.end())
        {
            current = &it->second;
            continue;
        }
        current->namespaces_.emplace(namespace_name, NamespaceNode{});
        current = &current->namespaces_[namespace_name];
    }
    global_namespace_.types_.emplace(info.type_->name_, info);
}

NamespaceNode& TypeRegistry::root()
{
    return global_namespace_;
}

std::unordered_map<std::string, DeclarationType> TypeResolver::decl_type_map_ = {
    {"class_declaration",      DeclarationType::Class},
    { "interface_declaration", DeclarationType::Interface},
    { "struct_declaration",    DeclarationType::Struct},
    { "enum_declaration",      DeclarationType::Enum},
    { "delegate_declaration",  DeclarationType::Delegate}
};

void TypeResolver::find_types_file(std::string const& file)
{
    std::ifstream file_stream(file);
    std::string const content(
        (std::istreambuf_iterator<char>(file_stream)),
        std::istreambuf_iterator<char>()
    );
    find_types_src(content);
}

void TypeResolver::find_types_src(std::string const& source_code)
{
    TSLanguage const* language = tree_sitter_c_sharp();
    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, language);
    TSTree* tree = ts_parser_parse_string(
        parser,
        nullptr,
        source_code.c_str(),
        source_code.length()
    );
    TSNode const root = ts_tree_root_node(tree);

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

    std::vector<TSNode> decl_nodes = find_nodes(
        root,
        language,
        type_decl_query
    );
    // todo not finished
    // for (TSNode& decl_node : decl_nodes)
    // {
    //
    // }
}

void TypeResolver::find_types_project(std::string const& project_dir)
{
    std::stack<std::string> dirs;
    dirs.push(project_dir);

    while (! dirs.empty())
    {
        auto dir_it = std::filesystem::directory_iterator(dirs.top());
        dirs.pop();
        for (auto const& dir_entry : dir_it)
        {
            if (dir_entry.is_directory())
            {
                auto const& path = dir_entry.path();
                if (path == project_dir + "/bin" || path == project_dir + "/obj")
                {
                    continue;
                }
                dirs.push(path);
            }
            else if (dir_entry.path().extension() == ".cs")
            {
                find_types_file(dir_entry.path());
            }
        }
    }
}

void TypeResolver::load_external_types(std::string const& json_file)
{

}

} // namespace astfri::csharp
