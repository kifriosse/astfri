#include <libastfri-text/inc/tools/Exporter.hpp>

#include <fstream>

using namespace astfri::text;

Exporter::Exporter(TextLibConfig* config) :
    m_config(config)
{
    std::filesystem::path cp = std::filesystem::current_path();
    bool foundBuildFolder = false;
    while (true)
    {
        std::filesystem::path buildPath = cp / "build";
        std::error_code ec;
        if (std::filesystem::is_directory(buildPath, ec) && !ec)
        {
            foundBuildFolder = true;
            m_defPath = buildPath;
            m_defPath /= "text_output";
            break;
        }
        if (cp == cp.root_path())
        {
            break;
        }
        cp = cp.parent_path();
    }
    if (!foundBuildFolder)
    {
        m_defPath = "text_output";
    }
}

void Exporter::export_file(std::string_view text, std::ostream* ost) const
{
    if (ost)
    {
        *ost << text;
        return;
    }
    std::filesystem::path directory;
    if (m_config->filePath == "default")
    {
        directory = m_defPath;
    }
    else if (m_config->filePath.empty())
    {
        directory = std::filesystem::current_path() / "text_output";
    }
    else
    {
        directory = m_config->filePath;
        directory /= "text_output";
    }
    std::filesystem::create_directories(directory);
    std::filesystem::path fullPath = directory;
    fullPath /= create_file_name();
    std::ofstream file(fullPath);
    if (!file)
    {
        throw std::runtime_error("Cannot create file: " + fullPath.string());
    }
    file << text;
}

std::string Exporter::create_file_name() const
{
    if (m_config->fileFormat == "c++")
    {
        return m_config->fileName + ".hpp";
    }
    else if (m_config->fileFormat == "java")
    {
        return m_config->fileName + ".java";
    }
    else if (m_config->fileFormat == "html")
    {
        return m_config->fileName + ".html";
    }
    else
    {
        return m_config->fileName + ".txt";
    }
}