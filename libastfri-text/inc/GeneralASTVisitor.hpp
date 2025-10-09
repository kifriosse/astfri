#ifndef LIBASTFRI_TEXT_GENERAL_AST_VISITOR
#define LIBASTFRI_TEXT_GENERAL_AST_VISITOR

#include <libastfri-text/inc/Exporter.hpp>
#include <libastfri/inc/Stmt.hpp>

namespace astfri::text
{
    template<typename AstfriPeak>
    concept AcceptableAstfriPeak =
        requires(AstfriPeak& peak)
        {
            {
                peak.accept(std::declval<IVisitor&>())
            } -> std::same_as<void>;
        };

    // -----

    template<typename VectorAstfriPeak>
    concept VectorParamOrArg =
        std::same_as<VectorAstfriPeak, std::vector<ParamVarDefStmt*>> ||
        std::same_as<VectorAstfriPeak, std::vector<GenericParam*>> ||
        //std::same_as<VParamOrArg, std::vector<GenericArg*>> ||
        std::same_as<VectorAstfriPeak, std::vector<Expr*>>;

    // -----

    template<typename VectorAstfriPeak>
    concept VectorMember =
        std::same_as<VectorAstfriPeak, std::vector<MemberVarDefStmt*>> ||
        std::same_as<VectorAstfriPeak, std::vector<ConstructorDefStmt*>> ||
        std::same_as<VectorAstfriPeak, std::vector<MethodDefStmt*>>;

    // -----

    class GeneralASTVisitor : public virtual IVisitor
    {
    protected:
        Exporter* exporter_;
    protected:
        GeneralASTVisitor(Exporter*& exp);
        virtual ~GeneralASTVisitor() = default;
    public:
        void set_exporter(Exporter*& exp);
    protected:
        void write_comma_space(bool isNotLast);
        // -----
        template<AcceptableAstfriPeak AstfriPeak>
        void check_and_accept_pointer_w_error(AstfriPeak* pointer);
        // -----
        template<AcceptableAstfriPeak AstfriPeak>
        void check_and_accept_pointer(AstfriPeak* pointer);
        // -----
        template<VectorParamOrArg VectorAstfriPeak>
        void write_params_or_args(VectorAstfriPeak const vectorPA, bool isGeneric = false);
        // -----
        template<VectorMember VectorAstfriPeak>
        bool has_acc_mod(VectorAstfriPeak const& vectorStmts, VectorAstfriPeak& vectorStmtsTmp, AccessModifier accmod);
    };

    // -----

    template<AcceptableAstfriPeak AstfriPeak>
    void GeneralASTVisitor::check_and_accept_pointer_w_error(AstfriPeak* pointer)
    {
        if (pointer)
        {
            pointer->accept(*this);
        }
        else
        {
            exporter_->write_invalid_expr_word();
        }
    }

    template<AcceptableAstfriPeak AstfriPeak>
    void GeneralASTVisitor::check_and_accept_pointer(AstfriPeak* pointer)
    {
        if (pointer)
        {
            pointer->accept(*this);
        }
    }

    template<VectorParamOrArg VectorAstfriPeak>
    void GeneralASTVisitor::write_params_or_args(VectorAstfriPeak const vectorPA, bool isGeneric)
    {
        if (isGeneric)
        {
            exporter_->write_separator_sign("<");
        }
        else
        {
            exporter_->write_left_bracket("(");
        }
        for (size_t i = 0; i < vectorPA.size(); ++i)
        {
            check_and_accept_pointer_w_error(vectorPA.at(i));
            write_comma_space(i < vectorPA.size() - 1);
        }
        if (isGeneric)
        {
            exporter_->write_separator_sign(">");
        }
        else
        {
            exporter_->write_right_bracket(")");
        }
    }

    template<VectorMember VectorAstfriPeak>
    bool GeneralASTVisitor::has_acc_mod(VectorAstfriPeak const& vectorStmts, VectorAstfriPeak& vectorStmtsTmp, AccessModifier accmod)
    {
        bool hasAccMod = false;
        for (size_t i = 0; i < vectorStmts.size(); ++i)
        {
            if (vectorStmts.at(i)->access_ == accmod)
            {
                vectorStmtsTmp.push_back(vectorStmts.at(i));
                hasAccMod = true;
            }
        }
        return hasAccMod;
    }
}

#endif