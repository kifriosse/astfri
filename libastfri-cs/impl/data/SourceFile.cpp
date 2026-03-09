#include <libastfri-cs/impl/data/SourceFile.hpp>

#include <tree_sitter/api.h>

namespace astfri::csharp
{

SourceFile::SourceFile(std::string src, TSTree* tree, Scope fileNms) :
    fileNms(std::move(fileNms)),
    srcStr(std::move(src)),
    tree(tree)
{
}

SourceFile::~SourceFile()
{
    if (tree)
    {
        ts_tree_delete(tree);
        tree = nullptr;
    }
}

SourceFile::SourceFile(SourceFile&& other) noexcept :
    usings(std::move(other.usings)),
    fileNms(std::move(other.fileNms)),
    srcStr(std::move(other.srcStr)),
    tree(other.tree)
{
    other.tree = nullptr;
}

SourceFile& SourceFile::operator=(SourceFile&& other) noexcept
{
    if (this != &other)
    {
        if (tree)
            ts_tree_delete(tree);
        srcStr     = std::move(other.srcStr);
        usings     = std::move(other.usings);
        fileNms    = std::move(other.fileNms);
        tree       = other.tree;
        other.tree = nullptr;
    }
    return *this;
}
} // namespace astfri::csharp