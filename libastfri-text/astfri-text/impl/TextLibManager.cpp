#include <astfri-text/TextLibManager.hpp>
#include <astfri-text/impl/PlainTextBuilder.hpp>
#include <astfri-text/impl/PseudocodeVisitor.hpp>

using namespace astfri::text;

TextLibManager::TextLibManager() :
    m_config(new TextLibConfig()), // TODO MM: Do not use new here.
    m_builder(new PlainTextBuilder(m_config)),
    m_visitor(new PseudocodeVisitor(static_cast<PlainTextBuilder*>(m_builder), m_config)),
    m_exporter(new Exporter(m_config)),
    m_isSetToPseudocode(true)
{
}

TextLibManager::~TextLibManager()
{
    delete m_config; // TODO MM: Won't be neccessary.
    delete m_builder;
    delete m_visitor;
    delete m_exporter;
}

std::string_view TextLibManager::version()
{
    return ASTFRI_TEXT_VERSION;
}

void TextLibManager::process_ast(TextLibConfig cfg, TranslationUnit const& root)
{
    process_ast(cfg, root, nullptr);
}

void TextLibManager::process_ast(TextLibConfig cfg, TranslationUnit const& root, std::ostream& ost)
{
    process_ast(cfg, root, &ost);
}

void TextLibManager::process_ast(TextLibConfig cfg, TranslationUnit const& root, std::ostream* ost)
{
    AbstractBuilder* builder = nullptr;
    AbstractVisitor* visitor = nullptr;
    if (cfg.fileFormat == "c++")
    {
    }
    else if (cfg.fileFormat == "java")
    {
    }
    else if (cfg.fileFormat == "html")
    {
    }
    else
    {
        builder = new PlainTextBuilder(&cfg);
        visitor = new PseudocodeVisitor(static_cast<PlainTextBuilder*>(builder), &cfg);
        cfg.fileFormat = "txt";
    }
    visitor->accept_node(const_cast<TranslationUnit*>(&root));
    Exporter exporter(&cfg);
    exporter.export_file(builder->get_builded_text(), ost);
    delete builder;
    delete visitor;
}

void TextLibManager::process_and_export_ast(TranslationUnit const& root, std::ostream* ost)
{
    m_visitor->accept_node(const_cast<TranslationUnit*>(&root));
    export_ast(ost);
}

void TextLibManager::process_ast(TranslationUnit const& root)
{
    m_visitor->accept_node(const_cast<TranslationUnit*>(&root));
}

void TextLibManager::export_ast(std::ostream* ost)
{
    m_exporter->export_file(m_builder->get_builded_text(), ost);
    m_builder->reset_builder();
    m_visitor->reset_visitor();
}

void TextLibManager::change_output_format(TextOutputFormat const& format)
{
    switch (format)
    {
        case TextOutputFormat::CxxCode:
            // TODO: implement cxx
            break;
        case TextOutputFormat::JavaCode:
            // TODO: implement java
            break;
        case TextOutputFormat::TxtPseudocode:
            change_output_format("txt");
            m_config->fileFormat = "txt";
            break;
        case TextOutputFormat::HtmlPseudocode:
            // TODO: implement html pseudocode
            break;
    }
}

void TextLibManager::change_config(std::filesystem::path const& path)
{
    m_config->change_to_default();
    m_config->load_from_file(path);
    change_output_format(m_config->fileFormat);
}

void TextLibManager::change_config(rapidjson::Value const& json)
{
    m_config->change_to_default();
    m_config->load_from_json(json);
    change_output_format(m_config->fileFormat);
}

void TextLibManager::change_output_format(std::string_view format)
{
    if (format == "c++")
    {
    }
    else if (format == "java")
    {
    }
    else if (format == "html")
    {
    }
    else
    {
        if (m_isSetToPseudocode)
        {
            if (dynamic_cast<PlainTextBuilder*>(m_builder))
            {
                m_builder->reset_builder();
            }
            else
            {
                delete m_builder;
                m_builder = new PlainTextBuilder(m_config);
                m_visitor->replace_builder(m_builder);
            }
            m_visitor->reset_visitor();
        }
        else
        {
            delete m_builder;
            delete m_visitor;
            m_builder = new PlainTextBuilder(m_config);
            m_visitor = new PseudocodeVisitor(static_cast<PlainTextBuilder*>(m_builder), m_config);
            m_isSetToPseudocode = true;
        }
        m_config->fileFormat = "txt";
    }
}