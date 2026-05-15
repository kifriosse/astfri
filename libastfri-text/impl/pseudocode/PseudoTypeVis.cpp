#include <libastfri-text/inc/pseudocode/PseudocodeVisitor.hpp>

using namespace astfri::text;

void PseudocodeVisitor::visit(const DynamicType& /*type*/)
{
    m_builder->write_dynamic_type_word();
}

void PseudocodeVisitor::visit(const IntType& /*type*/)
{
    m_builder->write_int_type_word();
}

void PseudocodeVisitor::visit(const FloatType& /*type*/)
{
    m_builder->write_float_type_word();
}

void PseudocodeVisitor::visit(const CharType& /*type*/)
{
    m_builder->write_char_type_word();
}

void PseudocodeVisitor::visit(const BoolType& /*type*/)
{
    m_builder->write_bool_type_word();
}

void PseudocodeVisitor::visit(const VoidType& /*type*/)
{
    m_builder->write_void_type_word();
}

void PseudocodeVisitor::visit(const IndirectionType& type)
{
    accept_node(type.indirect);
    m_builder->write_pointer_operator();
}

void PseudocodeVisitor::visit(const ClassType& type)
{
    m_builder->write_class_name(type.name);
}

void PseudocodeVisitor::visit(const InterfaceType& type)
{
    m_builder->write_interface_name(type.name);
}

void PseudocodeVisitor::visit(const LambdaType& type)
{
    m_builder->write_system_type(type.name);
}

void PseudocodeVisitor::visit(const IncompleteType& type)
{
    m_builder->write_system_type(type.name);
}

void PseudocodeVisitor::visit(const DeducedType& type)
{
    accept_node(type.realType);
}

void PseudocodeVisitor::visit(const UnknownType& /*type*/)
{
    m_builder->write_unknown_type();
}