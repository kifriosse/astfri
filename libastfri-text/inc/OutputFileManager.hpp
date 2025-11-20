#ifndef LIBASTFRI_TEXT_OUTPUT_FILE_MANAGER
#define LIBASTFRI_TEXT_OUTPUT_FILE_MANAGER

#include <string>

namespace astfri::text
{
    class OutputFileManager
    {
        std::string defaultFolderPath_;
        std::string desktopFolderPath_;
        std::string buildFolderPath_;
    public:
        static OutputFileManager& get_instance();
        OutputFileManager(OutputFileManager const&) = delete;
        OutputFileManager(OutputFileManager&&)      = delete;
        OutputFileManager& operator=(OutputFileManager const&) = delete;
        OutputFileManager& operator=(OutputFileManager&&)      = delete;
    private:
        explicit OutputFileManager();
        ~OutputFileManager() = default;
    public:
        void check_and_set_file_name(std::string& filename);
        void check_and_set_file_path(std::string& filepath);
        std::string default_folder_path();
        std::string desktop_folder_path();
        std::string build_folder_path();
    private:
        void set_default_folder_path();
        void set_desktop_folder_path();
        void set_build_folder_path();
        void remove_spaces(std::string& text);
        void remove_all_slashes(std::string& text);
        void remove_multiple_slashes(std::string& text);
    };
}

#endif