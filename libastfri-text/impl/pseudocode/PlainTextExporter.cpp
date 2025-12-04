#include <libastfri-text/inc/pseudocode/PlainTextExporter.hpp>

#include <libastfri-text/inc/pseudocode/TextConfigurator.hpp>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace astfri::text;

PlainTextExporter::PlainTextExporter(int rowCount, std::stringstream* const& text)
{
    execute(rowCount, text);
}

void PlainTextExporter::execute(int rowCount, std::stringstream* const& text)
{
    TextConfigurator& tc = TextConfigurator::get_instance();
    std::string suffix = ".txt";
    std::filesystem::path outputpath = tc.output_file_path()->str();
    std::string filename = tc.output_file_name()->str();
    outputpath.concat(filename + suffix);
    if (!tc.overwrite_file())
    {
        int id = 0;
        while (std::filesystem::exists(outputpath) && std::filesystem::is_regular_file(outputpath))
        {
            outputpath.replace_filename(filename + "(" + std::to_string(id) + ")" + suffix);
            ++id;
        }
    }
    try
    {
        std::filesystem::create_directories(outputpath.parent_path());
        write_pseudocode_into_file(rowCount, text, outputpath.string());
        std::cout << " > You can find output file at path: " << outputpath.string() << "\n";
        std::cout << " > File write completed!\n";
    }
    catch (std::filesystem::filesystem_error&)
    {
        std::cout << " > Ups! Something went wrong when creating output file!\n";
        std::cout << " > Try to change file name or file path.\n";
    }
}

void PlainTextExporter::write_pseudocode_into_file(int r, std::stringstream* const& text, std::string const& fullfilepath)
{
    TextConfigurator* tc = &TextConfigurator::get_instance();
    std::ofstream file(fullfilepath);
    if (tc->sh_row_num())
    {
        int row = 1;
        int delimiter = static_cast<int>(std::log10(r)) + 1;
        std::string line;
        while (std::getline(*text,line))
        {
            if (!line.empty() || (line.empty() && tc->sh_row_num_empty_row()))
            {
                for (int i = 0; i < tc->row_num_margin_left_len(); ++i)
                {
                    file << " ";
                }
                file << std::setw(delimiter) << row;
                if (tc->sh_dot_after_row_num())
                {
                    file << ".";
                }
                ++row;
            }
            else if (tc->reset_row_num_empty_row())
            {
                row = 1;
            }
            file << line << "\n";
        }
    }
    else
    {
        file << text->str();
    }
    file.close();
}