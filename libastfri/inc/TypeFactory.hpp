#ifndef LIBASTFRI_TYPE_FACTORY
#define LIBASTFRI_TYPE_FACTORY

#include <libastfri/inc/Type.hpp>

#include <map>
#include <memory>
#include <string>


namespace astfri
{


class ExprFactory;


/**
 * @brief Singleton type factory.
 */
class TypeFactory
{
public:
    friend class ExprFactory;

public:
    /**
     * @brief Returns the singleton instance.
     * @return Singleton instance.
     */
    static TypeFactory& get_instance();

public:
    /**
     * @brief Returns singleton instance of @c IntType type.
     * @return Singleton instance of @c IntType.
     */
    IntType* mk_int();

    /**
     * @brief Returns singleton instance of @c FloatType type.
     * @return Singleton instance of @c FloatType.
     */
    FloatType* mk_float();

    /**
     * @brief Returns singleton instance of @c CharType type.
     * @return Singleton instance of @c CharType.
     */
    CharType* mk_char();

    /**
     * @brief Returns singleton instance of @c BoolType type.
     * @return Singleton instance of @c BoolType.
     */
    BoolType* mk_bool();

    /**
     * @brief Returns singleton instance of @c VoidType type.
     * @return Singleton instance of @c VoidType.
     */
    VoidType* mk_void();

    /**
     * @brief Returns singleton instance of @c UnknownType type.
     * @return Singleton instance of @c UnknownType.
     */
    UnknownType* mk_unknown();

    /**
     * @brief Returns singleton instance of @c DynamicType type.
     * @return Singleton instance of @c DynamicType.
     */
    DynamicType* mk_dynamic();

    /**
     * @brief Returns singleton instance of @c IndirectionType for @p type type.
     * @param type Type the indirection points to.
     * @return Singleton instance of @c IndirectionType for @p type.
     */
    IndirectionType* mk_indirect(Type* type);

    /**
     * @brief Returns unique @c ClassType for class @p name in scope @p scope.
     * Sets the @c ClassType::def to @c Stmt::NULL.
     * @param name Unqualified name of the class.
     * @param scope Scope of the class.
     * @return unique @c ClassType for class @p name in scope @p scope.
     */
    ClassType *mk_class(const std::string &name, const Scope &scope);

    /**
     * @brief Returns unique @c ClassType for class @p name in scope @p scope.
     * Sets the @c ClassType::def to @p def.
     * @param name Unqualified name of the class.
     * @param scope Scope of the class.
     * @param def Definition of the class.
     * @return unique @c ClassType for class @p name in scope @p scope.
     */
    ClassType *mk_class(const std::string &name, const Scope &scope, ClassDefStmt *def);

    /**
     * @brief Returns unique @c InterfaceType for interface @p name in scope @p scope.
     * Sets the @c InterfaceType::def to @c Stmt::NULL.
     * @param name Unqualified name of the interface.
     * @param scope Scope of the interface.
     * @return unique @c InterfaceType for interface @p name in scope @p scope.
     */
    InterfaceType *mk_interface(const std::string &name, const Scope &scope);

    /**
     * @brief Returns unique @c InterfaceType for interface @p name in scope @p scope.
     * Sets the @c InterfaceType::def to @p def.
     * @param name Unqualified name of the interface.
     * @param scope Scope of the interface.
     * @param def Definition of the interface.
     * @return unique @c InterfaceType for interface @p name in scope @p scope.
     */
    InterfaceType *mk_interface(const std::string &name, const Scope &scope, InterfaceDefStmt *def);

    /**
     * @brief Returns unique instance of @c DeducedType for @p realType.
     * The same instace is shared if @p realType is null.
     * @param realType Real type deduced by a compiler.
     * @return Unique instance of @c DeducedType for @p realType.
     */
    DeducedType *mk_deduced(Type* realType);

    /**
     * @brief Returns unique instance of @c IncompleteType per @p name.
     * @param name Name of the type.
     * @returns Unique instance of @c IncompleteType for @p name.
     */
    IncompleteType *mk_incomplete(const std::string &name);

public:
    /**
     * @brief Deleted copy constructor.
     */
    TypeFactory(TypeFactory const& other) = delete;

    /**
     * @brief Explicitly deleted move constructor.
     */
    TypeFactory(TypeFactory&& other) = delete;

    /**
     * @brief Deleted copy-assignment.
     */
    void operator=(TypeFactory const& other) = delete;

    /**
     * @brief Explicitly deleted copy-assignment.
     */
    void operator=(TypeFactory&& other) = delete;

private:
    /**
     * @brief Private deafult constructor.
     */
    TypeFactory() = default;

    /**
     * @brief Creates instance of LambdaType.
     * LambdaType can only be created internally when a LambdaExpr is created.
     * @param name Internal name for the lambda function.
     * @param def Definition of the lambda function.
     * @return Newly constructure instace of @c LambdaType.
     */
    LambdaType *mk_lambda(std::string name, LambdaExpr *def);

private:
    IntType m_intType;
    FloatType m_floatType;
    CharType m_charType;
    BoolType m_boolType;
    VoidType m_voidType;
    UnknownType m_unknownType;
    DynamicType m_dynamicType;
    std::map<Type*, IndirectionType> m_indirectTypeMap;
    std::map<Type*, DeducedType> m_deducedTypeMap;
    std::map<std::string, ClassType> m_classTypeMap;
    std::map<std::string, InterfaceType> m_interfaceTypeMap;
    std::map<std::string, IncompleteType> m_incompleteTypeMap;
    std::vector<std::unique_ptr<Type>> m_otherTypes;
};


} // namespace astfri

#endif
