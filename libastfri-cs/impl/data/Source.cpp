#include <libastfri-cs/impl/data/Source.hpp>

namespace astfri::csharp
{
SourceCode::SourceCode(SourceFile file, TSTree* tree) :
    file(std::move(file)),
    tree(tree)
{
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
        file       = std::move(other.file);
        tree       = other.tree;
        other.tree = nullptr;
    }
    return *this;
}
} // namespace astfri::csharp