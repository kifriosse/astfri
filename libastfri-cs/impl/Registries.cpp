#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/utils.hpp>

namespace astfri::csharp
{
namespace regs
{

const std::string Queries::top_level_stmt_query =
    R"(
    (namespace_declaration
        body: (declaration_list
        [
            (class_declaration)     @class
            (interface_declaration) @interface
            (struct_declaration)    @struct
            (enum_declaration)      @enum
            (delegate_declaration)  @delegate
            (record_declaration)    @record
        ])
    )
    (compilation_unit
    [
        (class_declaration)     @class
        (interface_declaration) @interface
        (struct_declaration)    @struct
        (enum_declaration)      @enum
        (delegate_declaration)  @delegate
        (record_declaration)    @record
    ])
    )";

const std::string Queries::decl_query =
    R"(
    (variable_declaration
        (variable_declarator) @var_decl)
    )";

const std::string Queries::var_modif_query =
    R"(
    [
        (field_declaration (modifier) @modifier)
        (local_declaration_statement (modifier) @modifier)
    ]
    )";

const std::string Queries::method_modif_query =
    R"(
        (method_declaration
            (modifier) @modifier
        )
    )";

Handlers::Handlers() :
    stmts({
        {"class_declaration", SourceCodeVisitor::handle_class_def_stmt},
        {"struct_declaration", SourceCodeVisitor::handle_class_def_stmt},
        {"destructor_declaration", SourceCodeVisitor::handle_destr_def_stmt},
        {"constructor_declaration", SourceCodeVisitor::handle_constr_def_stmt},
        {"parameter", SourceCodeVisitor::handle_param_def_stmt},
        {"field_declaration", SourceCodeVisitor::handle_memb_var_def_stmt},
        {"constructor_initializer", SourceCodeVisitor::handle_construct_init},
        {"method_declaration", SourceCodeVisitor::handle_method_def_stmt},
        {"local_function_statement", SourceCodeVisitor::handle_local_func_stmt},
        {"block", SourceCodeVisitor::handle_block_stmt},
        {"arrow_expression_clause", SourceCodeVisitor::handle_arrow_stmt},
        {"local_declaration_statement",
         SourceCodeVisitor::handle_local_var_def_stmt},
        {"do_statement", SourceCodeVisitor::handle_do_while_loop},
        {"while_statement", SourceCodeVisitor::handle_while_loop},
        {"for_statement", SourceCodeVisitor::handle_for_loop},
        {"break_statement", SourceCodeVisitor::handle_break},
        {"continue_statement", SourceCodeVisitor::handle_continue},
        {"return_statement", SourceCodeVisitor::handle_return},
        {"throw_statement", SourceCodeVisitor::handle_throw},
        {"foreach_statement", SourceCodeVisitor::handle_for_each_loop},
        {"if_statement", SourceCodeVisitor::handle_if_stmt},
        {"try_statement", SourceCodeVisitor::handle_try_stmt},
        {"catch_clause", SourceCodeVisitor::handle_catch_clause},
        {"finally_clause", SourceCodeVisitor::handle_finally_clause},
        {"catch_declaration", SourceCodeVisitor::handle_catch_decl},
        {"switch_statement", SourceCodeVisitor::handle_switch_stmt},
        {"switch_section", SourceCodeVisitor::handle_case_stmt},
        {"expression_statement", SourceCodeVisitor::handle_expr_stmt},
        {"ERROR",
         [](SourceCodeVisitor*, const TSNode* node) -> Stmt*
         {
             const auto [row, column] = ts_node_start_point(*node);
             throw std::runtime_error(
                 "Invalid C# syntax in source code at: row"
                 + std::to_string(row) + "and column " + std::to_string(column)
             );
         }}
}),
    exprs(
        {{"integer_literal", SourceCodeVisitor::handle_int_lit},
         {"real_literal", SourceCodeVisitor::handle_float_lit},
         {"boolean_literal", SourceCodeVisitor::handle_bool_lit},
         {"character_literal", SourceCodeVisitor::handle_char_lit},
         {"string_literal", SourceCodeVisitor::handle_str_lit},
         {"null_literal", SourceCodeVisitor::handle_null_literal},
         {"verbatim_string_literal",
          SourceCodeVisitor::handle_verbatim_str_lit},
         {"raw_string_literal", SourceCodeVisitor::handle_raw_str_lit},
         {"this_expression", SourceCodeVisitor::handle_this_expr},
         {"this", SourceCodeVisitor::handle_this_expr},
         {"conditional_expression", SourceCodeVisitor::handle_ternary_expr},
         {"prefix_unary_expression",
          SourceCodeVisitor::handle_prefix_unary_op_expr},
         // {"ref_expression",
         // CSharpTSTreeVisitor::handle_prefix_unary_op_expr},
         {"postfix_unary_expression",
          SourceCodeVisitor::handle_postfix_unary_op_expr},
         {"binary_expression", SourceCodeVisitor::handle_binary_op_expr},
         {"assignment_expression", SourceCodeVisitor::handle_binary_op_expr},
         {"parenthesized_expression",
          SourceCodeVisitor::handle_parenthesized_expr},
         {"identifier", SourceCodeVisitor::handle_identifier},
         {"member_access_expression",
          SourceCodeVisitor::handle_memb_access_expr},
         {"invocation_expression", SourceCodeVisitor::handle_invocation_expr},
         {"constant_pattern", SourceCodeVisitor::handle_const_pattern},
         {"ERROR",
          [](SourceCodeVisitor*, const TSNode* node) -> Expr*
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
        {"async",     CSModifier::Async    }
})
{
}

Types::Types() :
    type_factory(TypeFactory::get_instance()),
    types({
        {"bool", type_factory.mk_bool()},
        {"byte", type_factory.mk_unknown()}, // TODO: implement `byte` type
        {"char", type_factory.mk_char()},
        {"short", type_factory.mk_unknown()}, // TODO: implement `short` type
        {"ushort", type_factory.mk_unknown()}, // TODO: implement `ushort` type
        {"int", type_factory.mk_int()},
        {"uint", type_factory.mk_unknown()}, // TODO: implement `uint` type
        {"long", type_factory.mk_unknown()}, // TODO: implement `long` type
        {"ulong", type_factory.mk_unknown()}, // TODO: implement `ulong` type

        {"float", type_factory.mk_float()},
        {"double", type_factory.mk_unknown()}, // TODO: implement `double` type
        {"decimal",
         type_factory.mk_unknown()}, // TODO: implement `decimal` type

        {"void", type_factory.mk_void()},
        {"dynamic", type_factory.mk_dynamic()},

        {"Boolean", type_factory.mk_bool()},
        {"Byte", type_factory.mk_unknown()}, // TODO: implement `Byte` type
        {"SByte", type_factory.mk_unknown()}, // TODO: implement `SByte` type
        {"Char", type_factory.mk_char()},
        {"Int16", type_factory.mk_unknown()}, // TODO: implement `Int16` type
        {"Int32", type_factory.mk_int()},
        {"Int64", type_factory.mk_unknown()}, // TODO: implement `Int64` type
        {"UInt16", type_factory.mk_unknown()}, // TODO: implement `UInt16` type
        {"UInt32", type_factory.mk_unknown()}, // TODO: implement `UInt32` type
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

SourceCodeVisitor::StmtHandler RegManager::get_stmt_handler(const TSNode& node)
{
    return get_stmt_handler(ts_node_type(node));
}

SourceCodeVisitor::ExprHandler RegManager::get_expr_handler(const TSNode& node)
{
    return get_expr_handler(ts_node_type(node));
}

SymbolTableBuilder::RegHandler RegManager::get_reg_handler(const TSNode& node)
{
    return get_reg_handler(ts_node_type(node));
}

SourceCodeVisitor::StmtHandler RegManager::get_stmt_handler(
    const std::string& node_type
)
{
    auto& stmts   = handlers_.stmts;
    const auto it = stmts.find(node_type);
    return it != stmts.end() ? it->second : default_stmt_handler;
}

SourceCodeVisitor::ExprHandler RegManager::get_expr_handler(
    const std::string& node_type
)
{
    auto& exprs   = handlers_.exprs;
    const auto it = exprs.find(node_type);
    return it != exprs.end() ? it->second : default_expr_handler;
}

SymbolTableBuilder::RegHandler RegManager::get_reg_handler(
    const std::string& node_type
)
{
    auto& reg_handlers = handlers_.symbol_reg_handlers;
    const auto it      = reg_handlers.find(node_type);
    return it != reg_handlers.end() ? it->second
                                    : [](auto, const auto&, auto&) { };
}

std::optional<UnaryOpType> RegManager::get_prefix_unary_op(
    const std::string& operation
)
{
    auto& prefix_unary_ops = operations_.prefix_unary_op;
    const auto it          = prefix_unary_ops.find(operation);
    if (it != prefix_unary_ops.end())
    {
        return it->second;
    }
    return {};
}

std::optional<BinOpType> RegManager::get_bin_op(const std::string& operation)
{
    auto& bin_operations = operations_.bin_operations;
    const auto it        = bin_operations.find(operation);
    if (it != bin_operations.end())
    {
        return it->second;
    }
    return {};
}

std::optional<Type*> RegManager::get_type(const std::string& type_name)
{
    auto& types   = types_.types;
    const auto it = types.find(type_name);
    if (it != types.end())
    {
        return it->second;
    }
    return {};
}

std::optional<CSModifier> RegManager::get_modifier(const std::string& modifier)
{
    auto& modifiers = modifiers_.modifiers;
    const auto it   = modifiers.find(modifier);
    if (it != modifiers.end())
    {
        return it->second;
    }
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

Expr* RegManager::default_expr_handler(SourceCodeVisitor*, const TSNode*)
{
    return ExprFactory::get_instance().mk_unknown();
}

Stmt* RegManager::default_stmt_handler(SourceCodeVisitor*, const TSNode*)
{
    return StmtFactory::get_instance().mk_uknown();
}
} // namespace astfri::csharp
