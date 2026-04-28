#ifndef ASTFRI_IMPL_AST_NODE_HPP
#define ASTFRI_IMPL_AST_NODE_HPP

#include <astfri/impl/Kind.hpp>
#include <astfri/impl/Visitor.hpp>


namespace astfri {


/**
 * @brief Base class for all AST nodes.
 *
 * Holds kind of the node and implements visitor dispatch.
 *
 * @tparam KindType enum of kinds (ExprKind, StmtKind, TypeKind)
 */
template<typename KindType>
struct ASTNode {
    /** @brief Enum value holding kind (type) of this node. */
    KindType kind;

    /**
     * @brief Accepts visitor by calling its corresponding @c visit method.
     * @param visitor Visitor to be accepted.
     */
    void accept(Visitor &visitor);

protected:
    /** @brief Pointer to a function implementing accept. */
    void(*m_accept_ptr)(void*, Visitor&);
};


/**
 * @brief Helper CRTP base. Indirectly inherits @c SelfType from @c BaseType.
 *
 * The hierarchy will look like this:
 * ASTNode <|-- BaseType <|-- MakeA <|-- SelfType
 *
 * Initializes @c kind member of the @c ASTNode base.
 * Initializes @c m_accept_ptr member of the @c ASTNode base.
 *
 * @tparam BaseType Base class type.
 * @tparam SelfType Child class type.
 */
template<typename BaseType, typename SelfType>
struct MakeA : BaseType {
    MakeA() {
        // The specialization must exist, otherwise we get a compile error.
        this->kind = KindOf<SelfType>::value;

        // Implicit conversion of lambda to function pointer.
        this->m_accept_ptr = +[](void *self, Visitor &visitor){
            visitor.visit(*static_cast<SelfType*>(self));
        };
    }
};


} // namespace astfri


#include <astfri/impl/ASTNode.inl>


#endif
