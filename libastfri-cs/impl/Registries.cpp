#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/SymbolTableBuilder.hpp>
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
        {"class_declaration", SrcCodeVisitor::handle_class_def_stmt},
        {"struct_declaration", SrcCodeVisitor::handle_class_def_stmt},
        {"destructor_declaration", SrcCodeVisitor::handle_destr_def_stmt},
        {"constructor_declaration", SrcCodeVisitor::handle_constr_def_stmt},
        {"parameter", SrcCodeVisitor::handle_param_def_stmt},
        {"field_declaration", SrcCodeVisitor::handle_memb_var_def_stmt},
        {"constructor_initializer", SrcCodeVisitor::handle_construct_init},
        {"method_declaration", SrcCodeVisitor::handle_method_def_stmt},
        {"local_function_statement", SrcCodeVisitor::handle_func_stmt},
        {"block", SrcCodeVisitor::handle_block_stmt},
        {"arrow_expression_clause", SrcCodeVisitor::handle_arrow_stmt},
        {"local_declaration_statement",
         SrcCodeVisitor::handle_local_var_def_stmt},
        {"do_statement", SrcCodeVisitor::handle_do_while_loop},
        {"while_statement", SrcCodeVisitor::handle_while_loop},
        {"for_statement", SrcCodeVisitor::handle_for_loop},
        {"break_statement", SrcCodeVisitor::handle_break},
        {"continue_statement", SrcCodeVisitor::handle_continue},
        {"return_statement", SrcCodeVisitor::handle_return},
        {"throw_statement", SrcCodeVisitor::handle_throw},
        {"foreach_statement", SrcCodeVisitor::handle_for_each_loop},
        {"if_statement", SrcCodeVisitor::handle_if_stmt},
        {"try_statement", SrcCodeVisitor::handle_try_stmt},
        {"catch_clause", SrcCodeVisitor::handle_catch_clause},
        {"finally_clause", SrcCodeVisitor::handle_finally},
        {"catch_declaration", SrcCodeVisitor::handle_catch_decl},
        {"switch_statement", SrcCodeVisitor::handle_switch_stmt},
        {"switch_section", SrcCodeVisitor::handle_case_stmt},
        {"expression_statement", SrcCodeVisitor::handle_expr_stmt},
        {"ERROR",
         [](SrcCodeVisitor*, const TSNode* node) -> Stmt*
         {
             const auto [row, column] = ts_node_start_point(*node);
             throw std::runtime_error(
                 "Invalid C# syntax in source code at: row"
                 + std::to_string(row) + "and column " + std::to_string(column)
             );
         }}
}),
    exprs(
        {{"integer_literal", SrcCodeVisitor::handle_int_lit},
         {"real_literal", SrcCodeVisitor::handle_float_lit},
         {"boolean_literal", SrcCodeVisitor::handle_bool_lit},
         {"character_literal", SrcCodeVisitor::handle_char_lit},
         {"string_literal", SrcCodeVisitor::handle_str_lit},
         {"null_literal", SrcCodeVisitor::handle_null_lit},
         {"verbatim_string_literal",
          SrcCodeVisitor::handle_verbatim_str_lit},
         {"raw_string_literal", SrcCodeVisitor::handle_raw_str_lit},
         {"this_expression", SrcCodeVisitor::handle_this_expr},
         {"this", SrcCodeVisitor::handle_this_expr},
         {"conditional_expression", SrcCodeVisitor::handle_ternary_expr},
         {"prefix_unary_expression",
          SrcCodeVisitor::handle_prefix_unary_op_expr},
         // {"ref_expression",
         // CSharpTSTreeVisitor::handle_prefix_unary_op_expr},
         {"postfix_unary_expression",
          SrcCodeVisitor::handle_postfix_unary_op_expr},
         {"binary_expression", SrcCodeVisitor::handle_binary_op_expr},
         {"assignment_expression", SrcCodeVisitor::handle_binary_op_expr},
         {"parenthesized_expression",
          SrcCodeVisitor::handle_parenthesized_expr},
         {"identifier", SrcCodeVisitor::handle_identifier},
         {"member_access_expression",
          SrcCodeVisitor::handle_memb_access_expr},
         {"invocation_expression", SrcCodeVisitor::handle_invoc_expr},
         {"constant_pattern", SrcCodeVisitor::handle_const_pattern},
         {"ERROR",
          [](SrcCodeVisitor*, const TSNode* node) -> Expr*
          {
              const auto [row, column] = ts_node_start_point(*node);
              throw std::runtime_error(
                  "Invalid C# syntax in source code at: row"
                  + std::to_string(row) + "and column " + std::to_string(column)
              );
          }}}
    ),
    symbol_reg_handlers({
        {"class_declaration", SymbolTableBuilder::register_class},
        {"struct_declaration", SymbolTableBuilder::register_class},
        {"interface_declaration", SymbolTableBuilder::register_interface},
        {"enum_declaration", SymbolTableBuilder::register_enum},
        {"delegate_declaration", SymbolTableBuilder::register_delegate},
        {"record_declaration", SymbolTableBuilder::register_record},
        {"field_declaration", SymbolTableBuilder::register_memb_var},
        {"property_declaration", SymbolTableBuilder::register_property},
        {"method_declaration", SymbolTableBuilder::register_method},
    })
{
}

Operations::Operations() :
    prefix_unary_op({
        {"+",   UnaryOpType::Plus        },
        {"-",   UnaryOpType::Minus       },
        {"++",  UnaryOpType::PreIncrement},
        {"--",  UnaryOpType::PreDecrement},
        {"!",   UnaryOpType::LogicalNot  },
        {"~",   UnaryOpType::BitFlip     },
        {"*",   UnaryOpType::Dereference },
        {"&",   UnaryOpType::AddressOf   },
        {"ref", UnaryOpType::AddressOf   }
}),
    bin_operations(
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
    type_factory(TypeFactory::get_instance()),
    types({
        {"bool", type_factory.mk_bool()},
        {"byte", type_factory.mk_int()}, // TODO: implement `byte` type
        {"char", type_factory.mk_char()},
        {"short", type_factory.mk_int()}, // TODO: implement `short` type
        {"ushort", type_factory.mk_int()}, // TODO: implement `ushort` type
        {"int", type_factory.mk_int()},
        {"uint", type_factory.mk_int()}, // TODO: implement `uint` type
        {"long", type_factory.mk_unknown()}, // TODO: implement `long` type
        {"ulong", type_factory.mk_unknown()}, // TODO: implement `ulong` type

        {"float", type_factory.mk_float()},
        {"double", type_factory.mk_unknown()}, // TODO: implement `double` type
        {"decimal",
         type_factory.mk_unknown()}, // TODO: implement `decimal` type

        {"void", type_factory.mk_void()},
        {"dynamic", type_factory.mk_dynamic()},

        {"Boolean", type_factory.mk_bool()},
        {"Byte", type_factory.mk_int()}, // TODO: implement `Byte` type
        {"SByte", type_factory.mk_int()}, // TODO: implement `SByte` type
        {"Char", type_factory.mk_char()},
        {"Int16", type_factory.mk_int()}, // TODO: implement `Int16` type
        {"Int32", type_factory.mk_int()},
        {"Int64", type_factory.mk_unknown()}, // TODO: implement `Int64` type
        {"UInt16", type_factory.mk_int()}, // TODO: implement `UInt16` type
        {"UInt32", type_factory.mk_int()}, // TODO: implement `UInt32` type
        {"UInt64", type_factory.mk_unknown()}, // TODO: implement `UInt64` type

        {"Single", type_factory.mk_float()},
        {"Double", type_factory.mk_unknown()}, // TODO: implement `double` type
        {"Decimal",
         type_factory.mk_unknown()}, // TODO: implement `decimal` type

        {"object", type_factory.mk_class("object", {{"System"}})},
        {"Object", type_factory.mk_class("object", {{"System"}})},
        {"string", type_factory.mk_class("string", {{"System"}})},
        {"String", type_factory.mk_class("string", {{"System"}})},

        {"var", type_factory.mk_unknown()}, // todo handle var type
        {"_", type_factory.mk_unknown()}, // todo handle discard type
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

RegHandler RegManager::get_reg_handler(const TSNode& node)
{
    return get_reg_handler(ts_node_type(node));
}

StmtHandler RegManager::get_stmt_handler(const std::string_view node_type)
{
    auto& stmts   = handlers_.stmts;
    const auto it = stmts.find(node_type);
    return it != stmts.end() ? it->second : default_stmt_handler;
}

ExprHandler RegManager::get_expr_handler(const std::string_view node_type)
{
    auto& exprs   = handlers_.exprs;
    const auto it = exprs.find(node_type);
    return it != exprs.end() ? it->second : default_expr_handler;
}

RegHandler RegManager::get_reg_handler(const std::string_view node_type)
{
    auto& reg_handlers = handlers_.symbol_reg_handlers;
    const auto it      = reg_handlers.find(node_type);
    return it != reg_handlers.end() ? it->second
                                    : [](auto, const auto&, auto&) { };
}

std::optional<UnaryOpType> RegManager::get_prefix_unary_op(
    const std::string_view op
)
{
    auto& prefix_unary_ops = operations_.prefix_unary_op;
    const auto it          = prefix_unary_ops.find(op);
    if (it != prefix_unary_ops.end())
    {
        return it->second;
    }
    return {};
}

std::optional<BinOpType> RegManager::get_bin_op(
    const std::string_view operation
)
{
    auto& bin_ops = operations_.bin_operations;
    const auto it = bin_ops.find(operation);
    if (it != bin_ops.end())
    {
        return it->second;
    }
    return {};
}

std::optional<Type*> RegManager::get_type(const std::string_view type_name)
{
    auto& types   = types_.types;
    const auto it = types.find(type_name);
    if (it != types.end())
    {
        return it->second;
    }
    return {};
}

std::optional<CSModifier> RegManager::get_modifier(
    const std::string_view modifier
)
{
    auto& modifs  = modifiers_.modifiers;
    const auto it = modifs.find(modifier);
    if (it != modifs.end())
        return it->second;
    return {};
}

bool RegManager::is_expr(const TSNode& node)
{
    return handlers_.exprs.contains(ts_node_type(node));
}

bool RegManager::is_stmt(const TSNode& node)
{
    return handlers_.stmts.contains(ts_node_type(node));
}

Expr* RegManager::default_expr_handler(SrcCodeVisitor*, const TSNode*)
{
    return ExprFactory::get_instance().mk_unknown();
}

Stmt* RegManager::default_stmt_handler(SrcCodeVisitor*, const TSNode*)
{
    return StmtFactory::get_instance().mk_uknown();
}
} // namespace astfri::csharp
