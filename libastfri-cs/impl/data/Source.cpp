#include <libastfri-cs/impl/data/Source.hpp>

#include <tree_sitter/api.h>

namespace astfri::csharp
{

SourceCode::SourceCode(SourceFile file, TSTree* tree) :
    file(std::move(file)),
    tree(tree)
{
}

const TSLanguage* SourceCode::lang() const
{
    return ts_tree_language(tree);
}

SourceCode::~SourceCode()
{
    if (tree)
    {
        ts_tree_delete(tree);
        tree = nullptr;
    }
}

SourceCode::SourceCode(SourceCode&& other) noexcept :
    fileContext(std::move(other.fileContext)),
    file(std::move(other.file)),
    tree(other.tree)
{
    other.tree = nullptr;
}

SourceCode& SourceCode::operator=(SourceCode&& other) noexcept
{
    if (this != &other)
    {
        if (tree)
            ts_tree_delete(tree);
        file        = std::move(other.file);
        fileContext = std::move(other.fileContext);
        tree        = other.tree;
        other.tree  = nullptr;
    }
    return *this;
}
} // namespace astfri::csharp