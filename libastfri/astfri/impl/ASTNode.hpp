#ifndef ASTFRI_IMPL_AST_NODE_HPP
#define ASTFRI_IMPL_AST_NODE_HPP

#include <astfri/impl/TypeInfoFwd.hpp>
#include <astfri/impl/Kind.hpp>
#include <astfri/impl/Visitor.hpp>


namespace astfri {


namespace detail {


template<typename KindType>
struct ASTNode;


/**
 * @brief Initializes ASTNode.
 *
 * Initializes @c kind member of the @c ASTNode base.
 * Initializes @c m_accept_ptr member of the @c ASTNode base.
 *
 * @tparam KindType enum of kinds (ExprKind, StmtKind, TypeKind)
 * @tparam SelfType Real (childmost) type of @p node.
 * @param node Node to initialize.
 */
template<typename SelfType, typename KindType>
void initialize(ASTNode<KindType> &node);


/**
 * @brief Base class for all AST nodes.
 *
 * Holds kind of the node and implements visitor dispatch.
 *
 * @tparam KindType enum of kinds (ExprKind, StmtKind, TypeKind)
 */
template<typename KindType>
struct ASTNode {
    /**
     * @brief Accepts visitor by calling its corresponding @c visit method.
     * @param visitor Visitor to be accepted.
     */
    void accept(Visitor &visitor);

private:
    KindType kind; /**< Enum value holding kind (type) of this node. */
    void(*m_accept_ptr)(void*, Visitor&); /**< Pointer to a function implementing accept. */

private:
    template<typename S, typename K>
    friend void ::astfri::detail::initialize(ASTNode<K>&);

    template<typename T, typename K>
    friend bool ::astfri::is_a(ASTNode<K>*);

    template<typename T, typename K>
    friend T *::astfri::as_a(ASTNode<K>*);
};


/**
 * @brief Helper CRTP base. Indirectly inherits @c SelfType from @c BaseType.
 *
 * The hierarchy will look like this:
 * ASTNode <|-- BaseType <|-- MakeA <|-- SelfType
 *
 * Initializes self using @c initialize.
 *
 * @tparam BaseType Base class type.
 * @tparam SelfType Child class type.
 */
template<typename BaseType, typename SelfType>
struct MakeA : BaseType {
    MakeA();
};


/**
 * @brief Adds a constructor that calls intialize.
 *
 * Can be used as an alternative to @c MakeA. See @c LocalVarDefStmt for an example.
 */
#define ASTFRI_ADD_NODE_CONSTRUCTOR(SelfType) SelfType() {\
  ::astfri::detail::initialize<SelfType>(*this);\
}


} // namespace detail


} // namespace astfri


#include <astfri/impl/ASTNode.inl>


#endif
