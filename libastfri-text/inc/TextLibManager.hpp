#ifndef LIBASTFRI_TEXT_TEXT_LIB_MANAGER
#define LIBASTFRI_TEXT_TEXT_LIB_MANAGER

#include <libastfri-text/inc/ASTTextVisitor.hpp>

namespace astfri::text
{
    template<typename AstfriPeak>
    concept VisitableAstfriPeak =
        //!std::is_abstract_v<AstfriPeak> &&
        requires(AstfriPeak& peak)
        {
            {
                peak.accept(std::declval<IVisitor&>())
            } -> std::same_as<void>;
        };

    // -----

    class TextLibManager
    {
        TextConfigurator* configurator_;
        ASTTextVisitor* visitor_;
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
        template<VisitableAstfriPeak AstfriPeak>
        void visit(AstfriPeak const& peak);
        // -----
        template<VisitableAstfriPeak AstfriPeak>
        void visit_and_export(AstfriPeak const& peak);
    private:
        void check_current_file_format();
        void create_new_exporter();
    };

    // -----

    template<VisitableAstfriPeak AstfriPeak>
    void TextLibManager::visit(AstfriPeak const& peak)
    {
        visitor_->visit(peak);
    }

    template<VisitableAstfriPeak AstfriPeak>
    void TextLibManager::visit_and_export(AstfriPeak const& peak)
    {
        visitor_->visit(peak);
        exporter_->execute_export();
    }
}

#endif