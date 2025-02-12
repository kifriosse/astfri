#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>

TxtFileExporter::TxtFileExporter(const Configurator* conf, std::stringstream* output) : Exporter(conf, output) {}

void TxtFileExporter::make_export() {
    namespace fs = std::filesystem;
    std::string userInput = config_->get_output_file_name()->str();
    userInput.append(".txt");
    fs::path filePath = fs::current_path().parent_path().parent_path() / userInput;
    std::ofstream file(filePath);
    if (file) {
        file << output_->str();
        std::cout << "Zápis prebehol úspešne!\n";
    } else {
        std::cout << "Nastala chyba pri zápise do súboru!\n";
    }
    file.close();
}