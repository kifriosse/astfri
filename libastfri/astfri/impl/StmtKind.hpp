#ifndef ASTFRI_IMPL_STMT_KIND_HPP
#define ASTFRI_IMPL_STMT_KIND_HPP

#include <astfri/impl/Kind.hpp>
#include <astfri/impl/StmtFwd.hpp>


namespace astfri {


enum class StmtKind {
    UNINITIALIZED = 0,
    LocalVarDef,
    ParamVarDef,
    MemberVarDef,
    GlobalVarDef,
    MultiLocalVarDef,
    FunctionDef,
    MethodDef,
    BaseInitializer,
    SelfInitializer,
    MemberInitializer,
    ConstructorDef,
    DestructorDef,
    GenericP,
    InterfaceDef,
    ClassDef,
    Compound,
    Return,
    Expr,
    If,
    Case,
    DefaultCase,
    Switch,
    Loop,
    While,
    DoWhile,
    For,
    ForEach,
    Throw,
    Catch,
    Try,
    Continue,
    Break,
    Unknown,
    TranslationUnit,
};


template<> struct KindOf<LocalVarDefStmt> {
    static constexpr StmtKind value{StmtKind::LocalVarDef};
};

template<> struct KindOf<ParamVarDefStmt> {
    static constexpr StmtKind value{StmtKind::ParamVarDef};
};

template<> struct KindOf<MemberVarDefStmt> {
    static constexpr StmtKind value{StmtKind::MemberVarDef};
};

template<> struct KindOf<GlobalVarDefStmt> {
    static constexpr StmtKind value{StmtKind::GlobalVarDef};
};

template<> struct KindOf<MultiLocalVarDefStmt> {
    static constexpr StmtKind value{StmtKind::MultiLocalVarDef};
};

template<> struct KindOf<FunctionDefStmt> {
    static constexpr StmtKind value{StmtKind::FunctionDef};
};

template<> struct KindOf<MethodDefStmt> {
    static constexpr StmtKind value{StmtKind::MethodDef};
};

template<> struct KindOf<BaseInitializerStmt> {
    static constexpr StmtKind value{StmtKind::BaseInitializer};
};

template<> struct KindOf<SelfInitializerStmt> {
    static constexpr StmtKind value{StmtKind::SelfInitializer};
};

template<> struct KindOf<MemberInitializerStmt> {
    static constexpr StmtKind value{StmtKind::MemberInitializer};
};

template<> struct KindOf<ConstructorDefStmt> {
    static constexpr StmtKind value{StmtKind::ConstructorDef};
};

template<> struct KindOf<DestructorDefStmt> {
    static constexpr StmtKind value{StmtKind::DestructorDef};
};

template<> struct KindOf<GenericParam> {
    static constexpr StmtKind value{StmtKind::GenericP};
};

template<> struct KindOf<InterfaceDefStmt> {
    static constexpr StmtKind value{StmtKind::InterfaceDef};
};

template<> struct KindOf<ClassDefStmt> {
    static constexpr StmtKind value{StmtKind::ClassDef};
};

template<> struct KindOf<CompoundStmt> {
    static constexpr StmtKind value{StmtKind::Compound};
};

template<> struct KindOf<ReturnStmt> {
    static constexpr StmtKind value{StmtKind::Return};
};

template<> struct KindOf<ExprStmt> {
    static constexpr StmtKind value{StmtKind::Expr};
};

template<> struct KindOf<IfStmt> {
    static constexpr StmtKind value{StmtKind::If};
};

template<> struct KindOf<CaseStmt> {
    static constexpr StmtKind value{StmtKind::Case};
};

template<> struct KindOf<DefaultCaseStmt> {
    static constexpr StmtKind value{StmtKind::DefaultCase};
};

template<> struct KindOf<SwitchStmt> {
    static constexpr StmtKind value{StmtKind::Switch};
};

template<> struct KindOf<LoopStmt> {
    static constexpr StmtKind value{StmtKind::Loop};
};

template<> struct KindOf<WhileStmt> {
    static constexpr StmtKind value{StmtKind::While};
};

template<> struct KindOf<DoWhileStmt> {
    static constexpr StmtKind value{StmtKind::DoWhile};
};

template<> struct KindOf<ForStmt> {
    static constexpr StmtKind value{StmtKind::For};
};

template<> struct KindOf<ForEachStmt> {
    static constexpr StmtKind value{StmtKind::ForEach};
};

template<> struct KindOf<ThrowStmt> {
    static constexpr StmtKind value{StmtKind::Throw};
};

template<> struct KindOf<CatchStmt> {
    static constexpr StmtKind value{StmtKind::Catch};
};

template<> struct KindOf<TryStmt> {
    static constexpr StmtKind value{StmtKind::Try};
};

template<> struct KindOf<ContinueStmt> {
    static constexpr StmtKind value{StmtKind::Continue};
};

template<> struct KindOf<BreakStmt> {
    static constexpr StmtKind value{StmtKind::Break};
};

template<> struct KindOf<UnknownStmt> {
    static constexpr StmtKind value{StmtKind::Unknown};
};

template<> struct KindOf<TranslationUnit> {
    static constexpr StmtKind value{StmtKind::TranslationUnit};
};


} // namespace astfri


#endif
