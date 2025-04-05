#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <iostream>
#include <fstream>

TxtFileExporter::TxtFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(std::move(conf)) {}

void TxtFileExporter::make_export() { create_folder(".txt"); }

void TxtFileExporter::write_output_into_file(const std::string& filepath) {
    std::cout << "Súbor nájdeš na ceste: " << filepath << "\n";
    std::ofstream file(std::move(filepath));
    file << output_->str();
    file.close();
    std::cout << "Zápis prebehol úspešne!\n";
}