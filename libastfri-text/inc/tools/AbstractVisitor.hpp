#ifndef LIBASTFRI_TEXT_ABSTRACT_VISITOR
#define LIBASTFRI_TEXT_ABSTRACT_VISITOR

#include <astfri/Astfri.hpp>
#include <libastfri-text/inc/tools/AbstractBuilder.hpp>

namespace astfri::text
{
    class AbstractVisitor : public ThrowingVisitorAdapter
    {
    protected:
        AbstractBuilder* m_builder;
    public:
        explicit AbstractVisitor(AbstractBuilder* builder);
        virtual ~AbstractVisitor() = default;
        //
        virtual void reset_visitor() = 0;
        virtual void replace_builder(AbstractBuilder* builder) = 0;
        void process_condition(Expr* expr);
        void process_body(Stmt* stmt, bool const& onNewLine);
        //
        template<typename Node>
        void accept_node(Node* node);
        //
        template<typename Vector>
        void process_params_or_args(Vector const& pargs, bool useGeneric);
        //
        template<typename Vector>
        bool try_find_access_mod(Vector const& all, Vector& found, AccessModifier mod);
    };

    template<typename Node>
    void AbstractVisitor::accept_node(Node* node)
    {
        node->accept(*this);
    }

    template<typename Vector>
    void AbstractVisitor::process_params_or_args(Vector const& pargs, bool useGeneric)
    {
        if (useGeneric)
        {
            m_builder->write_left_bracket("<");
        }
        else
        {
            m_builder->write_left_bracket("(");
        }
        for (size_t i = 0; i < pargs.size(); ++i)
        {
            accept_node(pargs.at(i));
            if (i < pargs.size() - 1)
            {
                m_builder->write_comma_space();
            }
        }
        if (useGeneric)
        {
            m_builder->write_right_bracket(">");
        }
        else
        {
            m_builder->write_right_bracket(")");
        }
    }

    template<typename Vector>
    bool AbstractVisitor::try_find_access_mod(Vector const& all, Vector& found, AccessModifier mod)
    {
        found.clear();
        for (size_t i = 0; i < all.size(); ++i)
        {
            if (all.at(i)->access == mod)
            {
                found.push_back(all.at(i));
            }
        }
        return !found.empty();
    }
}

#endif