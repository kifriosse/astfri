#ifndef LIBASTFRI_TEXT_TEXT_LIB_MANAGER
#define LIBASTFRI_TEXT_TEXT_LIB_MANAGER

#include <libastfri-text/inc/pseudocode/PseudocodeVisitor.hpp>

namespace astfri::text
{
    class TextLibManager
    {
        TextConfigurator* configurator_;
        PseudocodeVisitor* visitor_;
        Exporter* exporter_;
        std::string currentOutputFileFormat_;
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
        void update_configuration();
        void reload_configuration();
        void execute_export();
        void reset_exporter();
        void write_new_line();
        // -----
        template<AnyAstfriNode AstfriNode>
        void visit(AstfriNode const& node);
        // -----
        template<AnyAstfriNode AstfriNode>
        void visit_and_export(AstfriNode const& node);
    private:
        void check_current_file_format();
        void create_new_exporter();
    };

    //
    // -----
    //

    template<AnyAstfriNode AstfriNode>
    void TextLibManager::visit(AstfriNode const& node)
    {
        const_cast<AstfriNode&>(node).accept(*visitor_);
    }

    template<AnyAstfriNode AstfriNode>
    void TextLibManager::visit_and_export(AstfriNode const& node)
    {
        const_cast<AstfriNode&>(node).accept(*visitor_);
        exporter_->execute_export();
    }
}

#endif