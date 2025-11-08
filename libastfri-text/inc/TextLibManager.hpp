#ifndef LIBASTFRI_TEXT_TEXT_LIB_MANAGER
#define LIBASTFRI_TEXT_TEXT_LIB_MANAGER

#include <libastfri-text/inc/AbstractVisitor.hpp>

namespace astfri::text
{
    class TextLibManager
    {
        AbstractVisitor* visitor_;
        AbstractBuilder* builder_;
    public:
        static TextLibManager& get_instance();
        TextLibManager(TextLibManager const&) = delete;
        TextLibManager(TextLibManager&&)      = delete;
        TextLibManager& operator=(TextLibManager const&) = delete;
        TextLibManager& operator=(TextLibManager&&)      = delete;
    private:
        TextLibManager();
        ~TextLibManager();
    public:
        void export_pseudocode();
        void reset_builder();
        // -----
        template<astfri_node AstfriNode>
        void visit(AstfriNode const& node);
        // -----
        template<astfri_node AstfriNode>
        void visit_and_export(AstfriNode const& node);
    private:
        void create_new_builder();
    };

    //
    // -----
    //

    template<astfri_node AstfriNode>
    void TextLibManager::visit(AstfriNode const& node)
    {
        const_cast<AstfriNode&>(node).accept(*visitor_);
    }

    template<astfri_node AstfriNode>
    void TextLibManager::visit_and_export(AstfriNode const& node)
    {
        const_cast<AstfriNode&>(node).accept(*visitor_);
    }
}

#endif