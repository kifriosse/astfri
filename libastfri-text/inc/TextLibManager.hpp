#ifndef LIBASTFRI_TEXT_TEXT_LIB_MANAGER
#define LIBASTFRI_TEXT_TEXT_LIB_MANAGER

#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

#include <iostream>

namespace astfri::text
{
    enum class OutputFormat
    {
        CxxCode,
        JavaCode,
        Pseudocode
    };

    //
    // -----
    //

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
        void change_output(OutputFormat const& format);
        void execute_export();
        void clear_builder();
        void append_text(std::string const& text);
        void append_new_line();
        void append_space();
        void update_configuration();
        void reload_configuration();
        // -----
        template<astfri_node Node>
        void visit_and_export(Node const& node);
        // -----
        template<astfri_node Node>
        void visit(Node const& node);
    };

    //
    // -----
    //

    template<astfri_node Node>
    void TextLibManager::visit_and_export(Node const& node)
    {
        visit(node);
        execute_export();
    }

    // -----

    template<astfri_node Node>
    void TextLibManager::visit(Node const& node)
    {
        auto* tu = dynamic_cast<TranslationUnit const*>(&node);
        if (dynamic_cast<AbstractCodeVisitor*>(visitor_) && !tu)
        {
            std::cout << " > Instance of TranslationUnit required to generate code.\n";
            return;
        }
        const_cast<Node&>(node).accept(*visitor_);
    }
}

#endif