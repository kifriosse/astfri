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
 * @brief TODO
 */
class TypeFactory
{
public:
    friend class ExprFactory;

public:
    static TypeFactory& get_instance();

public:
    IntType* mk_int();
    FloatType* mk_float();
    CharType* mk_char();
    BoolType* mk_bool();
    VoidType* mk_void();
    UnknownType* mk_unknown();
    DynamicType* mk_dynamic();
    IndirectionType* mk_indirect(Type* type);
    ClassType *mk_class(const std::string &name, const Scope &scope);
    ClassType *mk_class(const std::string &name, const Scope &scope, ClassDefStmt *def);
    InterfaceType *mk_interface(const std::string &name, const Scope &scope);
    InterfaceType *mk_interface(const std::string &name, const Scope &scope, InterfaceDefStmt *def);

public:
    TypeFactory(TypeFactory const&)    = delete;
    void operator=(TypeFactory const&) = delete;

private:
    TypeFactory() = default;

    /**
     * @brief LambdaType can only be created internally
     * when a LambdaExpr is created.
     */
    LambdaType *mk_lambda(std::string name, LambdaExpr *def);

private:
    IntType int_;
    FloatType float_;
    CharType char_;
    BoolType bool_;
    VoidType void_;
    UnknownType unknown_;
    DynamicType dynamic_;
    std::map<Type*, IndirectionType> indirect_;
    std::map<std::string, ClassType> m_classes;
    std::map<std::string, InterfaceType> m_interfaces;
    std::vector<std::unique_ptr<Type>> m_types;
};

} // namespace astfri

#endif
