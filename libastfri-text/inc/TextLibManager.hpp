#ifndef LIBASTFRI_TEXT_TEXT_LIB_MANAGER
#define LIBASTFRI_TEXT_TEXT_LIB_MANAGER

#include <libastfri-text/inc/tools/AbstractVisitor.hpp>
#include <libastfri-text/inc/tools/Exporter.hpp>

namespace astfri::text
{
    enum class TextOutputFormat
    {
        CxxCode,
        JavaCode,
        TxtPseudocode,
        HtmlPseudocode
    };

    class TextLibManager
    {
        TextLibConfig* const m_config;
        AbstractBuilder* m_builder;
        AbstractVisitor* m_visitor;
        Exporter* const m_exporter;
        bool m_isSetToPseudocode;
    public:
        explicit TextLibManager();
        ~TextLibManager();
        //
        static void process_ast(TextLibConfig cfg, TranslationUnit const& root);
        static void process_ast(TextLibConfig cfg, TranslationUnit const& root, std::ostream& ost);
    private:
        static void process_ast(TextLibConfig cfg, TranslationUnit const& root, std::ostream* ost);
    public:
        void process_and_export_ast(TranslationUnit const& root, std::ostream* ost);
        void process_ast(TranslationUnit const& root);
        void export_ast(std::ostream* ost);
        //
        void change_output_format(TextOutputFormat const& format);
        void change_config(std::filesystem::path const& path);
        void change_config(rapidjson::Value const& json);
    private:
        void change_output_format(std::string_view format);
    };
    static_assert(IsOutputLibInterface<TextLibManager, TextLibConfig, rapidjson::Value>);
}

#endif