#ifndef LIBASTFRI_TEXT_ABSTRACT_VISITOR
#define LIBASTFRI_TEXT_ABSTRACT_VISITOR

#include <libastfri/inc/Astfri.hpp>

#include <libastfri-text/inc/AbstractBuilder.hpp>

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

    //
    // -----
    //

    class AbstractVisitor : public IVisitor
    {
    protected:
        AbstractBuilder* builder_;
    public:
        AbstractVisitor() = delete;
        explicit AbstractVisitor(AbstractBuilder* const& builder);
        virtual ~AbstractVisitor() = default;
    protected:
        void write_condition(Expr* const& expr);
        // -----
        template<astfri_node AstfriNode>
        void check_and_accept_pointer_w_error(AstfriNode* const& pointer);
        // -----
        template<astfri_node AstfriNode>
        void check_and_accept_pointer(AstfriNode* const& pointer);
        // -----
        template<typename VectorAstfriNode>
        void write_params_or_args(VectorAstfriNode const vectorPA, bool isGeneric);
        // -----
        template<typename VectorAstfriNode>
        bool has_acc_mod(VectorAstfriNode const& vectorStmts, VectorAstfriNode& vectorStmtsTmp, AccessModifier accmod);
    };

    // -----

    template<astfri_node AstfriNode>
    void AbstractVisitor::check_and_accept_pointer_w_error(AstfriNode* const&)
    {
    }

    template<astfri_node AstfriNode>
    void AbstractVisitor::check_and_accept_pointer(AstfriNode* const&)
    {
    }

    template<typename VectorAstfriNode>
    void AbstractVisitor::write_params_or_args(VectorAstfriNode const, bool)
    {
    }

    template<typename VectorAstfriNode>
    bool AbstractVisitor::has_acc_mod(VectorAstfriNode const&, VectorAstfriNode&, AccessModifier)
    {
    }
}

#endif