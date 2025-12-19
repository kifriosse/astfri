#ifndef CSHARP_REGISTRIES_HPP
#define CSHARP_REGISTRIES_HPP

#include <libastfri-cs/impl/CSModifiers.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <optional>
#include <string>
#include <unordered_map>

#include "SymbolTableBuilder.hpp"
#include "visitor/SourceCodeVisitor.hpp"

namespace astfri::csharp
{

namespace regs
{

inline static const std::string top_level_stmt_query =
    R"(
    (namespace_declaration
        body: (declaration_list
        [
            (class_declaration)
            (interface_declaration)
            (struct_declaration)
            (enum_declaration)
            (delegate_declaration)
            (record_declaration)
        ] @top_level)
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

struct Handlers
{
    std::unordered_map<std::string, SourceCodeVisitor::StmtHandler> stmts;
    std::unordered_map<std::string, SourceCodeVisitor::ExprHandler> exprs;
    std::unordered_map<std::string, SymbolTableBuilder::RegHandler>
        symbol_reg_handlers;
    Handlers();
};

struct Operations
{
    std::unordered_map<std::string, UnaryOpType> prefix_unary_op;
    std::unordered_map<std::string, BinOpType> bin_operations;
    Operations();
};

struct Types
{
    TypeFactory& type_factory;
    std::unordered_map<std::string, Type*> types;
    Types();
};

struct Modifiers
{
    std::unordered_map<std::string, CSModifier> modifiers;
    Modifiers();
};
} // namespace regs

class RegManager
{
private:
    static regs::Handlers handlers_;
    static regs::Operations operations_;
    static regs::Types types_;
    static regs::Modifiers modifiers_;

public:
    static SourceCodeVisitor::StmtHandler get_stmt_handler(const TSNode& node);
    static SourceCodeVisitor::ExprHandler get_expr_handler(const TSNode& node);
    static SymbolTableBuilder::RegHandler get_reg_handler(const TSNode& node);
    static SourceCodeVisitor::StmtHandler get_stmt_handler(
        const std::string& node_type
    );
    static SourceCodeVisitor::ExprHandler get_expr_handler(
        const std::string& node_type
    );
    static SymbolTableBuilder::RegHandler get_reg_handler(
        const std::string& node_type
    );
    static std::optional<UnaryOpType> get_prefix_unary_op(
        const std::string& operation
    );
    static std::optional<BinOpType> get_bin_op(const std::string& operation);
    static std::optional<Type*> get_type(const std::string& type_name);
    static std::optional<CSModifier> get_modifier(const std::string& modifier);
    static bool is_expr(const TSNode& node);
    static bool is_stmt(const TSNode& node);

private:
    static Expr* default_expr_handler(SourceCodeVisitor*, const TSNode*);
    static Stmt* default_stmt_handler(SourceCodeVisitor*, const TSNode*);
};
} // namespace astfri::csharp
#endif // CSHARP_REGISTRIES_HPP