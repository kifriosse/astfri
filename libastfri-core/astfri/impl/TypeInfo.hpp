#ifndef ASTFRI_IMPL_TYPE_INFO_HPP
#define ASTFRI_IMPL_TYPE_INFO_HPP

#include <astfri/impl/ASTNodeType.hpp>


namespace astfri {


/**
 * @brief Base class for all AST nodes.
 *
 * Holds type tag of the node and implements visitor dispatch.
 */
struct ASTNode;


/**
 * @brief Returns true if @p node is @c T.
 * @param node Node to typecheck.
 * @return true iff @p node is @c T.
 */
template<typename T>
bool is(ASTNode *node);


/**
 * @brief Returns true if @p node is-a @c B.
 * @param node Node to typecheck.
 * @return true iff @p node is-a @c B.
 */
template<typename B>
bool is_a(ASTNode *node);


/**
 * @brief Returns @p node casted to @c T iff @p node is @c T, otherwise returns nullptr.
 * @param node Node to typecast.
 * @return Typecasted @p node iff @p node is @c T, otherwise nullptr.
 */
template<typename T>
T *as(ASTNode *node);


namespace detail {


/**
 * @brief Trait specialized for each node type tag.
 *
 * Holds static member @c value that returns type tag of the parent
 * or NodeType::NO_TYPE if node type has no parent.
 */
template<ASTNodeType T>
struct BaseOf {
};


/**
 * @brief Trait specialized for each node type.
 *
 * Holds static member @c value that returns type tag of the node.
 */
template<typename T>
struct TypeOf {
};


/**
 * @brief @c TypeOf specialization for @c ASTNode.
 */
template<>
struct TypeOf<ASTNode> {
    static constexpr ASTNodeType value{ASTNodeType::ASTNode};
};


/**
 * @brief @c BaseOf specialization for @c ASTNode which has no base.
 */
template<>
struct BaseOf<TypeOf<ASTNode>::value> {
    static constexpr ASTNodeType value{ASTNodeType::NO_TYPE};
};


/**
 * @brief Defines specializations of @c TypeOf and @c BaseOf for type T with base B.
 */
#define ASTFRI_REGISTER_TYPE(T,B)\
namespace detail {\
template<>\
struct TypeOf<T> {\
    static constexpr ::astfri::ASTNodeType value{::astfri::ASTNodeType::T};\
};\
\
\
template<>\
struct BaseOf<::astfri::detail::TypeOf<T>::value> {\
    static constexpr ::astfri::ASTNodeType value{::astfri::detail::TypeOf<B>::value};\
};\
}\


} // namespace detail


} // namespace astfri


#include <astfri/impl/TypeInfo.inl>


#endif
