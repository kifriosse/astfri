#ifndef LIBASTFRI_TEXT_TEXT_LIB_MANAGER
#define LIBASTFRI_TEXT_TEXT_LIB_MANAGER

#include <libastfri-text/inc/ASTTextVisitor.hpp>

namespace astfri::text
{
    template <typename ASTFRI_PEAK>
    concept Visitable_ASTFRI_Peak =
        !std::is_abstract_v<ASTFRI_PEAK> &&
        requires(ASTFRI_PEAK& peak) {
            peak.accept(std::declval<IVisitor&>());
        };

    class TextLibManager
    {
        TextConfigurator* configurator_;
        ASTTextVisitor* visitor_;
        Exporter* exporter_;
        std::string currentOutputFileFormat_;
    public:
        TextLibManager(const TextLibManager&) = delete;
        TextLibManager(TextLibManager&&)      = delete;
        TextLibManager& operator=(const TextLibManager&) = delete;
        TextLibManager& operator=(TextLibManager&&)      = delete;
        static TextLibManager& get_instance();
    private:
        TextLibManager();
        ~TextLibManager();
    public:
        template <Visitable_ASTFRI_Peak ASTFRI_PEAK>
        void visit(const ASTFRI_PEAK& peak);
        template <Visitable_ASTFRI_Peak ASTFRI_PEAK>
        void visit_and_export(const ASTFRI_PEAK& peak);
        void reload_configuration(); // use for default settings
        void update_configuration(); // only read json file
        void execute_export();
        void reset_exporter();
        void write_new_line();
    private:
        void check_current_file_format();
        void create_new_exporter(const std::string& fileformat);
    };

    template <Visitable_ASTFRI_Peak ASTFRI_PEAK>
    inline void TextLibManager::visit(const ASTFRI_PEAK& peak)
    {
        visitor_->visit(peak);
    }

    template <Visitable_ASTFRI_Peak ASTFRI_PEAK>
    inline void TextLibManager::visit_and_export(const ASTFRI_PEAK& peak)
    {
        visitor_->visit(peak);
        execute_export();
    }
}

#endif