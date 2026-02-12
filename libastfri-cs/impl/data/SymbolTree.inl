#ifndef CSHARP_SYMBOL_TREE_INL
#define CSHARP_SYMBOL_TREE_INL

namespace astfri::csharp
{

template<typename T>
requires requires(ScopeNode::NodeData v) { std::get_if<T>(&v); }
T* ScopeNode::has_data()
{
    return std::get_if<T>(&data_);
}

template<typename T>
requires requires(ScopeNode::NodeData v) { std::get_if<T>(&v); }
T& ScopeNode::data()
{
    return std::get<T>(data_);
}

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TREE_INL