#include <astfri/Astfri.hpp>

#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Maps.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/visitors/src_code/SrcCodeVisitor.hpp>
#include <libastfri-cs/impl/visitors/SymbTableBuilder.hpp>
#include <libastfri-cs/impl/visitors/TypeTranslator.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <optional>
#include <string>

namespace astfri::csharp {

class SrcCodeVisitor;
class SymbTableBuilder;
class TypeTranslator;

namespace maps {
namespace {
using enum NodeType;

/**
 * @brief Helper struct for mapping tree-sitter node names to NodeType enum
 * values
 */
struct MappingRule {
    std::string_view nodeName;
    NodeType nodeType;
    bool isNamed;
};

constexpr MappingRule mappingRules_[] = {
    {"compilation_unit",                  CompilationUnit,             true },
    {"namespace_declaration",             NamespaceDecl,               true },
    {"class_declaration",                 ClassDecl,                   true },
    {"struct_declaration",                StructDecl,                  true },
    {"interface_declaration",             InterfaceDecl,               true },
    {"enum_declaration",                  EnumDecl,                    true },
    {"delegate_declaration",              DelegateDecl,                true },
    {"record_declaration",                RecordDecl,                  true },
    {"constructor_declaration",           ConstructorDecl,             true },
    {"constructor_initializer",           ConstructorInit,             true },
    {"destructor_declaration",            DestructorDecl,              true },
    {"method_declaration",                MethodDecl,                  true },
    {"field_declaration",                 MemberVarDef,                true },
    {"property_declaration",              PropertyDecl,                true },
    {"indexer_declaration",               IndexerDecl,                 true },
    {"operator_declaration",              OprDecl,                     true },
    {"conversion_operator_declaration",   CastOprDecl,                 true },
    {"local_function_statement",          LocalFuncDecl,               true },
    {"parameter",                         ParameterDecl,               true },
    {"local_declaration_statement",       LocalVarDef,                 true },
    {"block",                             Block,                       true },
    {"arrow_expression_clause",           ArrowExprClaus,              true },
    {"do_statement",                      DoWhileLoop,                 true },
    {"while_statement",                   WhileLoop,                   true },
    {"for_statement",                     ForLoop,                     true },
    {"foreach_statement",                 ForeachLoop,                 true },
    {"break_statement",                   BreakStmt,                   true },
    {"continue_statement",                ContinueStmt,                true },
    {"if_statement",                      IfStmt,                      true },
    {"switch_statement",                  SwitchStmt,                  true },
    {"switch_section",                    SwitchSection,               true },
    {"try_statement",                     TryStmt,                     true },
    {"catch_clause",                      CatchClause,                 true },
    {"finally_clause",                    FinallyClause,               true },
    {"catch_declaration",                 CatchDecl,                   true },
    {"expression_statement",              ExprStmt,                    true },
    {"return_statement",                  ReturnStmt,                  true },
    {"throw_statement",                   ThrowStmt,                   true },
    {"integer_literal",                   IntLit,                      true },
    {"real_literal",                      RealLit,                     true },
    {"boolean_literal",                   BoolLit,                     true },
    {"character_literal",                 CharLit,                     true },
    {"string_literal",                    StrLit,                      true },
    {"verbatim_string_literal",           VerbatimStrLit,              true },
    {"raw_string_literal",                RawStrLit,                   true },
    {"null_literal",                      NullLit,                     true },
    {"prefix_unary_expression",           PrefixUnOpr,                 true },
    {"postfix_unary_expression",          PostfixUnOpr,                true },
    {"binary_expression",                 BinaryOpr,                   true },
    {"conditional_expression",            TernaryOpr,                  true },
    {"assignment_expression",             Assignment,                  true },
    {"=",                                 Assign,                      false},
    {"+",                                 Plus,                        false},
    {"-",                                 Minus,                       false},
    {"*",                                 Asterisk,                    false},
    {"/",                                 Divide,                      false},
    {"%",                                 Modulo,                      false},
    {"==",                                Equal,                       false},
    {"!=",                                NotEqual,                    false},
    {"<",                                 Less,                        false},
    {"<=",                                LessEqual,                   false},
    {">",                                 Greater,                     false},
    {">=",                                GreaterEqual,                false},
    {"&&",                                LogicalAnd,                  false},
    {"||",                                LogicalOr,                   false},
    {">>",                                BitShiftRight,               false},
    {">>>",                               BitShiftRightUnsigned,       false},
    {"<<",                                BitShiftLeft,                false},
    {"&",                                 Ampersand,                   false},
    {"|",                                 BitOr,                       false},
    {"^",                                 BitXor,                      false},
    {"+=",                                AddAssign,                   false},
    {"-=",                                SubtractAssign,              false},
    {"*=",                                MultiplyAssign,              false},
    {"/=",                                DivideAssign,                false},
    {"%=",                                ModuloAssign,                false},
    {">>=",                               BitShiftRightAssign,         false},
    {">>>=",                              BitShiftRightUnsignedAssign, false},
    {"<<=",                               BitShiftLeftAssign,          false},
    {"&=",                                BitAndAssign,                false},
    {"|=",                                BitOrAssign,                 false},
    {"^=",                                BitXorAssign,                false},
    {"++",                                Increment,                   false},
    {"--",                                Decrement,                   false},
    {"~",                                 BitFlip,                     false},
    {"!",                                 LogicalNot,                  false},
    {"constant_pattern",                  ConstPattern,                true },
    {"this",                              This,                        false},
    {"base",                              Base,                        false},
    {"ref_expression",                    RefExpr,                     true },
    {"parenthesized_expression",          ParenthesizedExpr,           true },
    {"member_access_expression",          MemberAccess,                true },
    {"invocation_expression",             Invocation,                  true },
    {"cast_expression",                   CastExpr,                    true },
    {"lambda_expression",                 LambdaExpr,                  true },
    {"predefined_type",                   PredefinedType,              true },
    {"qualified_name",                    QualifName,                  true },
    {"implicit_type",                     ImplicitType,                true },
    {"nullable_type",                     NullableType,                true },
    {"pointer_type",                      PointerType,                 true },
    {"ref_type",                          RefType,                     true },
    {"array_type",                        ArrayType,                   true },
    {"generic_name",                      GenericType,                 true },
    {"tuple_type",                        TupleType,                   true },
    {"function_pointer_type",             FuncPointerType,             true },
    {"scoped_type",                       ScopedType,                  true },
    {"modifier",                          Modifier,                    true },
    {"identifier",                        Identifier,                  true },
    {"global",                            Global,                      false},
    {"static",                            Static,                      false},
    {"alias_qualified_name",              AliasQualifName,             true },
    {"base_list",                         BaseList,                    true },
    {"type_parameter_list",               TypeParamList,               true },
    {"type_parameter_constraints_clause", TypeParamConstrClause,       true },
    {"type_parameter_constraint",         TypeParamConstraint,         true },
};

} // namespace

NodeTypes::NodeTypes() {
    const TSLanguage* lang = tree_sitter_c_sharp();
    const uint32_t size    = ts_language_symbol_count(lang);
    nodeTypeMap_.assign(size, Unknown);
    symbolMap_.assign(size, INVALID_SYMBOL);
    for (const auto& [nodeName, nodeType, isNamed] : mappingRules_) {
        const TSSymbol s                            = util::symbol_for_name(nodeName, isNamed);
        nodeTypeMap_[s]                             = nodeType;
        symbolMap_[static_cast<TSSymbol>(nodeType)] = s;
    }
}

TSSymbol NodeTypes::get_symbol(const NodeType type) const {
    return symbolMap_.at(static_cast<TSSymbol>(type));
}

NodeType NodeTypes::get_node_type(const TSNode& node) const {
    return nodeTypeMap_.at(ts_node_symbol(node));
}

Mappers::Mappers() :
    stmts({
        {ClassDecl,       SrcCodeVisitor::visit_class_def    },
        {StructDecl,      SrcCodeVisitor::visit_class_def    },
        {InterfaceDecl,   SrcCodeVisitor::visit_interface_def},
        {ConstructorDecl, SrcCodeVisitor::visit_constr_def   },
        {ConstructorInit, SrcCodeVisitor::visit_constr_init  },
        {MethodDecl,      SrcCodeVisitor::visit_method_def   },
        {DestructorDecl,  SrcCodeVisitor::visit_destr_def    },
        {MemberVarDef,    SrcCodeVisitor::visit_memb_var_def },
        {LocalFuncDecl,   SrcCodeVisitor::visit_func_stmt    },
        {ParameterDecl,   SrcCodeVisitor::visit_param_def    },
        {LocalVarDef,     SrcCodeVisitor::visit_local_var_def},
        {Block,           SrcCodeVisitor::visit_block        },
        {ArrowExprClaus,  SrcCodeVisitor::visit_arrow_body   },
        {DoWhileLoop,     SrcCodeVisitor::visit_do_while     },
        {WhileLoop,       SrcCodeVisitor::visit_while        },
        {ForLoop,         SrcCodeVisitor::visit_for_loop     },
        {ForeachLoop,     SrcCodeVisitor::visit_for_each     },
        {BreakStmt,       SrcCodeVisitor::visit_break        },
        {ContinueStmt,    SrcCodeVisitor::visit_continue     },
        {IfStmt,          SrcCodeVisitor::visit_if           },
        {TryStmt,         SrcCodeVisitor::visit_try          },
        {CatchClause,     SrcCodeVisitor::visit_catch        },
        {FinallyClause,   SrcCodeVisitor::visit_finally      },
        {CatchDecl,       SrcCodeVisitor::visit_catch_decl   },
        {SwitchStmt,      SrcCodeVisitor::visit_switch       },
        {SwitchSection,   SrcCodeVisitor::visit_case_stmt    },
        {ExprStmt,        SrcCodeVisitor::visit_expr_stmt    },
        {ReturnStmt,      SrcCodeVisitor::visit_return       },
        {ThrowStmt,       SrcCodeVisitor::visit_throw        },
}),
    exprs({
        {IntLit, SrcCodeVisitor::visit_int_lit},
        {RealLit, SrcCodeVisitor::visit_float_lit},
        {BoolLit, SrcCodeVisitor::visit_bool_lit},
        {CharLit, SrcCodeVisitor::visit_char_lit},
        {StrLit, SrcCodeVisitor::visit_str_lit},
        {NullLit, SrcCodeVisitor::visit_null_lit},
        {VerbatimStrLit, SrcCodeVisitor::visit_verbatim_str_lit},
        {RawStrLit, SrcCodeVisitor::visit_raw_str_lit},
        {This, SrcCodeVisitor::visit_this_expr},
        {Base, SrcCodeVisitor::visit_base_expr},
        {PrefixUnOpr, SrcCodeVisitor::visit_prefix_unary_opr},
        {PostfixUnOpr, SrcCodeVisitor::visit_postfix_unary_opr},
        {BinaryOpr, SrcCodeVisitor::visit_binary_opr},
        {TernaryOpr, SrcCodeVisitor::visit_ternary_expr},
        {RefExpr, SrcCodeVisitor::visit_ref_expr},
        {Assignment, SrcCodeVisitor::visit_binary_opr},
        {ParenthesizedExpr, SrcCodeVisitor::visit_parenthesized_expr},
        {Identifier, SrcCodeVisitor::visit_identifier},
        {MemberAccess, SrcCodeVisitor::visit_memb_access},
        {Invocation, SrcCodeVisitor::visit_invoc},
        {ConstPattern, SrcCodeVisitor::visit_const_pattern},
    }),
    types({
        {PredefinedType, TypeTranslator::visit_predefined},
        {Identifier, TypeTranslator::visit_identitifier},
        {QualifName, TypeTranslator::visit_qualified_name},   // FQN, PQN
        {ImplicitType, TypeTranslator::visit_qualified_name}, // var
        {NullableType, TypeTranslator::visit_wrapper},
        {PointerType, TypeTranslator::visit_indirect},
        {RefType, TypeTranslator::visit_indirect},
        {ArrayType, TypeTranslator::visit_array},
        {GenericType, TypeTranslator::visit_generic_name},
        {TupleType, TypeTranslator::visit_tuple}, // function pointer - not delegate
        {FuncPointerType, TypeTranslator::visit_func_pointer},
        {ScopedType, TypeTranslator::visit_wrapper},
    }),
    typeCollectors({
        {ClassDecl, SymbTableBuilder::visit_class},
        {StructDecl, SymbTableBuilder::visit_class},
        {InterfaceDecl, SymbTableBuilder::visit_interface},
        {EnumDecl, SymbTableBuilder::visit_enum},
        {DelegateDecl, SymbTableBuilder::visit_delegate},
        {RecordDecl, SymbTableBuilder::visit_record},
    }),
    symbCollectors({
        {MemberVarDef, SymbTableBuilder::visit_memb_var},
        {PropertyDecl, SymbTableBuilder::visit_property},
        {MethodDecl, SymbTableBuilder::visit_method},
        {BaseList, SymbTableBuilder::visit_base_list},
        {TypeParamConstraint, SymbTableBuilder::visit_type_param_constraint},
    }) {
}

Operations::Operations() :
    prefixUnaryOps({
        {Plus,       UnaryOpType::Plus        },
        {Minus,      UnaryOpType::Minus       },
        {Increment,  UnaryOpType::PreIncrement},
        {Decrement,  UnaryOpType::PreDecrement},
        {LogicalNot, UnaryOpType::LogicalNot  },
        {BitFlip,    UnaryOpType::BitFlip     },
        {Asterisk,   UnaryOpType::Dereference },
        {Ampersand,  UnaryOpType::AddressOf   },
}),
    binaryOps(
        {{Assign, BinOpType::Assign},
         {Plus, BinOpType::Add},
         {Minus, BinOpType::Subtract},
         {Asterisk, BinOpType::Multiply},
         {Divide, BinOpType::Divide},
         {Modulo, BinOpType::Modulo},
         {Equal, BinOpType::Equal},
         {NotEqual, BinOpType::NotEqual},
         {Less, BinOpType::Less},
         {LessEqual, BinOpType::LessEqual},
         {Greater, BinOpType::Greater},
         {GreaterEqual, BinOpType::GreaterEqual},
         {LogicalAnd, BinOpType::LogicalAnd},
         {LogicalOr, BinOpType::LogicalOr},
         {Ampersand, BinOpType::BitAnd},
         {BitOr, BinOpType::BitOr},
         {BitXor, BinOpType::BitXor},
         {BitShiftRight, BinOpType::BitShiftRight},
         {BitShiftRightUnsigned, BinOpType::BitShiftRightUnsigned},
         {BitShiftLeft, BinOpType::BitShiftLeft},
         {AddAssign, BinOpType::AddAssign},
         {SubtractAssign, BinOpType::SubtractAssign},
         {MultiplyAssign, BinOpType::MultiplyAssign},
         {DivideAssign, BinOpType::DivideAssign},
         {ModuloAssign, BinOpType::ModuloAssign},
         {BitShiftRightAssign, BinOpType::BitShiftRightAssign},
         {BitShiftLeftAssign, BinOpType::BitShiftLeftAssign},
        // todo add right unsigned bit shift assign
         {BitAndAssign, BinOpType::BitAndAssign},
         {BitOrAssign, BinOpType::BitOrAssign},
         {BitXorAssign, BinOpType::BitXorAssign}}
    ) {
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
}) {
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
        {"long", typeFact.mk_int()}, // TODO: implement `long` type
        {"ulong", typeFact.mk_int()}, // TODO: implement `ulong` type

        {"float", typeFact.mk_float()},
        {"double", typeFact.mk_float()}, // TODO: implement `double` type
        {"decimal", typeFact.mk_float()}, // TODO: implement `decimal` type

        {"nint", typeFact.mk_int()}, // TODO: implement `nint` type
        {"nuint", typeFact.mk_int()}, // TODO: implement `nuint` type

        {"void", typeFact.mk_void()},
        {"dynamic", typeFact.mk_dynamic()},

        {"Boolean", typeFact.mk_bool()},
        {"Byte", typeFact.mk_int()}, // TODO: implement `Byte` type
        {"SByte", typeFact.mk_int()}, // TODO: implement `SByte` type
        {"Char", typeFact.mk_char()},
        {"Int16", typeFact.mk_int()}, // TODO: implement `Int16` type
        {"Int32", typeFact.mk_int()},
        {"Int64", typeFact.mk_int()}, // TODO: implement `Int64` type
        {"UInt16", typeFact.mk_int()}, // TODO: implement `UInt16` type
        {"UInt32", typeFact.mk_int()}, // TODO: implement `UInt32` type
        {"UInt64", typeFact.mk_int()}, // TODO: implement `UInt64` type

        {"IntPtr", typeFact.mk_int()}, // TODO: implement IntPtr type
        {"UIntPtr", typeFact.mk_int()}, // TODO: implement UIntPtr type

        {"Single", typeFact.mk_float()},
        {"Double", typeFact.mk_float()}, // TODO: implement `double` type
        {"Decimal", typeFact.mk_float()}, // TODO: implement `decimal` type

        {"object", typeFact.mk_class("Object", {{"System"}})},
        {"Object", typeFact.mk_class("Object", {{"System"}})},
        {"string", typeFact.mk_class("String", {{"System"}})},
        {"String", typeFact.mk_class("String", {{"System"}})},

        {"var", typeFact.mk_deduced(nullptr)},
        {"_", typeFact.mk_unknown()}, // todo handle discard type
}) {
}

} // namespace maps

maps::Mappers MapManager::handlers_;
maps::Modifiers MapManager::modifiers_;
maps::NodeTypes MapManager::nodeTypes_;
maps::Operations MapManager::operations_;
maps::Types MapManager::types_;

StmtMapper MapManager::get_stmt_mapper(const TSNode& node) {
    return get_stmt_mapper(nodeTypes_.get_node_type(node));
}

StmtMapper MapManager::get_stmt_mapper(const NodeType nodeType) {
    // todo redo this when the typo is fixed
    // std::cerr << "Entering statement handler: " << nodeType << "\n";
    const StmtMapper def = default_stmt_visit;
    return get_or_default(handlers_.stmts, nodeType, def);
}

ExprMapper MapManager::get_expr_mapper(const TSNode& node) {
    return get_expr_mapper(nodeTypes_.get_node_type(node));
}

ExprMapper MapManager::get_expr_mapper(const NodeType nodeType) {
    const ExprMapper def = default_visit<ExprFactory, SrcCodeVisitor, Expr*>;
    return get_or_default(handlers_.exprs, nodeType, def);
}

TypeMapper MapManager::get_type_mapper(const TSNode& node) {
    return get_type_mapper(nodeTypes_.get_node_type(node));
}

TypeMapper MapManager::get_type_mapper(const NodeType nodeType) {
    const TypeMapper def = default_visit<TypeFactory, TypeTranslator, Type*>;
    return get_or_default(handlers_.types, nodeType, def);
}

TypeCollector MapManager::get_type_collector(const TSNode& node) {
    return get_type_collector(nodeTypes_.get_node_type(node));
}

TypeCollector MapManager::get_type_collector(const NodeType nodeType) {
    const TypeCollector def = [](auto*, const auto &) -> ScopeNode* { return nullptr; };
    return get_or_default(handlers_.typeCollectors, nodeType, def);
}

MemberCollector MapManager::get_symb_collector(const TSNode& node) {
    return get_symb_collector(nodeTypes_.get_node_type(node));
}

MemberCollector MapManager::get_symb_collector(const NodeType nodeType) {
    const MemberCollector def = [](auto*, const auto&) { };
    return get_or_default(handlers_.symbCollectors, nodeType, def);
}

std::optional<UnaryOpType> MapManager::get_prefix_unary_op(const TSNode& op) {
    return get_opt(operations_.prefixUnaryOps, get_node_type(op));
}

std::optional<BinOpType> MapManager::get_bin_op(const TSNode& op) {
    return get_opt(operations_.binaryOps, get_node_type(op));
}

Type* MapManager::get_primitive_type(const std::string_view nodeType) {
    return get_opt(types_.types, nodeType).value_or(nullptr);
}

CSModifier MapManager::get_modifier(const TSNode& node, const std::string_view src) {
    return get_modifier(util::extract_text(node, src));
}

CSModifier MapManager::get_modifier(const std::string_view modifs) {
    return get_opt(modifiers_.modifiers, modifs).value_or(CSModifier::None);
}

bool MapManager::is_expr(const TSNode& node) {
    return handlers_.exprs.contains(nodeTypes_.get_node_type(node));
}

bool MapManager::is_stmt(const TSNode& node) {
    return handlers_.stmts.contains(nodeTypes_.get_node_type(node));
}

NodeType MapManager::get_node_type(const TSNode& node) {
    return nodeTypes_.get_node_type(node);
}

TSSymbol MapManager::get_symbol(const NodeType type) {
    return nodeTypes_.get_symbol(type);
}

Stmt* MapManager::default_stmt_visit(SrcCodeVisitor*, const TSNode&) {
    // todo remove this when type is fixed
    return StmtFactory::get_instance().mk_uknown();
}

} // namespace astfri::csharp
