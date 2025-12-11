#ifndef LIBASTFRI_VISITOR_HPP
#define LIBASTFRI_VISITOR_HPP

#include <libastfri/impl/ExprFwd.hpp>
#include <libastfri/impl/StmtFwd.hpp>
#include <libastfri/impl/TypeFwd.hpp>

#include <stdexcept>

namespace astfri
{

/**
 * @brief TODO
 */
struct IVisitor
{
    virtual void visit(DynamicType const& type)         = 0;
    virtual void visit(IntType const& type)             = 0;
    virtual void visit(FloatType const& type)           = 0;
    virtual void visit(CharType const& type)            = 0;
    virtual void visit(BoolType const& type)            = 0;
    virtual void visit(VoidType const& type)            = 0;
    virtual void visit(IndirectionType const& type)     = 0;
    virtual void visit(ClassType const& type)           = 0;
    virtual void visit(InterfaceType const& type)       = 0;
    virtual void visit(LambdaType const& type)          = 0;
    virtual void visit(IncompleteType const& type)      = 0;
    virtual void visit(UnknownType const& type)         = 0;

    virtual void visit(IntLiteralExpr const& expr)      = 0;
    virtual void visit(FloatLiteralExpr const& expr)    = 0;
    virtual void visit(CharLiteralExpr const& expr)     = 0;
    virtual void visit(StringLiteralExpr const& expr)   = 0;
    virtual void visit(BoolLiteralExpr const& expr)     = 0;
    virtual void visit(NullLiteralExpr const& expr)     = 0;
    virtual void visit(IfExpr const& expr)              = 0;
    virtual void visit(BinOpExpr const& expr)           = 0;
    virtual void visit(UnaryOpExpr const& expr)         = 0;
    virtual void visit(ParamVarRefExpr const& expr)     = 0;
    virtual void visit(LocalVarRefExpr const& expr)     = 0;
    virtual void visit(MemberVarRefExpr const& expr)    = 0;
    virtual void visit(GlobalVarRefExpr const& expr)    = 0;
    virtual void visit(ClassRefExpr const& expr)        = 0;
    virtual void visit(FunctionCallExpr const& expr)    = 0;
    virtual void visit(MethodCallExpr const& expr)      = 0;
    virtual void visit(LambdaCallExpr const& expr)      = 0;
    virtual void visit(LambdaExpr const& expr)          = 0;
    virtual void visit(ThisExpr const& expr)            = 0;
    virtual void visit(ConstructorCallExpr const& expr) = 0;
    virtual void visit(NewExpr const& expr)             = 0;
    virtual void visit(DeleteExpr const& expr)          = 0;
    virtual void visit(BracketExpr const& expr)         = 0;
    virtual void visit(UnknownExpr const& expr)         = 0;

    virtual void visit(TranslationUnit const& stmt)     = 0;
    virtual void visit(CompoundStmt const& stmt)        = 0;
    virtual void visit(ReturnStmt const& stmt)          = 0;
    virtual void visit(ExprStmt const& stmt)            = 0;
    virtual void visit(IfStmt const& stmt)              = 0;
    virtual void visit(CaseStmt const& stmt)            = 0;
    virtual void visit(DefaultCaseStmt const& stmt)     = 0;
    virtual void visit(SwitchStmt const& stmt)          = 0;
    virtual void visit(WhileStmt const& stmt)           = 0;
    virtual void visit(DoWhileStmt const& stmt)         = 0;
    virtual void visit(ForStmt const& stmt)             = 0;
    virtual void visit(ForEachStmt const& stmt)         = 0;
    virtual void visit(ThrowStmt const& stmt)           = 0;
    virtual void visit(CatchStmt const& stmt)           = 0;
    virtual void visit(TryStmt const& stmt)             = 0;
    virtual void visit(UnknownStmt const& stmt)         = 0;
    virtual void visit(LocalVarDefStmt const& stmt)     = 0;
    virtual void visit(ParamVarDefStmt const& stmt)     = 0;
    virtual void visit(MemberVarDefStmt const& stmt)    = 0;
    virtual void visit(GlobalVarDefStmt const& stmt)    = 0;
    virtual void visit(FunctionDefStmt const& stmt)     = 0;
    virtual void visit(DefStmt const& stmt)             = 0;
    virtual void visit(MethodDefStmt const& stmt)       = 0;
    virtual void visit(BaseInitializerStmt const& stmt) = 0;
    virtual void visit(SelfInitializerStmt const& stmt) = 0;
    virtual void visit(MemberInitializerStmt const& stmt) = 0;
    virtual void visit(ConstructorDefStmt const& stmt)  = 0;
    virtual void visit(DestructorDefStmt const& stmt)   = 0;
    virtual void visit(GenericParam const& stmt)        = 0;
    virtual void visit(InterfaceDefStmt const& stmt)    = 0;
    virtual void visit(ClassDefStmt const& stmt)        = 0;
    virtual void visit(ContinueStmt const& stmt)        = 0;
    virtual void visit(BreakStmt const& stmt)           = 0;

    virtual ~IVisitor()                                 = default;
};

/**
 * @brief TODO
 */
struct IVisitable
{
    virtual void accept(IVisitor& visitor) = 0;
    virtual ~IVisitable()                  = default;
};

/**
 * @brief TODO
 */
struct VisitorAdapter : IVisitor
{
    void visit(DynamicType const& /*type*/) override
    {
    }

    void visit(IntType const& /*type*/) override
    {
    }

    void visit(FloatType const& /*type*/) override
    {
    }

    void visit(CharType const& /*type*/) override
    {
    }

    void visit(BoolType const& /*type*/) override
    {
    }

    void visit(VoidType const& /*type*/) override
    {
    }

    void visit(IndirectionType const& /*type*/) override
    {
    }

    void visit(ClassType const& /*type*/) override
    {
    }

    void visit(InterfaceType const& /*type*/) override
    {
    }

    void visit(LambdaType const& /*type*/) override
    {
    }

    void visit(IncompleteType const& /*type*/) override
    {
    }

    void visit(UnknownType const& /*type*/) override
    {
    }

    void visit(IntLiteralExpr const& /*expr*/) override
    {
    }

    void visit(FloatLiteralExpr const& /*expr*/) override
    {
    }

    void visit(CharLiteralExpr const& /*expr*/) override
    {
    }

    void visit(StringLiteralExpr const& /*expr*/) override
    {
    }

    void visit(BoolLiteralExpr const& /*expr*/) override
    {
    }

    void visit(NullLiteralExpr const& /*expr*/) override
    {
    }

    void visit(IfExpr const& /*expr*/) override
    {
    }

    void visit(BinOpExpr const& /*expr*/) override
    {
    }

    void visit(UnaryOpExpr const& /*expr*/) override
    {
    }

    void visit(ParamVarRefExpr const& /*expr*/) override
    {
    }

    void visit(LocalVarRefExpr const& /*expr*/) override
    {
    }

    void visit(MemberVarRefExpr const& /*expr*/) override
    {
    }

    void visit(GlobalVarRefExpr const& /*expr*/) override
    {
    }

    void visit(ClassRefExpr const& /*expr*/) override
    {
    }

    void visit(FunctionCallExpr const& /*expr*/) override
    {
    }

    void visit(MethodCallExpr const& /*expr*/) override
    {
    }

    void visit(LambdaCallExpr const& /*expr*/) override
    {
    }

    void visit(LambdaExpr const& /*expr*/) override
    {
    }

    void visit(ThisExpr const& /*expr*/) override
    {
    }

    void visit(ConstructorCallExpr const& /*expr*/) override
    {
    }

    void visit(NewExpr const& /*expr*/) override
    {
    }

    void visit(DeleteExpr const& /*expr*/) override
    {
    }

    void visit(BracketExpr const& /*expr*/) override
    {
    }

    void visit(UnknownExpr const& /*expr*/) override
    {
    }

    void visit(TranslationUnit const& /*stmt*/) override
    {
    }

    void visit(CompoundStmt const& /*stmt*/) override
    {
    }

    void visit(ReturnStmt const& /*stmt*/) override
    {
    }

    void visit(ExprStmt const& /*stmt*/) override
    {
    }

    void visit(IfStmt const& /*stmt*/) override
    {
    }

    void visit(CaseStmt const& /*stmt*/) override
    {
    }

    void visit(DefaultCaseStmt const& /*stmt*/) override
    {
    }

    void visit(SwitchStmt const& /*stmt*/) override
    {
    }

    void visit(WhileStmt const& /*stmt*/) override
    {
    }

    void visit(DoWhileStmt const& /*stmt*/) override
    {
    }

    void visit(ForStmt const& /*stmt*/) override
    {
    }

    void visit(ForEachStmt const& /*stmt*/) override
    {
    }

    void visit(ThrowStmt const& /*stmt*/) override
    {
    }

    void visit(CatchStmt const& /*stmt*/) override
    {
    }

    void visit(TryStmt const& /*stmt*/) override
    {
    }

    void visit(UnknownStmt const& /*stmt*/) override
    {
    }

    void visit(LocalVarDefStmt const& /*stmt*/) override
    {
    }

    void visit(ParamVarDefStmt const& /*stmt*/) override
    {
    }

    void visit(MemberVarDefStmt const& /*stmt*/) override
    {
    }

    void visit(GlobalVarDefStmt const& /*stmt*/) override
    {
    }

    void visit(FunctionDefStmt const& /*stmt*/) override
    {
    }

    void visit(DefStmt const& /*stmt*/) override
    {
    }

    void visit(MethodDefStmt const& /*stmt*/) override
    {
    }

    void visit(BaseInitializerStmt const& /*stmt*/) override
    {
    }

    void visit(SelfInitializerStmt const& /*stmt*/) override
    {
    }

    void visit(MemberInitializerStmt const& /*stmt*/) override
    {
    }

    void visit(ConstructorDefStmt const& /*stmt*/) override
    {
    }

    void visit(DestructorDefStmt const& /*stmt*/) override
    {
    }

    void visit(GenericParam const& /*stmt*/) override
    {
    }

    void visit(InterfaceDefStmt const& /*stmt*/) override
    {
    }

    void visit(ClassDefStmt const& /*stmt*/) override
    {
    }

    void visit(ContinueStmt const& /*stmt*/) override
    {
    }

    void visit(BreakStmt const& /*stmt*/) override
    {
    }
};


/**
 * @brief TODO
 */
struct ThrowingVisitorAdapter : IVisitor
{
    void visit(DynamicType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(IntType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(FloatType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(CharType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(BoolType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(VoidType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(IndirectionType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ClassType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(InterfaceType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(LambdaType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(IncompleteType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(UnknownType const& /*type*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(IntLiteralExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(FloatLiteralExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(CharLiteralExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(StringLiteralExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(BoolLiteralExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(NullLiteralExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(IfExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(BinOpExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(UnaryOpExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ParamVarRefExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(LocalVarRefExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(MemberVarRefExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(GlobalVarRefExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ClassRefExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(FunctionCallExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(MethodCallExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(LambdaCallExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(LambdaExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ThisExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ConstructorCallExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(NewExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(DeleteExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(BracketExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(UnknownExpr const& /*expr*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(TranslationUnit const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(CompoundStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ReturnStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ExprStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(IfStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(CaseStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(DefaultCaseStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(SwitchStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(WhileStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(DoWhileStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ForStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ForEachStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ThrowStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(CatchStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(TryStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(UnknownStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(LocalVarDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ParamVarDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(MemberVarDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(GlobalVarDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(FunctionDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(DefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(MethodDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(BaseInitializerStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(SelfInitializerStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(MemberInitializerStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ConstructorDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(DestructorDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(GenericParam const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(InterfaceDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ClassDefStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(ContinueStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(BreakStmt const& /*stmt*/) override
    {
        throw std::logic_error("Not Implemented Yet!");
    }
};


namespace details
{

/**
 * @brief CRTP mixin that makes @c This child class visitable.
 * @tparam This Type of the child class.
 */
template<typename This>
struct MkVisitable : virtual IVisitable
{
    void accept(IVisitor& visitor) override
    {
        visitor.visit(static_cast<This const&>(*this));
    }
};

} // namespace details

} // namespace astfri

#endif
