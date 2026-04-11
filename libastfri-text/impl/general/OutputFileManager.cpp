#include <libastfri-text/inc/general/OutputFileManager.hpp>

#include <sys/utsname.h>
#include <filesystem>
#include <fstream>

using namespace astfri::text;

OutputFileManager& OutputFileManager::get_instance()
{
    static OutputFileManager manager;
    return manager;
}

OutputFileManager::OutputFileManager()
{
    set_default_folder_path();
    set_desktop_folder_path();
    set_build_folder_path();
}

//
// -----
//

void OutputFileManager::check_and_set_file_name(std::string& filename)
{
    remove_spaces(filename);
    remove_all_slashes(filename);
    if (filename.length() == 0)
    {
        filename = "output";
    }
}

void OutputFileManager::check_and_set_file_path(std::string& filepath)
{
    if (filepath == "default")
    {
        filepath = m_defaultFolderPath;
        return;
    }
    if (filepath == "desktop")
    {
        filepath = m_desktopFolderPath;
        return;
    }
    if (filepath == "build")
    {
        filepath = m_buildFolderPath;
        return;
    }
    remove_spaces(filepath);
    remove_multiple_slashes(filepath);
    if (!filepath.ends_with('/'))
    {
        filepath.append("/");
    }
    std::error_code ec;
    if (std::filesystem::is_directory(filepath, ec) && ! ec)
    {
        std::ofstream file(filepath + "tmptmprandomtmpfile9488456198.txt");
        if (file.is_open())
        {
            file.close();
            std::filesystem::remove(filepath + "tmptmprandomtmpfile9488456198.txt");
            return;
        }
    }
    filepath = m_defaultFolderPath;
}

std::string OutputFileManager::default_folder_path()
{
    return m_defaultFolderPath;
}

std::string OutputFileManager::desktop_folder_path()
{
    return m_desktopFolderPath;
}

std::string OutputFileManager::build_folder_path()
{
    return m_buildFolderPath;
}

void OutputFileManager::set_default_folder_path()
{
    m_defaultFolderPath = std::filesystem::current_path().string();
    m_defaultFolderPath.append("/text_output/");
}

void OutputFileManager::set_desktop_folder_path()
{
    struct utsname buffer;
    if (uname(&buffer) == 0)
    {
        std::string release = buffer.release;
        if (release.find("Microsoft") != std::string::npos ||
            release.find("microsoft") != std::string::npos)
        {
            FILE* file = popen("cd /mnt/c && cmd.exe /c \"echo %USERNAME%\"", "r");
            std::stringstream res;
            char ch;
            while ((ch = fgetc(file)) != EOF)
            {
                res.put(ch);
            }
            pclose(file);
            std::string user = res.str();
            user.pop_back();
            user.pop_back();
            m_desktopFolderPath = "/mnt/c/Users/" + user + "/Desktop/text_output/";
        }
        else
        {
            m_desktopFolderPath = std::getenv("HOME");
            m_desktopFolderPath.append("/Desktop/text_output/");
        }
    }
}

void OutputFileManager::set_build_folder_path()
{
    std::filesystem::path cp = std::filesystem::current_path();
    bool foundBuildFolder = false;
    while (true)
    {
        std::filesystem::path buildPath = cp / "build";
        std::error_code ec;
        if (std::filesystem::is_directory(buildPath, ec) && ! ec)
        {
            foundBuildFolder = true;
            m_buildFolderPath = buildPath;
            m_buildFolderPath.append("/text_output/");
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
        m_buildFolderPath = m_defaultFolderPath;
    }
}

void OutputFileManager::remove_spaces(std::string& text)
{
    size_t p = 0;
    while ((p = text.find(' ', p)) != std::string::npos)
    {
        text.replace(p, 1, "");
    }
}

void OutputFileManager::remove_all_slashes(std::string& text)
{
    size_t p = 0;
    while ((p = text.find('/', p)) != std::string::npos)
    {
        text.replace(p, 1, "");
    }
    p = 0;
    while ((p = text.find('\\', p)) != std::string::npos)
    {
        text.replace(p, 1, "");
    }
}

void OutputFileManager::remove_multiple_slashes(std::string& text)
{
    size_t p = 0;
    while ((p = text.find('\\', p)) != std::string::npos)
    {
        text.replace(p, 1, "/");
    }
    p = 0;
    while ((p = text.find("//", p)) != std::string::npos)
    {
        text.replace(p, 2, "/");
    }
}