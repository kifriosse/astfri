#include <astfri/Astfri.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <tuple>

using namespace astfri;


namespace {


using LiteralExprs = std::tuple<
    IntLiteralExpr,
    FloatLiteralExpr,
    CharLiteralExpr,
    StringLiteralExpr,
    BoolLiteralExpr,
    NullLiteralExpr>;

using NonLiteralExprs = std::tuple<
    IfExpr,
    BinOpExpr,
    UnaryOpExpr,
    ParamVarRefExpr,
    LocalVarRefExpr,
    MemberVarRefExpr,
    GlobalVarRefExpr,
    ClassRefExpr,
    FunctionCallExpr,
    MethodCallExpr,
    LambdaCallExpr,
    LambdaExpr,
    ThisExpr,
    BaseExpr,
    ConstructorCallExpr,
    NewExpr,
    DeleteExpr,
    BracketExpr,
    NamedExpr,
    UnknownExpr>;

using VarDefStmts = std::tuple<
    LocalVarDefStmt,
    ParamVarDefStmt,
    MemberVarDefStmt,
    GlobalVarDefStmt>;

using LabelStmts = std::tuple<
    GotoLabelStmt,
    CaseStmt,
    DefaultCaseStmt>;

using UserTypeDefStmts = std::tuple<
    InterfaceDefStmt,
    ClassDefStmt>;

using OtherStmts = std::tuple<
    MultiVarDefStmt,
    FunctionDefStmt,
    MethodDefStmt,
    BaseInitializerStmt,
    SelfInitializerStmt,
    MemberInitializerStmt,
    ConstructorDefStmt,
    DestructorDefStmt,
    GenericParam,
    CompoundStmt,
    ReturnStmt,
    ExprStmt,
    IfStmt,
    GotoStmt,
    SwitchStmt,
    WhileStmt,
    DoWhileStmt,
    ForStmt,
    ForEachStmt,
    ThrowStmt,
    CatchStmt,
    TryStmt,
    ContinueStmt,
    BreakStmt,
    UnknownStmt,
    TranslationUnit>;

using ScopedTypes = std::tuple<
    ClassType,
    InterfaceType>;

using OtherTypes = std::tuple<
    DynamicType,
    IntType,
    FloatType,
    CharType,
    BoolType,
    VoidType,
    IndirectionType,
    LambdaType,
    DeducedType,
    IncompleteType,
    UnknownType>;


// Concatenation of tuple type lists (only needs to be done at type level).
template<typename... Tuples>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

using ExprNodes = tuple_cat_t<LiteralExprs, NonLiteralExprs>;
using StmtNodes = tuple_cat_t<VarDefStmts, LabelStmts, UserTypeDefStmts, OtherStmts>;
using TypeNodes = tuple_cat_t<ScopedTypes, OtherTypes>;
using AllNodes  = tuple_cat_t<ExprNodes, StmtNodes, TypeNodes>;


} // namespace


TEST_CASE( "`is` type queries", "[type-info]" ) {

    SECTION( "distinguishes sibling types with the same parent" ) {
        IntLiteralExpr int_lit;
        FloatLiteralExpr float_lit;
        ASTNode *a = &int_lit;
        ASTNode *b = &float_lit;

        REQUIRE( is<IntLiteralExpr>(a) );
        REQUIRE_FALSE( is<FloatLiteralExpr>(a) );
        REQUIRE( is<FloatLiteralExpr>(b) );
        REQUIRE_FALSE( is<IntLiteralExpr>(b) );
    }

    SECTION( "is exact -- parent types do not match" ) {
        IntLiteralExpr node;
        ASTNode *p = &node;

        REQUIRE( is<IntLiteralExpr>(p) );
        REQUIRE_FALSE( is<LiteralExpr>(p) );
        REQUIRE_FALSE( is<Expr>(p) );
        REQUIRE_FALSE( is<ASTNode>(p) );
    }

    SECTION( "is exact for deeper hierarchies" ) {
        LocalVarDefStmt node;
        ASTNode *p = &node;

        REQUIRE( is<LocalVarDefStmt>(p) );
        REQUIRE_FALSE( is<VarDefStmt>(p) );
        REQUIRE_FALSE( is<Stmt>(p) );
        REQUIRE_FALSE( is<ASTNode>(p) );
    }

    SECTION( "types from different branches never match" ) {
        IntLiteralExpr expr;
        ClassDefStmt stmt;
        IntType type;
        ASTNode *e = &expr;
        ASTNode *s = &stmt;
        ASTNode *t = &type;

        REQUIRE_FALSE( is<ClassDefStmt>(e) );
        REQUIRE_FALSE( is<IntType>(e) );
        REQUIRE_FALSE( is<IntLiteralExpr>(s) );
        REQUIRE_FALSE( is<IntType>(s) );
        REQUIRE_FALSE( is<IntLiteralExpr>(t) );
        REQUIRE_FALSE( is<ClassDefStmt>(t) );
    }

    SECTION( "types with similar names are not confused" ) {
        // IfExpr vs IfStmt, ClassType vs ClassDefStmt vs ClassRefExpr,
        // InterfaceType vs InterfaceDefStmt
        IfExpr if_expr;
        IfStmt if_stmt;
        ClassType class_type;
        ClassDefStmt class_def;
        ClassRefExpr class_ref;
        InterfaceType iface_type;
        InterfaceDefStmt iface_def;

        REQUIRE( is<IfExpr>(&if_expr) );
        REQUIRE_FALSE( is<IfStmt>(&if_expr) );
        REQUIRE( is<IfStmt>(&if_stmt) );
        REQUIRE_FALSE( is<IfExpr>(&if_stmt) );

        REQUIRE( is<ClassType>(&class_type) );
        REQUIRE_FALSE( is<ClassDefStmt>(&class_type) );
        REQUIRE_FALSE( is<ClassRefExpr>(&class_type) );
        REQUIRE( is<ClassDefStmt>(&class_def) );
        REQUIRE_FALSE( is<ClassType>(&class_def) );
        REQUIRE( is<ClassRefExpr>(&class_ref) );
        REQUIRE_FALSE( is<ClassType>(&class_ref) );

        REQUIRE( is<InterfaceType>(&iface_type) );
        REQUIRE_FALSE( is<InterfaceDefStmt>(&iface_type) );
        REQUIRE( is<InterfaceDefStmt>(&iface_def) );
        REQUIRE_FALSE( is<InterfaceType>(&iface_def) );
    }

    SECTION( "works through a pointer to a derived node type" ) {
        BinOpExpr node;
        BinOpExpr *derived_ptr = &node;
        Expr *expr_ptr = &node;

        REQUIRE( is<BinOpExpr>(derived_ptr) );
        REQUIRE( is<BinOpExpr>(expr_ptr) );
        REQUIRE_FALSE( is<UnaryOpExpr>(expr_ptr) );
    }

    SECTION( "is consistent with `as`" ) {
        WhileStmt node;
        ASTNode *p = &node;

        REQUIRE( is<WhileStmt>(p) == (as<WhileStmt>(p) != nullptr) );
        REQUIRE( is<DoWhileStmt>(p) == (as<DoWhileStmt>(p) != nullptr) );
        REQUIRE( as<WhileStmt>(p) == &node );
        REQUIRE( as<DoWhileStmt>(p) == nullptr );
    }
}


TEMPLATE_LIST_TEST_CASE(
    "`is` is true for the node's own type",
    "[type-info]",
    AllNodes
) {
    TestType node;
    ASTNode *p = &node;

    REQUIRE( is<TestType>(p) );
    REQUIRE( as<TestType>(p) == &node );
}


TEMPLATE_LIST_TEST_CASE(
    "`is` is false for abstract bases of the node",
    "[type-info]",
    AllNodes
) {
    TestType node;
    ASTNode *p = &node;

    REQUIRE_FALSE( is<ASTNode>(p) );
    REQUIRE_FALSE( is<Expr>(p) );
    REQUIRE_FALSE( is<Stmt>(p) );
    REQUIRE_FALSE( is<Type>(p) );
    REQUIRE_FALSE( is<LiteralExpr>(p) );
    REQUIRE_FALSE( is<VarDefStmt>(p) );
    REQUIRE_FALSE( is<LabelStmt>(p) );
    REQUIRE_FALSE( is<UserTypeDefStmt>(p) );
    REQUIRE_FALSE( is<ScopedType>(p) );
}



TEST_CASE( "`is_a` type queries", "[type-info]" ) {

    SECTION( "node is-a its own type" ) {
        IntLiteralExpr lit;
        LocalVarDefStmt var;
        ClassType type;

        REQUIRE( is_a<IntLiteralExpr>(&lit) );
        REQUIRE( is_a<LocalVarDefStmt>(&var) );
        REQUIRE( is_a<ClassType>(&type) );
    }

    SECTION( "literal expression hierarchy" ) {
        IntLiteralExpr node;
        ASTNode *p = &node;

        REQUIRE( is_a<IntLiteralExpr>(p) );
        REQUIRE( is_a<LiteralExpr>(p) );
        REQUIRE( is_a<Expr>(p) );
        REQUIRE( is_a<ASTNode>(p) );

        REQUIRE_FALSE( is_a<FloatLiteralExpr>(p) );
        REQUIRE_FALSE( is_a<IfExpr>(p) );
        REQUIRE_FALSE( is_a<Stmt>(p) );
        REQUIRE_FALSE( is_a<Type>(p) );
    }

    SECTION( "non-literal expression is not a LiteralExpr" ) {
        BinOpExpr node;
        ASTNode *p = &node;

        REQUIRE( is_a<BinOpExpr>(p) );
        REQUIRE( is_a<Expr>(p) );
        REQUIRE( is_a<ASTNode>(p) );
        REQUIRE_FALSE( is_a<LiteralExpr>(p) );
    }

    SECTION( "variable definition hierarchy (three levels below ASTNode)" ) {
        MemberVarDefStmt node;
        ASTNode *p = &node;

        REQUIRE( is_a<MemberVarDefStmt>(p) );
        REQUIRE( is_a<VarDefStmt>(p) );
        REQUIRE( is_a<Stmt>(p) );
        REQUIRE( is_a<ASTNode>(p) );

        REQUIRE_FALSE( is_a<LocalVarDefStmt>(p) );
        REQUIRE_FALSE( is_a<GlobalVarDefStmt>(p) );
        REQUIRE_FALSE( is_a<MultiVarDefStmt>(p) );
        REQUIRE_FALSE( is_a<Expr>(p) );
        REQUIRE_FALSE( is_a<Type>(p) );
    }

    SECTION( "label statement hierarchy" ) {
        CaseStmt case_stmt;
        DefaultCaseStmt default_stmt;
        GotoLabelStmt label_stmt;
        GotoStmt goto_stmt;

        REQUIRE( is_a<LabelStmt>(&case_stmt) );
        REQUIRE( is_a<LabelStmt>(&default_stmt) );
        REQUIRE( is_a<LabelStmt>(&label_stmt) );
        REQUIRE( is_a<Stmt>(&case_stmt) );

        // GotoStmt refers to a label, but is not a label itself.
        REQUIRE_FALSE( is_a<LabelStmt>(&goto_stmt) );
        REQUIRE( is_a<Stmt>(&goto_stmt) );

        REQUIRE_FALSE( is_a<CaseStmt>(&default_stmt) );
        REQUIRE_FALSE( is_a<DefaultCaseStmt>(&case_stmt) );
    }

    SECTION( "user type definition hierarchy" ) {
        ClassDefStmt class_def;
        InterfaceDefStmt iface_def;

        REQUIRE( is_a<UserTypeDefStmt>(&class_def) );
        REQUIRE( is_a<UserTypeDefStmt>(&iface_def) );
        REQUIRE( is_a<Stmt>(&class_def) );
        REQUIRE( is_a<Stmt>(&iface_def) );

        REQUIRE_FALSE( is_a<InterfaceDefStmt>(&class_def) );
        REQUIRE_FALSE( is_a<ClassDefStmt>(&iface_def) );
    }

    SECTION( "scoped type hierarchy" ) {
        ClassType class_type;
        InterfaceType iface_type;
        IntType int_type;

        REQUIRE( is_a<ScopedType>(&class_type) );
        REQUIRE( is_a<ScopedType>(&iface_type) );
        REQUIRE( is_a<Type>(&class_type) );
        REQUIRE( is_a<ASTNode>(&class_type) );

        REQUIRE_FALSE( is_a<InterfaceType>(&class_type) );
        REQUIRE_FALSE( is_a<ClassType>(&iface_type) );
        REQUIRE_FALSE( is_a<ScopedType>(&int_type) );
        REQUIRE( is_a<Type>(&int_type) );
    }

    SECTION( "similarly named types from different branches are unrelated" ) {
        ClassType class_type;
        ClassDefStmt class_def;
        ClassRefExpr class_ref;

        REQUIRE_FALSE( is_a<ClassDefStmt>(&class_type) );
        REQUIRE_FALSE( is_a<ClassRefExpr>(&class_type) );
        REQUIRE_FALSE( is_a<ClassType>(&class_def) );
        REQUIRE_FALSE( is_a<ClassRefExpr>(&class_def) );
        REQUIRE_FALSE( is_a<ClassType>(&class_ref) );
        REQUIRE_FALSE( is_a<ClassDefStmt>(&class_ref) );
    }

    SECTION( "uses the real type, not the static pointer type" ) {
        IntLiteralExpr node;
        Expr *expr_ptr = &node;
        REQUIRE( is_a<LiteralExpr>(expr_ptr) );
        REQUIRE( is_a<IntLiteralExpr>(expr_ptr) );
        REQUIRE_FALSE( is_a<FloatLiteralExpr>(expr_ptr) );
    }
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a` is reflexive and true for ASTNode",
    "[type-info]",
    AllNodes
) {
    TestType node;
    ASTNode *p = &node;

    REQUIRE( is_a<TestType>(p) );
    REQUIRE( is_a<ASTNode>(p) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<Expr>` holds for every expression and only for them",
    "[type-info]",
    ExprNodes
) {
    TestType node;
    ASTNode *p = &node;

    REQUIRE( is_a<Expr>(p) );
    REQUIRE_FALSE( is_a<Stmt>(p) );
    REQUIRE_FALSE( is_a<Type>(p) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<Stmt>` holds for every statement and only for them",
    "[type-info]",
    StmtNodes
) {
    TestType node;
    ASTNode *p = &node;

    REQUIRE( is_a<Stmt>(p) );
    REQUIRE_FALSE( is_a<Expr>(p) );
    REQUIRE_FALSE( is_a<Type>(p) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<Type>` holds for every type and only for them",
    "[type-info]",
    TypeNodes
) {
    TestType node;
    ASTNode *p = &node;

    REQUIRE( is_a<Type>(p) );
    REQUIRE_FALSE( is_a<Expr>(p) );
    REQUIRE_FALSE( is_a<Stmt>(p) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<LiteralExpr>` holds for every literal",
    "[type-info]",
    LiteralExprs
) {
    TestType node;
    REQUIRE( is_a<LiteralExpr>(&node) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<LiteralExpr>` does not hold for non-literal expressions",
    "[type-info]",
    NonLiteralExprs
) {
    TestType node;
    REQUIRE_FALSE( is_a<LiteralExpr>(&node) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<VarDefStmt>` holds for every variable definition",
    "[type-info]",
    VarDefStmts
) {
    TestType node;
    REQUIRE( is_a<VarDefStmt>(&node) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<LabelStmt>` holds for every label",
    "[type-info]",
    LabelStmts
) {
    TestType node;
    REQUIRE( is_a<LabelStmt>(&node) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<UserTypeDefStmt>` holds for class and interface definitions",
    "[type-info]",
    UserTypeDefStmts
) {
    TestType node;
    REQUIRE( is_a<UserTypeDefStmt>(&node) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<ScopedType>` holds for class and interface types",
    "[type-info]",
    ScopedTypes
) {
    TestType node;
    REQUIRE( is_a<ScopedType>(&node) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a<ScopedType>` does not hold for other types",
    "[type-info]",
    OtherTypes
) {
    TestType node;
    REQUIRE_FALSE( is_a<ScopedType>(&node) );
}


TEMPLATE_LIST_TEST_CASE(
    "`is_a` of intermediate bases is false for unrelated statements",
    "[type-info]",
    OtherStmts
) {
    TestType node;
    ASTNode *p = &node;

    REQUIRE_FALSE( is_a<VarDefStmt>(p) );
    REQUIRE_FALSE( is_a<LabelStmt>(p) );
    REQUIRE_FALSE( is_a<UserTypeDefStmt>(p) );
}
