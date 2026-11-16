#ifndef ASTFRI_IMPL_AST_NODE_HPP
#define ASTFRI_IMPL_AST_NODE_HPP


namespace astfri {


/**
 * @brief Base class for all AST nodes.
 *
 * Holds type tag of the node and implements visitor dispatch.
 */
struct ASTNode;


/**
 * @brief Visitor intrface for ASTNodes.
 */
class Visitor;


/**
 * @brief Type tags for each ASTNode type.
 */
enum class ASTNodeType;


/**
 * @brief Base class for all AST nodes.
 *
 * Holds type tag of the node and implements visitor dispatch.
 */
struct ASTNode {
    /**
     * @brief Accepts visitor by calling its corresponding @c visit method.
     * @param visitor Visitor to be accepted.
     */
    void accept(Visitor &visitor) {
        m_accept_ptr(this, visitor);
    }

    /**
     * @brief Returns type tag of the real node type.
     * @return Type tag of this node real (childmost) type.
     */
    ASTNodeType type() const {
        return m_type;
    }

protected:
    /**
     * Enum value holding type tag of this node.
     */
    ASTNodeType m_type;

    /**
     * Pointer to a function implementing accept for the this nodes real type.
     */
    void(*m_accept_ptr)(void*, Visitor&);
};


/**
 * @brief Adds a constructor that calls intialize.
 */
/**
 * @brief Adds a constructor definition that initializes ASTNode.
 *
 * Initializes @c m_type member of the @c ASTNode base.
 * Initializes @c m_accept_ptr member of the @c ASTNode base.
 *
 * @tparam T Real (childmost) type of @p node.
 * @param node Node to initialize.
 */
#define ASTFRI_ADD_NODE_CONSTRUCTOR(T) inline T::T() {\
    this->m_type = ::astfri::detail::TypeOf<T>::value;\
    this->m_accept_ptr = +[](void *self, Visitor &visitor){\
        visitor.visit(*static_cast<T*>(self));\
    };\
}\


} // namespace astfri


#endif
