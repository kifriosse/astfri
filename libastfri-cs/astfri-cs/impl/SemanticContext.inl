#ifndef CSHARP_SEMANTIC_CONTEXT_INL
#define CSHARP_SEMANTIC_CONTEXT_INL

namespace astfri::csharp {

inline auto SemanticContext::get_type_metadata() const {
    return symbTable_.get_type_metadata();
}

} // namespace astfri::csharp

#endif // CSHARP_SEMANTIC_CONTEXT_INL