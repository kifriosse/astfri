#ifndef LIBASTFRI_IMPL_TYPE_ID_HPP
#define LIBASTFRI_IMPL_TYPE_ID_HPP

#include <libastfri/impl/TypeFwd.hpp>
#include <libastfri/impl/ExprFwd.hpp>
#include <libastfri/impl/StmtFwd.hpp>

namespace astfri::details
{

struct type_id_t
{
    int value_;
};

struct ITypeQueryable
{
    virtual type_id_t get_type_id() = 0;
};
template<class T>
type_id_t get_type_id();

template<>
type_id_t get_type_id<class T>();

template<> inline type_id_t get_type_id<DynamicType>()     { return {1}; }
template<> inline type_id_t get_type_id<IntType>()         { return {2}; }
template<> inline type_id_t get_type_id<FloatType>()       { return {3}; }
template<> inline type_id_t get_type_id<CharType>()        { return {4}; }
template<> inline type_id_t get_type_id<BoolType>()        { return {5}; }
template<> inline type_id_t get_type_id<VoidType>()        { return {6}; }
template<> inline type_id_t get_type_id<UserType>()        { return {7}; }
template<> inline type_id_t get_type_id<IndirectionType>() { return {8}; }
template<> inline type_id_t get_type_id<UnknownType>()     { return {9}; }

}  // namespace astfri::details

#endif
