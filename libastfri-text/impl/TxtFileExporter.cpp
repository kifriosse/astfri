#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <iostream>
#include <fstream>

TxtFileExporter::TxtFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(std::move(conf)) {}

void TxtFileExporter::make_export() {
    check_output_file_path(std::move(".txt"));
}

void TxtFileExporter::write_output_into_file(std::string filepath) {
    std::cout << std::move("Súbor nájdeš na ceste: ") << filepath << std::move("\n");
    std::ofstream file(std::move(filepath));
    file << std::move(output_->str());
    file.close();
    std::cout << std::move("Zápis prebehol úspešne!\n");
}