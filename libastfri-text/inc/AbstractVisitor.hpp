#ifndef LIBASTFRI_TEXT_ABSTRACT_VISITOR
#define LIBASTFRI_TEXT_ABSTRACT_VISITOR

#include <libastfri/inc/Astfri.hpp>

#include <libastfri-text/inc/AbstractBuilder.hpp>

namespace astfri::text
{
    template<typename Node>
    concept astfri_node =
        requires(Node& node)
        {
            { node.accept(std::declval<IVisitor&>()) } -> std::same_as<void>;
        };

    // -----

    template<typename Vector>
    concept v_astfri_nodes =
        requires
        {
            typename Vector::value_type;
        } &&
        std::same_as<Vector, std::vector<typename Vector::value_type>> &&
        std::is_pointer_v<typename Vector::value_type> &&
        astfri_node<std::remove_pointer_t<typename Vector::value_type>>;

    // -----

    template<typename Vector>
    concept v_astfri_members =
        std::same_as<Vector, std::vector<MemberVarDefStmt*>> ||
        std::same_as<Vector, std::vector<ConstructorDefStmt*>> ||
        std::same_as<Vector, std::vector<MethodDefStmt*>>;

    // -----

    template<typename Vector>
    concept v_astfri_supertypes =
        std::same_as<Vector, std::vector<ClassDefStmt*>> ||
        std::same_as<Vector, std::vector<InterfaceDefStmt*>>;

    //
    // -----
    //

    class AbstractVisitor : public ThrowingVisitorAdapter
    {
    protected:
        AbstractBuilder* builder_;
    protected:
        AbstractVisitor() = delete;
        explicit AbstractVisitor(AbstractBuilder& builder);
        virtual ~AbstractVisitor() = default;
    protected:
        void process_condition(Expr const* const& expr);
        // -----
        template<v_astfri_nodes Vector>
        void process_pargs(Vector const& pargs, bool useGeneric);
        // -----
        template<astfri_node Node>
        bool try_accept_node(Node const* const& node);
        // -----
        template<v_astfri_members Vector>
        bool try_find_access_mod(Vector const& all, Vector& found, AccessModifier mod);
    };

    //
    // -----
    //

    template<v_astfri_nodes Vector>
    void AbstractVisitor::process_pargs(Vector const& pargs, bool useGeneric)
    {
        if (useGeneric)
        {
            builder_->write_left_bracket("<");
        }
        else
        {
            builder_->write_left_bracket("(");
        }
        for (size_t i = 0; i < pargs.size(); ++i)
        {
            if (!try_accept_node(pargs.at(i)))
            {
                builder_->write_invalid_expr();
            }
            if (i < pargs.size() - 1)
            {
                builder_->write_separator(",");
                builder_->append_space();
            }
        }
        if (useGeneric)
        {
            builder_->write_right_bracket(">");
        }
        else
        {
            builder_->write_right_bracket(")");
        }
    }

    // -----

    template<astfri_node Node>
    bool AbstractVisitor::try_accept_node(Node const* const& node)
    {
        if (node)
        {
            const_cast<Node*>(node)->accept(*this);
            return true;
        }
        return false;
    }

    // -----

    template<v_astfri_members Vector>
    bool AbstractVisitor::try_find_access_mod(
        Vector const& all, Vector& found, AccessModifier mod)
    {
        found.clear();
        for (size_t i = 0; i < all.size(); ++i)
        {
            if (all.at(i) && all.at(i)->access_ == mod)
            {
                found.push_back(all.at(i));
            }
        }
        return !found.empty();
    }
}

#endif