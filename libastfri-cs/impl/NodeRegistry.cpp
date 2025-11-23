#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>

namespace astfri::csharp
{
TypeFactory& NodeRegistry::type_factory_ = TypeFactory::get_instance();

std::unordered_map<std::string, CSharpTSTreeVisitor::StmtHandler> NodeRegistry::stmt_handlers_ = {
    {"class_declaration",      CSharpTSTreeVisitor::handle_class_def_stmt},
    {"variable_declaration",
     CSharpTSTreeVisitor::handle_memb_var_def_stmt                       }, // todo fix this might not work
    {"destructor_declaration", CSharpTSTreeVisitor::handle_destr_def_stmt},
    // {"field_declaration",    CSharpTSTreeVisitor::handle_memb_var_def_stmt},
};

std::unordered_map<std::string, CSharpTSTreeVisitor::ExprHandler> NodeRegistry::expr_handlers_ = {
    {"integer_literal", CSharpTSTreeVisitor::handle_int_lit},
    {"real_literal", CSharpTSTreeVisitor::handle_float_lit},
    {"boolean_literal", CSharpTSTreeVisitor::handle_bool_lit},
    {"character_literal", CSharpTSTreeVisitor::handle_char_lit},
    {"string_literal", CSharpTSTreeVisitor::handle_str_lit},
    {"null_literal", CSharpTSTreeVisitor::handle_null_literal},
    {"verbatim_string_literal", CSharpTSTreeVisitor::handle_verbatim_str_lit},
    {"this_expression", CSharpTSTreeVisitor::handle_this_expr},
    {"conditional_expression", CSharpTSTreeVisitor::handle_ternary_expr},
    {"prefix_unary_expression", CSharpTSTreeVisitor::handle_prefix_unary_op_expr},
    {"ref_expression", CSharpTSTreeVisitor::handle_prefix_unary_op_expr},
    {"postfix_unary_expression", CSharpTSTreeVisitor::handle_postfix_unary_op_expr},
    {"binary_expression", CSharpTSTreeVisitor::handle_binary_op_expr},
    {"assignment_expression", CSharpTSTreeVisitor::handle_binary_op_expr},

    {"ERROR",
     [](CSharpTSTreeVisitor*, TSNode const* node) -> Expr*
     {
         auto const [row, column] = ts_node_start_point(*node);
         throw std::runtime_error(
             "Invalid C# syntax in source code at: row" + std::to_string(row) + "and column "
             + std::to_string(column)
         );
     }}
};

std::unordered_map<std::string, UnaryOpType> NodeRegistry::prefix_unary_op = {
    {"+",   UnaryOpType::Plus        },
    {"-",   UnaryOpType::Minus       },
    {"++",  UnaryOpType::PreIncrement},
    {"--",  UnaryOpType::PreDecrement},
    {"!",   UnaryOpType::LogicalNot  },
    {"~",   UnaryOpType::BitFlip     },
    {"*",   UnaryOpType::Dereference },
    {"&",   UnaryOpType::AddressOf   },
    {"ref", UnaryOpType::AddressOf   }
};

std::unordered_map<std::string, BinOpType> NodeRegistry::bin_operations = {
    {"=",   BinOpType::Assign       },
    {"+",   BinOpType::Add          },
    {"-",   BinOpType::Subtract     },
    {"*",   BinOpType::Multiply     },
    {"/",   BinOpType::Divide       },
    {"%",   BinOpType::Modulo       },
    {"==",  BinOpType::Equal        },
    {"!=",  BinOpType::NotEqual     },
    {"<",   BinOpType::Less         },
    {"<=",  BinOpType::LessEqual    },
    {">",   BinOpType::Greater      },
    {">=",  BinOpType::GreaterEqual },
    {"&&",  BinOpType::LogicalAnd   },
    {"||",  BinOpType::LogicalOr    },
    {"&",   BinOpType::BitAnd       },
    {"|",   BinOpType::BitOr        },
    {"^",   BinOpType::BitXor       },
    {"<<",  BinOpType::BitShiftLeft },
    {">>",  BinOpType::BitShiftRight},
    {"+= ", BinOpType::Add          },
    {"-=",  BinOpType::Subtract     },
    {"*=",  BinOpType::Multiply     },
    {"/=",  BinOpType::Divide       },
    {"%=",  BinOpType::Modulo       },
    {">>=", BinOpType::BitShiftRight},
    {"<<=", BinOpType::BitShiftLeft },
    {"&=",  BinOpType::BitAnd       },
    {"|=",  BinOpType::BitOr        },
    {"^=",  BinOpType::BitXor       }
};

std::unordered_map<std::string, Type*> NodeRegistry::types_ = {
    {"bool", type_factory_.mk_bool()},
    {"byte", type_factory_.mk_unknown()}, // TODO: implement `byte` type
    {"sbyte", type_factory_.mk_unknown()}, // TODO: implement `sbyte` type
    {"char", type_factory_.mk_char()},
    {"short", type_factory_.mk_unknown()}, // TODO: implement `short` type
    {"ushort", type_factory_.mk_unknown()}, // TODO: implement `ushort` type
    {"int", type_factory_.mk_int()},
    {"uint", type_factory_.mk_unknown()}, // TODO: implement `uint` type
    {"long", type_factory_.mk_unknown()}, // TODO: implement `long` type
    {"ulong", type_factory_.mk_unknown()}, // TODO: implement `ulong` type

    {"float", type_factory_.mk_float()},
    {"double", type_factory_.mk_unknown()}, // TODO: implement `double` type
    {"decimal", type_factory_.mk_unknown()}, // TODO: implement `decimal` type

    {"void", type_factory_.mk_void()},
    {"dynamic", type_factory_.mk_dynamic()},

    {"Boolean", type_factory_.mk_bool()},
    {"Byte", type_factory_.mk_unknown()}, // TODO: implement `Byte` type
    {"SByte", type_factory_.mk_unknown()}, // TODO: implement `SByte` type
    {"Char", type_factory_.mk_char()},
    {"Int16", type_factory_.mk_unknown()}, // TODO: implement `Int16` type
    {"Int32", type_factory_.mk_int()},
    {"Int64", type_factory_.mk_unknown()}, // TODO: implement `Int64` type
    {"UInt16", type_factory_.mk_unknown()}, // TODO: implement `UInt16` type
    {"UInt32", type_factory_.mk_unknown()}, // TODO: implement `UInt32` type
    {"UInt64", type_factory_.mk_unknown()}, // TODO: implement `UInt64` type

    {"Single", type_factory_.mk_float()},
    {"Double", type_factory_.mk_unknown()}, // TODO: implement `double` type
    {"Decimal", type_factory_.mk_unknown()}, // TODO: implement `decimal` type

    {"object", type_factory_.mk_class("object", {{"System"}})},
    {"Object", type_factory_.mk_class("object", {{"System"}})},
    {"string", type_factory_.mk_class("string", {{"System"}})},
    {"String", type_factory_.mk_class("string", {{"System"}})},

    {"var", type_factory_.mk_unknown()}, // todo handle var type
    {"_", type_factory_.mk_unknown()}, // todo handle discard type
};

std::unordered_map<std::string, AccessModifier> NodeRegistry::access_modifiers = {
    {"private",   AccessModifier::Private  },
    {"public",    AccessModifier::Public   },
    {"protected", AccessModifier::Protected},
    {"internal",  AccessModifier::Internal },
};

CSharpTSTreeVisitor::StmtHandler NodeRegistry::get_stmt_handler(TSNode const& node)
{
    auto const it = stmt_handlers_.find(ts_node_type(node));
    return it != stmt_handlers_.end() ? it->second : default_stmt_handler;
}

CSharpTSTreeVisitor::ExprHandler NodeRegistry::get_expr_handler(TSNode const& node)
{
    auto const it = expr_handlers_.find(ts_node_type(node));
    return it != expr_handlers_.end() ? it->second : default_expr_handler;
}

std::optional<UnaryOpType> NodeRegistry::get_prefix_unary_op(std::string const& operation)
{
    if (auto const it = prefix_unary_op.find(operation); it != prefix_unary_op.end())
    {
        return it->second;
    }
    return {};
}

std::optional<BinOpType> NodeRegistry::get_bin_op(std::string const& operation)
{
    if (auto const it = bin_operations.find(operation); it != bin_operations.end())
    {
        return it->second;
    }
    return {};
}

std::optional<Type*> NodeRegistry::get_type(std::string const& type_name)
{
    if (auto const it = types_.find(type_name); it != types_.end())
    {
        return it->second;
    }
    return {};
}

std::optional<AccessModifier> NodeRegistry::get_access_modifier(std::string const& modifier)
{
    if (auto const it = access_modifiers.find(modifier); it != access_modifiers.end())
    {
        return it->second;
    }
    return {};
}

Expr* NodeRegistry::default_expr_handler(CSharpTSTreeVisitor*, TSNode const*)
{
    return ExprFactory::get_instance().mk_unknown();
}

Stmt* NodeRegistry::default_stmt_handler(CSharpTSTreeVisitor*, TSNode const*)
{
    return StmtFactory::get_instance().mk_uknown();
}
} // namespace astfri::csharp