#ifndef ASTFRI_IMPL_VISITOR_HPP
#define ASTFRI_IMPL_VISITOR_HPP

#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/StmtFwd.hpp>
#include <astfri/impl/TypeFwd.hpp>

#include <stdexcept>

namespace astfri {

/**
 * @brief TODO
 */
struct Visitor {
    virtual void visit(const DynamicType& type)           = 0;
    virtual void visit(const IntType& type)               = 0;
    virtual void visit(const FloatType& type)             = 0;
    virtual void visit(const CharType& type)              = 0;
    virtual void visit(const BoolType& type)              = 0;
    virtual void visit(const VoidType& type)              = 0;
    virtual void visit(const IndirectionType& type)       = 0;
    virtual void visit(const ClassType& type)             = 0;
    virtual void visit(const InterfaceType& type)         = 0;
    virtual void visit(const LambdaType& type)            = 0;
    virtual void visit(const IncompleteType& type)        = 0;
    virtual void visit(const DeducedType& type)           = 0;
    virtual void visit(const UnknownType& type)           = 0;

    virtual void visit(const IntLiteralExpr& expr)        = 0;
    virtual void visit(const FloatLiteralExpr& expr)      = 0;
    virtual void visit(const CharLiteralExpr& expr)       = 0;
    virtual void visit(const StringLiteralExpr& expr)     = 0;
    virtual void visit(const BoolLiteralExpr& expr)       = 0;
    virtual void visit(const NullLiteralExpr& expr)       = 0;
    virtual void visit(const IfExpr& expr)                = 0;
    virtual void visit(const BinOpExpr& expr)             = 0;
    virtual void visit(const UnaryOpExpr& expr)           = 0;
    virtual void visit(const ParamVarRefExpr& expr)       = 0;
    virtual void visit(const LocalVarRefExpr& expr)       = 0;
    virtual void visit(const MemberVarRefExpr& expr)      = 0;
    virtual void visit(const GlobalVarRefExpr& expr)      = 0;
    virtual void visit(const ClassRefExpr& expr)          = 0;
    virtual void visit(const FunctionCallExpr& expr)      = 0;
    virtual void visit(const MethodCallExpr& expr)        = 0;
    virtual void visit(const LambdaCallExpr& expr)        = 0;
    virtual void visit(const LambdaExpr& expr)            = 0;
    virtual void visit(const ThisExpr& expr)              = 0;
    virtual void visit(const ConstructorCallExpr& expr)   = 0;
    virtual void visit(const NewExpr& expr)               = 0;
    virtual void visit(const DeleteExpr& expr)            = 0;
    virtual void visit(const BracketExpr& expr)           = 0;
    virtual void visit(const UnknownExpr& expr)           = 0;

    virtual void visit(const TranslationUnit& stmt)       = 0;
    virtual void visit(const CompoundStmt& stmt)          = 0;
    virtual void visit(const ReturnStmt& stmt)            = 0;
    virtual void visit(const ExprStmt& stmt)              = 0;
    virtual void visit(const IfStmt& stmt)                = 0;
    virtual void visit(const CaseStmt& stmt)              = 0;
    virtual void visit(const DefaultCaseStmt& stmt)       = 0;
    virtual void visit(const SwitchStmt& stmt)            = 0;
    virtual void visit(const WhileStmt& stmt)             = 0;
    virtual void visit(const DoWhileStmt& stmt)           = 0;
    virtual void visit(const ForStmt& stmt)               = 0;
    virtual void visit(const ForEachStmt& stmt)           = 0;
    virtual void visit(const ThrowStmt& stmt)             = 0;
    virtual void visit(const CatchStmt& stmt)             = 0;
    virtual void visit(const TryStmt& stmt)               = 0;
    virtual void visit(const UnknownStmt& stmt)           = 0;
    virtual void visit(const LocalVarDefStmt& stmt)       = 0;
    virtual void visit(const ParamVarDefStmt& stmt)       = 0;
    virtual void visit(const MemberVarDefStmt& stmt)      = 0;
    virtual void visit(const GlobalVarDefStmt& stmt)      = 0;
    virtual void visit(const FunctionDefStmt& stmt)       = 0;
    virtual void visit(const DefStmt& stmt)               = 0;
    virtual void visit(const MethodDefStmt& stmt)         = 0;
    virtual void visit(const BaseInitializerStmt& stmt)   = 0;
    virtual void visit(const SelfInitializerStmt& stmt)   = 0;
    virtual void visit(const MemberInitializerStmt& stmt) = 0;
    virtual void visit(const ConstructorDefStmt& stmt)    = 0;
    virtual void visit(const DestructorDefStmt& stmt)     = 0;
    virtual void visit(const GenericParam& stmt)          = 0;
    virtual void visit(const InterfaceDefStmt& stmt)      = 0;
    virtual void visit(const ClassDefStmt& stmt)          = 0;
    virtual void visit(const ContinueStmt& stmt)          = 0;
    virtual void visit(const BreakStmt& stmt)             = 0;

    virtual ~Visitor()                                    = default;
};

/**
 * @brief TODO
 */
struct Visitable {
    virtual void accept(Visitor& visitor) = 0;
    virtual ~Visitable()                  = default;
};

/**
 * @brief TODO
 */
struct VisitorAdapter : Visitor {
    void visit(const DynamicType& /*type*/) override {
    }

    void visit(const IntType& /*type*/) override {
    }

    void visit(const FloatType& /*type*/) override {
    }

    void visit(const CharType& /*type*/) override {
    }

    void visit(const BoolType& /*type*/) override {
    }

    void visit(const VoidType& /*type*/) override {
    }

    void visit(const IndirectionType& /*type*/) override {
    }

    void visit(const ClassType& /*type*/) override {
    }

    void visit(const InterfaceType& /*type*/) override {
    }

    void visit(const LambdaType& /*type*/) override {
    }

    void visit(const IncompleteType& /*type*/) override {
    }

    void visit(const DeducedType& /*type*/) override {
    }

    void visit(const UnknownType& /*type*/) override {
    }

    void visit(const IntLiteralExpr& /*expr*/) override {
    }

    void visit(const FloatLiteralExpr& /*expr*/) override {
    }

    void visit(const CharLiteralExpr& /*expr*/) override {
    }

    void visit(const StringLiteralExpr& /*expr*/) override {
    }

    void visit(const BoolLiteralExpr& /*expr*/) override {
    }

    void visit(const NullLiteralExpr& /*expr*/) override {
    }

    void visit(const IfExpr& /*expr*/) override {
    }

    void visit(const BinOpExpr& /*expr*/) override {
    }

    void visit(const UnaryOpExpr& /*expr*/) override {
    }

    void visit(const ParamVarRefExpr& /*expr*/) override {
    }

    void visit(const LocalVarRefExpr& /*expr*/) override {
    }

    void visit(const MemberVarRefExpr& /*expr*/) override {
    }

    void visit(const GlobalVarRefExpr& /*expr*/) override {
    }

    void visit(const ClassRefExpr& /*expr*/) override {
    }

    void visit(const FunctionCallExpr& /*expr*/) override {
    }

    void visit(const MethodCallExpr& /*expr*/) override {
    }

    void visit(const LambdaCallExpr& /*expr*/) override {
    }

    void visit(const LambdaExpr& /*expr*/) override {
    }

    void visit(const ThisExpr& /*expr*/) override {
    }

    void visit(const ConstructorCallExpr& /*expr*/) override {
    }

    void visit(const NewExpr& /*expr*/) override {
    }

    void visit(const DeleteExpr& /*expr*/) override {
    }

    void visit(const BracketExpr& /*expr*/) override {
    }

    void visit(const UnknownExpr& /*expr*/) override {
    }

    void visit(const TranslationUnit& /*stmt*/) override {
    }

    void visit(const CompoundStmt& /*stmt*/) override {
    }

    void visit(const ReturnStmt& /*stmt*/) override {
    }

    void visit(const ExprStmt& /*stmt*/) override {
    }

    void visit(const IfStmt& /*stmt*/) override {
    }

    void visit(const CaseStmt& /*stmt*/) override {
    }

    void visit(const DefaultCaseStmt& /*stmt*/) override {
    }

    void visit(const SwitchStmt& /*stmt*/) override {
    }

    void visit(const WhileStmt& /*stmt*/) override {
    }

    void visit(const DoWhileStmt& /*stmt*/) override {
    }

    void visit(const ForStmt& /*stmt*/) override {
    }

    void visit(const ForEachStmt& /*stmt*/) override {
    }

    void visit(const ThrowStmt& /*stmt*/) override {
    }

    void visit(const CatchStmt& /*stmt*/) override {
    }

    void visit(const TryStmt& /*stmt*/) override {
    }

    void visit(const UnknownStmt& /*stmt*/) override {
    }

    void visit(const LocalVarDefStmt& /*stmt*/) override {
    }

    void visit(const ParamVarDefStmt& /*stmt*/) override {
    }

    void visit(const MemberVarDefStmt& /*stmt*/) override {
    }

    void visit(const GlobalVarDefStmt& /*stmt*/) override {
    }

    void visit(const FunctionDefStmt& /*stmt*/) override {
    }

    void visit(const DefStmt& /*stmt*/) override {
    }

    void visit(const MethodDefStmt& /*stmt*/) override {
    }

    void visit(const BaseInitializerStmt& /*stmt*/) override {
    }

    void visit(const SelfInitializerStmt& /*stmt*/) override {
    }

    void visit(const MemberInitializerStmt& /*stmt*/) override {
    }

    void visit(const ConstructorDefStmt& /*stmt*/) override {
    }

    void visit(const DestructorDefStmt& /*stmt*/) override {
    }

    void visit(const GenericParam& /*stmt*/) override {
    }

    void visit(const InterfaceDefStmt& /*stmt*/) override {
    }

    void visit(const ClassDefStmt& /*stmt*/) override {
    }

    void visit(const ContinueStmt& /*stmt*/) override {
    }

    void visit(const BreakStmt& /*stmt*/) override {
    }
};

/**
 * @brief TODO
 */
struct ThrowingVisitorAdapter : Visitor {
    void visit(const DynamicType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const IntType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const FloatType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const CharType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const BoolType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const VoidType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const IndirectionType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ClassType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const InterfaceType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const LambdaType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const IncompleteType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const DeducedType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const UnknownType& /*type*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const IntLiteralExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const FloatLiteralExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const CharLiteralExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const StringLiteralExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const BoolLiteralExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const NullLiteralExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const IfExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const BinOpExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const UnaryOpExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ParamVarRefExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const LocalVarRefExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const MemberVarRefExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const GlobalVarRefExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ClassRefExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const FunctionCallExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const MethodCallExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const LambdaCallExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const LambdaExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ThisExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ConstructorCallExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const NewExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const DeleteExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const BracketExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const UnknownExpr& /*expr*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const TranslationUnit& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const CompoundStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ReturnStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ExprStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const IfStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const CaseStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const DefaultCaseStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const SwitchStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const WhileStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const DoWhileStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ForStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ForEachStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ThrowStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const CatchStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const TryStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const UnknownStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const LocalVarDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ParamVarDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const MemberVarDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const GlobalVarDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const FunctionDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const DefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const MethodDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const BaseInitializerStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const SelfInitializerStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const MemberInitializerStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ConstructorDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const DestructorDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const GenericParam& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const InterfaceDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ClassDefStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const ContinueStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }

    void visit(const BreakStmt& /*stmt*/) override {
        throw std::logic_error("Not Implemented Yet!");
    }
};

namespace details {

/**
 * @brief CRTP mixin that makes @c This child class visitable.
 * @tparam This Type of the child class.
 */
template<typename This>
struct MkVisitable : virtual Visitable {
    void accept(Visitor& visitor) override {
        visitor.visit(static_cast<const This&>(*this));
    }
};

} // namespace details

} // namespace astfri

#endif
