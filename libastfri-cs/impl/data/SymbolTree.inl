#ifndef CSHARP_SYMBOL_TREE_INL
#define CSHARP_SYMBOL_TREE_INL

namespace astfri::csharp
{

template<typename T>
requires requires(SymbolNode::Content v) { std::get_if<T>(&v); }
T* SymbolNode::is_content()
{
    return std::get_if<T>(&content_);
}

}

#endif