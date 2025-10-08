#ifndef LIBASTFRI_TEXT_GENERAL_AST_VISITOR
#define LIBASTFRI_TEXT_GENERAL_AST_VISITOR

#include <libastfri-text/inc/Exporter.hpp>
#include <libastfri/inc/Stmt.hpp>

namespace astfri::text
{
    template <typename ASTFRI_PEAK>
    concept Acceptable_ASTFRI_Peak = requires(ASTFRI_PEAK& peak)
    {
        { peak.accept(std::declval<IVisitor&>()) };
    };

    template <typename VParamsOrArgs>
    concept Vector_of_Params_or_Args =
        std::same_as<VParamsOrArgs, std::vector<ParamVarDefStmt*>> ||
        std::same_as<VParamsOrArgs, std::vector<GenericParam*>> ||
        //std::same_as<VParamOrArg, std::vector<GenericArg*>> ||
        std::same_as<VParamsOrArgs, std::vector<Expr*>>;

    template <typename VMembers>
    concept Vector_of_Members =
        std::same_as<VMembers, std::vector<MemberVarDefStmt*>> ||
        std::same_as<VMembers, std::vector<ConstructorDefStmt*>> ||
        std::same_as<VMembers, std::vector<MethodDefStmt*>>;

    class GeneralASTVisitor : public virtual IVisitor
    {
    protected:
        Exporter* exporter_;
    public:
        inline GeneralASTVisitor(Exporter*& exp) : exporter_(exp) {}
        virtual ~GeneralASTVisitor() = 0;
        inline void set_exporter(Exporter*& exp) { exporter_ = exp; }
    protected:
        void write_comma_space(bool isNotLast);
        template <Acceptable_ASTFRI_Peak ASTFRI_PEAK>
        void check_and_accept_pointer_w_error(ASTFRI_PEAK* pointer);
        template <Acceptable_ASTFRI_Peak ASTFRI_PEAK>
        void check_and_accept_pointer(ASTFRI_PEAK* pointer);
        template <Vector_of_Params_or_Args VParamsOrArgs>
        void write_params_or_args(const VParamsOrArgs& vectorPA, bool isGeneric = false);
        template <Vector_of_Members VMembers>
        bool has_acc_mod(const VMembers& vectorStmts, VMembers& vectorStmtsTmp, AccessModifier accmod);
    };

    inline GeneralASTVisitor::~GeneralASTVisitor() = default;

    inline void GeneralASTVisitor::write_comma_space(bool isNotLast)
    {
        if (isNotLast)
        {
            exporter_->write_separator_sign(",");
            exporter_->write_space();
        }
    }

    template <Acceptable_ASTFRI_Peak ASTFRI_PEAK>
    inline void GeneralASTVisitor::check_and_accept_pointer_w_error(ASTFRI_PEAK* pointer)
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

    template <Acceptable_ASTFRI_Peak ASTFRI_PEAK>
    inline void GeneralASTVisitor::check_and_accept_pointer(ASTFRI_PEAK* pointer)
    {
        if (pointer)
        {
            pointer->accept(*this);
        }
    }

    template <Vector_of_Params_or_Args VParamsOrArgs>
    void GeneralASTVisitor::write_params_or_args(const VParamsOrArgs& vectorPA, bool isGeneric)
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

    template <Vector_of_Members VMembers>
    bool GeneralASTVisitor::has_acc_mod(const VMembers& vectorStmts, VMembers& vectorStmtsTmp, AccessModifier accmod)
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