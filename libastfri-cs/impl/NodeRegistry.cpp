#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>

namespace astfri::csharp
{
TypeFactory& NodeRegistry::type_factory_ = TypeFactory::get_instance();

std::unordered_map<std::string, CSharpTSTreeVisitor::StmtHandler>
    NodeRegistry::stmt_handlers_ = {
        {"class_declaration",       CSharpTSTreeVisitor::handle_class_def_stmt   },
        {"variable_declaration",    CSharpTSTreeVisitor::handle_memb_var_def_stmt},
        // todo fix this might not work
        {"destructor_declaration",  CSharpTSTreeVisitor::handle_destr_def_stmt   },
        {"constructor_declaration", CSharpTSTreeVisitor::handle_constr_def_stmt
        },
        {"parameter",               CSharpTSTreeVisitor::handle_param_def_stmt   },
        {"constructor_initializer", CSharpTSTreeVisitor::handle_base_init_stmt   },
        {"constructor_declaration", CSharpTSTreeVisitor::handle_method_def_stmt
        },
        {"block",                   CSharpTSTreeVisitor::handle_block_stmt       },
        {"arrow_expression_clause", CSharpTSTreeVisitor::handle_arrow_stmt       },
        {"expression_statement",    CSharpTSTreeVisitor::handle_expr_stmt        },
        {"do_statement",            CSharpTSTreeVisitor::handle_do_while_loop    },
        {"while_statement",         CSharpTSTreeVisitor::handle_while_loop       },
        {"break_statement",         CSharpTSTreeVisitor::handle_break            },
        {"continue_statement",      CSharpTSTreeVisitor::handle_continue         },
        {"return_statement",        CSharpTSTreeVisitor::handle_return           },
        {"throw_statement",         CSharpTSTreeVisitor::handle_throw            }
        // {"field_declaration", CSharpTSTreeVisitor::handle_memb_var_def_stmt},
};

std::unordered_map<std::string, CSharpTSTreeVisitor::ExprHandler>
    NodeRegistry::expr_handlers_ = {
        {"integer_literal", CSharpTSTreeVisitor::handle_int_lit},
        {"real_literal", CSharpTSTreeVisitor::handle_float_lit},
        {"boolean_literal", CSharpTSTreeVisitor::handle_bool_lit},
        {"character_literal", CSharpTSTreeVisitor::handle_char_lit},
        {"string_literal", CSharpTSTreeVisitor::handle_str_lit},
        {"null_literal", CSharpTSTreeVisitor::handle_null_literal},
        {"verbatim_string_literal", CSharpTSTreeVisitor::handle_verbatim_str_lit
        },
        {"this_expression", CSharpTSTreeVisitor::handle_this_expr},
        {"conditional_expression", CSharpTSTreeVisitor::handle_ternary_expr},
        {"prefix_unary_expression",
         CSharpTSTreeVisitor::handle_prefix_unary_op_expr},
        {"ref_expression", CSharpTSTreeVisitor::handle_prefix_unary_op_expr},
        {"postfix_unary_expression",
         CSharpTSTreeVisitor::handle_postfix_unary_op_expr},
        {"binary_expression", CSharpTSTreeVisitor::handle_binary_op_expr},
        {"assignment_expression", CSharpTSTreeVisitor::handle_binary_op_expr},

        {"ERROR",
         [](CSharpTSTreeVisitor*, const TSNode* node) -> Expr*
         {
             const auto [row, column] = ts_node_start_point(*node);
             throw std::runtime_error(
                 "Invalid C# syntax in source code at: row"
                 + std::to_string(row) + "and column " + std::to_string(column)
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

std::unordered_map<std::string, CSModifier> NodeRegistry::modifiers = {
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
};

CSharpTSTreeVisitor::StmtHandler NodeRegistry::get_stmt_handler(
    const TSNode& node
)
{
    return get_stmt_handler(ts_node_type(node));
}

CSharpTSTreeVisitor::ExprHandler NodeRegistry::get_expr_handler(
    const TSNode& node
)
{
    return get_expr_handler(ts_node_type(node));
}

CSharpTSTreeVisitor::StmtHandler NodeRegistry::get_stmt_handler(
    const std::string& node_type
)
{
    const auto it = stmt_handlers_.find(node_type);
    return it != stmt_handlers_.end() ? it->second : default_stmt_handler;
}

CSharpTSTreeVisitor::ExprHandler NodeRegistry::get_expr_handler(
    const std::string& node_type
)
{
    const auto it = expr_handlers_.find(node_type);
    return it != expr_handlers_.end() ? it->second : default_expr_handler;
}

std::optional<UnaryOpType> NodeRegistry::get_prefix_unary_op(
    const std::string& operation
)
{
    const auto it = prefix_unary_op.find(operation);
    if (it != prefix_unary_op.end())
    {
        return it->second;
    }
    return {};
}

std::optional<BinOpType> NodeRegistry::get_bin_op(const std::string& operation)
{
    const auto it = bin_operations.find(operation);
    if (it != bin_operations.end())
    {
        return it->second;
    }
    return {};
}

std::optional<Type*> NodeRegistry::get_type(const std::string& type_name)
{
    const auto it = types_.find(type_name);
    if (it != types_.end())
    {
        return it->second;
    }
    return {};
}

std::optional<CSModifier> NodeRegistry::get_modifier(
    const std::string& modifier
)
{
    const auto it = modifiers.find(modifier);
    if (it != modifiers.end())
    {
        return it->second;
    }
    return {};
}

Expr* NodeRegistry::default_expr_handler(CSharpTSTreeVisitor*, const TSNode*)
{
    return ExprFactory::get_instance().mk_unknown();
}

Stmt* NodeRegistry::default_stmt_handler(CSharpTSTreeVisitor*, const TSNode*)
{
    return StmtFactory::get_instance().mk_uknown();
}
} // namespace astfri::csharp