#ifndef ASTFRI_IMPL_EXPR_KIND_HPP
#define ASTFRI_IMPL_EXPR_KIND_HPP

#include <astfri/impl/Kind.hpp>
#include <astfri/impl/ExprFwd.hpp>


namespace astfri {


enum class ExprKind {
    UNINITIALIZED = 0,
    IntLiteral,
    FloatLiteral,
    CharLiteral,
    StringLiteral,
    BoolLiteral,
    NullLiteral,
    If,
    BinOp,
    UnaryOp,
    ParamVarRef,
    LocalVarRef,
    MemberVarRef,
    GlobalVarRef,
    ClassRef,
    FunctionCall,
    MethodCall,
    LambdaCall,
    Lambda,
    This,
    Base,
    ConstructorCall,
    New,
    Delete,
    Bracket,
};


template<> struct KindOf<IntLiteralExpr> {
    static constexpr ExprKind value{ExprKind::IntLiteral};
};

template<> struct KindOf<FloatLiteralExpr> {
    static constexpr ExprKind value{ExprKind::FloatLiteral};
};

template<> struct KindOf<CharLiteralExpr> {
    static constexpr ExprKind value{ExprKind::CharLiteral};
};

template<> struct KindOf<StringLiteralExpr> {
    static constexpr ExprKind value{ExprKind::StringLiteral};
};

template<> struct KindOf<BoolLiteralExpr> {
    static constexpr ExprKind value{ExprKind::BoolLiteral};
};

template<> struct KindOf<NullLiteralExpr> {
    static constexpr ExprKind value{ExprKind::NullLiteral};
};

template<> struct KindOf<IfExpr> {
    static constexpr ExprKind value{ExprKind::If};
};

template<> struct KindOf<BinOpExpr> {
    static constexpr ExprKind value{ExprKind::BinOp};
};

template<> struct KindOf<UnaryOpExpr> {
    static constexpr ExprKind value{ExprKind::UnaryOp};
};

template<> struct KindOf<ParamVarRefExpr> {
    static constexpr ExprKind value{ExprKind::ParamVarRef};
};

template<> struct KindOf<LocalVarRefExpr> {
    static constexpr ExprKind value{ExprKind::LocalVarRef};
};

template<> struct KindOf<MemberVarRefExpr> {
    static constexpr ExprKind value{ExprKind::MemberVarRef};
};

template<> struct KindOf<GlobalVarRefExpr> {
    static constexpr ExprKind value{ExprKind::GlobalVarRef};
};

template<> struct KindOf<ClassRefExpr> {
    static constexpr ExprKind value{ExprKind::ClassRef};
};

template<> struct KindOf<FunctionCallExpr> {
    static constexpr ExprKind value{ExprKind::FunctionCall};
};

template<> struct KindOf<MethodCallExpr> {
    static constexpr ExprKind value{ExprKind::MethodCall};
};

template<> struct KindOf<LambdaCallExpr> {
    static constexpr ExprKind value{ExprKind::LambdaCall};
};

template<> struct KindOf<LambdaExpr> {
    static constexpr ExprKind value{ExprKind::Lambda};
};

template<> struct KindOf<ThisExpr> {
    static constexpr ExprKind value{ExprKind::This};
};

template<> struct KindOf<BaseExpr> {
    static constexpr ExprKind value{ExprKind::Base};
};

template<> struct KindOf<ConstructorCallExpr> {
    static constexpr ExprKind value{ExprKind::ConstructorCall};
};

template<> struct KindOf<NewExpr> {
    static constexpr ExprKind value{ExprKind::New};
};

template<> struct KindOf<DeleteExpr> {
    static constexpr ExprKind value{ExprKind::Delete};
};

template<> struct KindOf<BracketExpr> {
    static constexpr ExprKind value{ExprKind::Bracket};
};


} // namespace astfri

#endif
