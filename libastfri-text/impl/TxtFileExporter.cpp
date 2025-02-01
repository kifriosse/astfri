#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>

TxtFileExporter::TxtFileExporter(const Configurator* conf, std::stringstream* output) : Exporter(conf, output) {}

void TxtFileExporter::make_export() {
    namespace fs = std::filesystem;
    std::cout << "Zadaj názov súboru bez prípony: ";
    std::string userInput;
    std::getline(std::cin, userInput);
    userInput.append(".txt");
    fs::path filePath = fs::current_path().parent_path().parent_path() / userInput;
    std::ofstream file(filePath);
    if (file) {
        file << get_output()->str();
        std::cout << "Zápis prebehol úspešne!\n";
    }
    file.close();
}