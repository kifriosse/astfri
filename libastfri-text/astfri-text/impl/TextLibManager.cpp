#include <astfri-text/TextLibManager.hpp>
#include <astfri-text/impl/PlainTextBuilder.hpp>
#include <astfri-text/impl/PseudocodeVisitor.hpp>

using namespace astfri::text;

TextLibManager::TextLibManager() :
    m_config(Config::createDefault()),
    m_exporter(Exporter(m_config)),
    m_builder(new PlainTextBuilder(m_config)),
    m_visitor(new PseudocodeVisitor(static_cast<PlainTextBuilder&>(*m_builder), m_config))
{
}

TextLibManager::~TextLibManager()
{
    // TODO MM: Use smart pointers or values.
    delete m_builder;
    delete m_visitor;
}

std::string_view TextLibManager::version()
{
    return ASTFRI_TEXT_VERSION;
}

void TextLibManager::process_ast(Config cfg, TranslationUnit const& root)
{
    process_ast(cfg, root, nullptr);
}

void TextLibManager::process_ast(Config cfg, TranslationUnit const& root, std::ostream& ost)
{
    process_ast(cfg, root, &ost);
}

void TextLibManager::process_ast(Config cfg, TranslationUnit const& root, std::ostream* ost)
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
        builder = new PlainTextBuilder(cfg);
        visitor = new PseudocodeVisitor(static_cast<PlainTextBuilder&>(*builder), cfg);
    }
    visitor->accept_node(const_cast<TranslationUnit*>(&root));
    Exporter exporter(cfg);
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
    m_exporter.export_file(m_builder->get_builded_text(), ost);
    m_builder->reset_builder();
    m_visitor->reset_visitor();
}

void TextLibManager::change_output_format(TextOutputFormat const& format)
{
    switch (format)
    {
        case TextOutputFormat::CxxCode:
            change_output_format("c++");
            break;
        case TextOutputFormat::JavaCode:
            change_output_format("java");
            break;
        case TextOutputFormat::TxtPseudocode:
            change_output_format("txt");
            break;
        case TextOutputFormat::HtmlPseudocode:
            change_output_format("html");
            break;
    }
}

void TextLibManager::change_config(std::filesystem::path const& path)
{
    m_config = Config::createFromJson(path);
    change_output_format(m_config.fileFormat);
}

void TextLibManager::change_config(rapidjson::Value const& json)
{
    m_config = Config::createFromJson(json);
    change_output_format(m_config.fileFormat);
}

void TextLibManager::change_output_format(std::string_view format)
{
    delete m_builder;
    delete m_visitor;
    if (format == "c++")
    {
        m_config.fileFormat = "c++";
        // TODO: implement cxx
    }
    else if (format == "java")
    {
        m_config.fileFormat = "java";
        // TODO: implement java
    }
    else if (format == "html")
    {
        m_config.fileFormat = "html";
        // TODO: implement html pseudocode
    }
    else
    {
        m_config.fileFormat = "txt";
        m_builder = new PlainTextBuilder(m_config);
        m_visitor = new PseudocodeVisitor(static_cast<PlainTextBuilder&>(*m_builder), m_config);
    }
}