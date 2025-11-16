#ifndef LIBASTFRI_TEXT_TEXT_LIB_MANAGER
#define LIBASTFRI_TEXT_TEXT_LIB_MANAGER

#include <libastfri-text/inc/AbstractVisitor.hpp>

namespace astfri::text
{
    class TextLibManager
    {
        AbstractVisitor* visitor_;
    public:
        static TextLibManager& get_instance();
        TextLibManager(TextLibManager const&) = delete;
        TextLibManager(TextLibManager&&)      = delete;
        TextLibManager& operator=(TextLibManager const&) = delete;
        TextLibManager& operator=(TextLibManager&&)      = delete;
    private:
        explicit TextLibManager();
        ~TextLibManager() = default;
    public:
        // -----
        template<astfri_node Node>
        void visit(Node const& node);
        // -----
        template<astfri_node Node>
        void visit_and_export(Node const& node);
    };

    //
    // -----
    //

    template<astfri_node Node>
    void TextLibManager::visit(Node const& node)
    {
        const_cast<Node&>(node).accept(*visitor_);
    }

    // -----

    template<astfri_node Node>
    void TextLibManager::visit_and_export(Node const& node)
    {
        const_cast<Node&>(node).accept(*visitor_);
    }
}

#endif