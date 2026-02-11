#include <libastfri-cs/impl/data/SourceFile.hpp>

#include <tree_sitter/api.h>

namespace astfri::csharp
{

SourceFile::SourceFile(FileContext context, std::string src, TSTree* tree) :
    fileContext(std::move(context)),
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
    fileContext(std::move(other.fileContext)),
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
        srcStr      = std::move(other.srcStr);
        fileContext = std::move(other.fileContext);
        tree        = other.tree;
        other.tree  = nullptr;
    }
    return *this;
}
} // namespace astfri::csharp