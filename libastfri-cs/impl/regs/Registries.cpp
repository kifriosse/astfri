#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/SymbolTableBuilder.hpp>
#include <libastfri-cs/impl/TypeTranslator.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <string>

namespace astfri::csharp
{

namespace regs
{

Handlers::Handlers() :
    stmts({
        {"class_declaration", SrcCodeVisitor::visit_class_def_stmt},
        {"struct_declaration", SrcCodeVisitor::visit_class_def_stmt},
        {"destructor_declaration", SrcCodeVisitor::visit_destr_def_stmt},
        {"constructor_declaration", SrcCodeVisitor::visit_constr_def_stmt},
        {"parameter", SrcCodeVisitor::visit_param_def_stmt},
        {"field_declaration", SrcCodeVisitor::visit_memb_var_def_stmt},
        {"constructor_initializer", SrcCodeVisitor::visit_construct_init},
        {"method_declaration", SrcCodeVisitor::visit_method_def_stmt},
        {"local_function_statement", SrcCodeVisitor::visit_func_stmt},
        {"block", SrcCodeVisitor::visit_block_stmt},
        {"arrow_expression_clause", SrcCodeVisitor::visit_arrow_stmt},
        {"local_declaration_statement",
         SrcCodeVisitor::visit_local_var_def_stmt},
        {"do_statement", SrcCodeVisitor::visit_do_while_loop},
        {"while_statement", SrcCodeVisitor::visit_while_loop},
        {"for_statement", SrcCodeVisitor::visit_for_loop},
        {"break_statement", SrcCodeVisitor::visit_break},
        {"continue_statement", SrcCodeVisitor::visit_continue},
        {"return_statement", SrcCodeVisitor::visit_return},
        {"throw_statement", SrcCodeVisitor::visit_throw},
        {"foreach_statement", SrcCodeVisitor::visit_for_each_loop},
        {"if_statement", SrcCodeVisitor::visit_if_stmt},
        {"try_statement", SrcCodeVisitor::visit_try_stmt},
        {"catch_clause", SrcCodeVisitor::visit_catch_clause},
        {"finally_clause", SrcCodeVisitor::visit_finally},
        {"catch_declaration", SrcCodeVisitor::visit_catch_decl},
        {"switch_statement", SrcCodeVisitor::visit_switch_stmt},
        {"switch_section", SrcCodeVisitor::visit_case_stmt},
        {"expression_statement", SrcCodeVisitor::visit_expr_stmt},
        {"ERROR", visit_error<SrcCodeVisitor, Stmt*>}
}),
    exprs(
        {{"integer_literal", SrcCodeVisitor::visit_int_lit},
         {"real_literal", SrcCodeVisitor::visit_float_lit},
         {"boolean_literal", SrcCodeVisitor::visit_bool_lit},
         {"character_literal", SrcCodeVisitor::visit_char_lit},
         {"string_literal", SrcCodeVisitor::visit_str_lit},
         {"null_literal", SrcCodeVisitor::visit_null_lit},
         {"verbatim_string_literal", SrcCodeVisitor::visit_verbatim_str_lit},
         {"raw_string_literal", SrcCodeVisitor::visit_raw_str_lit},
         {"this_expression", SrcCodeVisitor::visit_this_expr},
         {"this", SrcCodeVisitor::visit_this_expr},
         {"conditional_expression", SrcCodeVisitor::visit_ternary_expr},
         {"prefix_unary_expression",
          SrcCodeVisitor::visit_prefix_unary_op_expr},
         {"ref_expression", SrcCodeVisitor::visit_ref_expr},
         {"postfix_unary_expression",
          SrcCodeVisitor::visit_postfix_unary_op_expr},
         {"binary_expression", SrcCodeVisitor::visit_binary_op_expr},
         {"assignment_expression", SrcCodeVisitor::visit_binary_op_expr},
         {"parenthesized_expression", SrcCodeVisitor::visit_parenthesized_expr},
         {"identifier", SrcCodeVisitor::visit_identifier},
         {"member_access_expression", SrcCodeVisitor::visit_memb_access_expr},
         {"invocation_expression", SrcCodeVisitor::visit_invoc_expr},
         {"constant_pattern", SrcCodeVisitor::visit_const_pattern},
         {"ERROR", visit_error<SrcCodeVisitor, Expr*>}}
    ),
    types(
        {{"predefined_type", TypeTranslator::visit_predefined},
         {"identifier", TypeTranslator::visit_identitifier},
         {"qualified_name",
          TypeTranslator::visit_qualified_name}, // type with namespace
         {"implicit_type", TypeTranslator::visit_qualified_name}, // var
         {"nullable_type", TypeTranslator::visit_wrapper},
         {"pointer_type", TypeTranslator::visit_inderect},
         {"ref_type", TypeTranslator::visit_inderect},
         {"array_type", TypeTranslator::visit_array},
         {"generic_name", TypeTranslator::visit_generic_name},
         {"tuple_type", TypeTranslator::visit_tuple},
         {"function_pointer_type",
          TypeTranslator::visit_func_pointer}, // function pointer - not
                                               // delegate
         {"scoped_type", TypeTranslator::visit_wrapper},
         {"ERROR", visit_error<TypeTranslator, Type*>}}
    ),
    symbolRegs(
        {{"class_declaration", SymbolTableBuilder::visit_class},
         {"struct_declaration", SymbolTableBuilder::visit_class},
         {"interface_declaration", SymbolTableBuilder::visit_interface},
         {"enum_declaration", SymbolTableBuilder::visit_enum},
         {"delegate_declaration", SymbolTableBuilder::visit_delegate},
         {"record_declaration", SymbolTableBuilder::visit_record},
         {"field_declaration", SymbolTableBuilder::visit_memb_var},
         {"property_declaration", SymbolTableBuilder::visit_property},
         {"method_declaration", SymbolTableBuilder::visit_method},
         {"ERROR", visit_error<SymbolTableBuilder, void>}}
    )
{
}

Operations::Operations() :
    prefixUnaryOps({
        {"+",  UnaryOpType::Plus        },
        {"-",  UnaryOpType::Minus       },
        {"++", UnaryOpType::PreIncrement},
        {"--", UnaryOpType::PreDecrement},
        {"!",  UnaryOpType::LogicalNot  },
        {"~",  UnaryOpType::BitFlip     },
        {"*",  UnaryOpType::Dereference },
        {"&",  UnaryOpType::AddressOf   },
}),
    binaryOps(
        {{"=", BinOpType::Assign},
         {"+", BinOpType::Add},
         {"-", BinOpType::Subtract},
         {"*", BinOpType::Multiply},
         {"/", BinOpType::Divide},
         {"%", BinOpType::Modulo},
         {"==", BinOpType::Equal},
         {"!=", BinOpType::NotEqual},
         {"<", BinOpType::Less},
         {"<=", BinOpType::LessEqual},
         {">", BinOpType::Greater},
         {">=", BinOpType::GreaterEqual},
         {"&&", BinOpType::LogicalAnd},
         {"||", BinOpType::LogicalOr},
         {"&", BinOpType::BitAnd},
         {"|", BinOpType::BitOr},
         {"^", BinOpType::BitXor},
         {"<<", BinOpType::BitShiftLeft},
         {">>", BinOpType::BitShiftRight},
         {"+=", BinOpType::AddAssign},
         {"-=", BinOpType::SubtractAssign},
         {"*=", BinOpType::MultiplyAssign},
         {"/=", BinOpType::DivideAssign},
         {"%=", BinOpType::ModuloAssign},
         {">>=", BinOpType::BitShiftRightAssign},
         {"<<=", BinOpType::BitShiftLeftAssign},
         {"&=", BinOpType::BitAndAssign},
         {"|=", BinOpType::BitOrAssign},
         {"^=", BinOpType::BitXorAssign}}
    )
{
}

Modifiers::Modifiers() :
    modifiers({
        {"private",   CSModifier::Private  },
        {"public",    CSModifier::Public   },
        {"protected", CSModifier::Protected},
        {"internal",  CSModifier::Internal },
        {"file",      CSModifier::File     },
        {"static",    CSModifier::Static   },
        {"readonly",  CSModifier::Readonly },
        {"const",     CSModifier::Const    },
        {"volatile",  CSModifier::Volatile },
        {"abstract",  CSModifier::Abstract },
        {"virtual",   CSModifier::Virtual  },
        {"override",  CSModifier::Override },
        {"sealed",    CSModifier::Sealed   },
        {"partial",   CSModifier::Partial  },
        {"async",     CSModifier::Async    },
        {"out",       CSModifier::Out      },
        {"in",        CSModifier::In       },
        {"ref",       CSModifier::Ref      },
})
{
}

Types::Types() :
    typeFact(TypeFactory::get_instance()),
    types({
        {"bool", typeFact.mk_bool()},
        {"byte", typeFact.mk_int()}, // TODO: implement `byte` type
        {"char", typeFact.mk_char()},
        {"short", typeFact.mk_int()}, // TODO: implement `short` type
        {"ushort", typeFact.mk_int()}, // TODO: implement `ushort` type
        {"int", typeFact.mk_int()},
        {"uint", typeFact.mk_int()}, // TODO: implement `uint` type
        {"long", typeFact.mk_unknown()}, // TODO: implement `long` type
        {"ulong", typeFact.mk_unknown()}, // TODO: implement `ulong` type

        {"float", typeFact.mk_float()},
        {"double", typeFact.mk_unknown()}, // TODO: implement `double` type
        {"decimal", typeFact.mk_unknown()}, // TODO: implement `decimal` type

        {"void", typeFact.mk_void()},
        {"dynamic", typeFact.mk_dynamic()},

        {"Boolean", typeFact.mk_bool()},
        {"Byte", typeFact.mk_int()}, // TODO: implement `Byte` type
        {"SByte", typeFact.mk_int()}, // TODO: implement `SByte` type
        {"Char", typeFact.mk_char()},
        {"Int16", typeFact.mk_int()}, // TODO: implement `Int16` type
        {"Int32", typeFact.mk_int()},
        {"Int64", typeFact.mk_unknown()}, // TODO: implement `Int64` type
        {"UInt16", typeFact.mk_int()}, // TODO: implement `UInt16` type
        {"UInt32", typeFact.mk_int()}, // TODO: implement `UInt32` type
        {"UInt64", typeFact.mk_unknown()}, // TODO: implement `UInt64` type

        {"Single", typeFact.mk_float()},
        {"Double", typeFact.mk_unknown()}, // TODO: implement `double` type
        {"Decimal", typeFact.mk_unknown()}, // TODO: implement `decimal` type

        {"object", typeFact.mk_class("object", {{"System"}})},
        {"Object", typeFact.mk_class("object", {{"System"}})},
        {"string", typeFact.mk_class("string", {{"System"}})},
        {"String", typeFact.mk_class("string", {{"System"}})},

        {"var", typeFact.mk_unknown()}, // todo handle var type
        {"_", typeFact.mk_unknown()}, // todo handle discard type
})
{
}

} // namespace regs

regs::Handlers RegManager::handlers_;
regs::Operations RegManager::operations_;
regs::Types RegManager::types_;
regs::Modifiers RegManager::modifiers_;

StmtHandler RegManager::get_stmt_handler(const TSNode& node)
{
    return get_stmt_handler(ts_node_type(node));
}

ExprHandler RegManager::get_expr_handler(const TSNode& node)
{
    return get_expr_handler(ts_node_type(node));
}

TypeHandler RegManager::get_type_handler(const TSNode& node)
{
    return get_type_handler(ts_node_type(node));
}

RegHandler RegManager::get_reg_handler(const TSNode& node)
{
    return get_reg_handler(ts_node_type(node));
}

StmtHandler RegManager::get_stmt_handler(const std::string_view nodeType)
{
    // todo redo this when the typo is fixed
    StmtHandler def = default_stmt_visit;
    return get_or_default(handlers_.stmts, nodeType, std::move(def));
}

ExprHandler RegManager::get_expr_handler(const std::string_view nodeType)
{
    ExprHandler def = default_visit<ExprFactory, SrcCodeVisitor, Expr*>;
    return get_or_default(handlers_.exprs, nodeType, std::move(def));
}

TypeHandler RegManager::get_type_handler(const std::string_view nodeType)
{
    TypeHandler def = default_visit<TypeFactory, TypeTranslator, Type*>;
    return get_or_default(handlers_.types, nodeType, std::move(def));
}

RegHandler RegManager::get_reg_handler(const std::string_view nodeType)
{
    RegHandler def = [](auto*, const auto&) { };
    return get_or_default(handlers_.symbolRegs, nodeType, std::move(def));
}

std::optional<UnaryOpType> RegManager::get_prefix_unary_op(
    const std::string_view op
)
{
    return get_opt(operations_.prefixUnaryOps, op);
}

std::optional<BinOpType> RegManager::get_bin_op(const std::string_view op)
{
    return get_opt(operations_.binaryOps, op);
}

std::optional<Type*> RegManager::get_type(const std::string_view nodeType)
{
    return get_opt(types_.types, nodeType);
}

CSModifier RegManager::get_modifier(
    const TSNode& node,
    const std::string_view src
)
{
    return get_modifier(util::extract_text(node, src));
}

CSModifier RegManager::get_modifier(const std::string_view modifs)
{
    return get_opt(modifiers_.modifiers, modifs).value_or(CSModifier::None);
}

bool RegManager::is_expr(const TSNode& node)
{
    return handlers_.exprs.contains(ts_node_type(node));
}

bool RegManager::is_stmt(const TSNode& node)
{
    return handlers_.stmts.contains(ts_node_type(node));
}

Stmt* RegManager::default_stmt_visit(SrcCodeVisitor*, const TSNode&)
{
    // todo remove this when type is fixed
    return StmtFactory::get_instance().mk_uknown();
}

} // namespace astfri::csharp
