#ifndef LIBASTFRI_TEXT_ASTFRI_VISITOR
#define LIBASTFRI_TEXT_ASTFRI_VISITOR

#include <libastfri-text/inc/pseudocode/Exporter.hpp>
#include <libastfri/inc/Astfri.hpp>

namespace astfri::text
{
    template<typename AstfriNode>
    concept astfri_node =
        requires(AstfriNode& node)
        {
            {
                node.accept(std::declval<IVisitor&>())
            } -> std::same_as<void>;
        };

    // -----

    template<typename VectorAstfriNode>
    concept vect_param_arg =
        std::same_as<VectorAstfriNode, std::vector<ParamVarDefStmt*>> ||
        std::same_as<VectorAstfriNode, std::vector<GenericParam*>> ||
        std::same_as<VectorAstfriNode, std::vector<Expr*>>;

    // -----

    template<typename VectorAstfriNode>
    concept vect_member =
        std::same_as<VectorAstfriNode, std::vector<MemberVarDefStmt*>> ||
        std::same_as<VectorAstfriNode, std::vector<ConstructorDefStmt*>> ||
        std::same_as<VectorAstfriNode, std::vector<MethodDefStmt*>>;

    // -----

    class AstfriVisitor : public virtual IVisitor
    {
    protected:
        Exporter* exporter_;
    protected:
        AstfriVisitor(Exporter*& exp);
        virtual ~AstfriVisitor() = default;
    public:
        void set_exporter(Exporter*& exp);
    protected:
        void write_comma_space(bool isNotLast);
        // -----
        template<astfri_node AstfriNode>
        void check_and_accept_pointer_w_error(AstfriNode* pointer);
        // -----
        template<astfri_node AstfriNode>
        void check_and_accept_pointer(AstfriNode* pointer);
        // -----
        template<vect_param_arg VectorAstfriNode>
        void write_params_or_args(VectorAstfriNode const vectorPA, bool isGeneric = false);
        // -----
        template<vect_member VectorAstfriNode>
        bool has_acc_mod(VectorAstfriNode const& vectorStmts, VectorAstfriNode& vectorStmtsTmp, AccessModifier accmod);
    };

    // -----

    template<astfri_node AstfriNode>
    void AstfriVisitor::check_and_accept_pointer_w_error(AstfriNode* pointer)
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

    template<astfri_node AstfriNode>
    void AstfriVisitor::check_and_accept_pointer(AstfriNode* pointer)
    {
        if (pointer)
        {
            pointer->accept(*this);
        }
    }

    template<vect_param_arg VectorAstfriNode>
    void AstfriVisitor::write_params_or_args(VectorAstfriNode const vectorPA, bool isGeneric)
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

    template<vect_member VectorAstfriNode>
    bool AstfriVisitor::has_acc_mod(VectorAstfriNode const& vectorStmts, VectorAstfriNode& vectorStmtsTmp, AccessModifier accmod)
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