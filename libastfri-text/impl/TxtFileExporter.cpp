#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <iostream>
#include <fstream>

TxtFileExporter::TxtFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(std::move(conf)) {}

void TxtFileExporter::make_export() {
    check_file(std::move(".txt"));
}

void TxtFileExporter::write_file(std::string path) {
    std::cout << std::move("Súbor nájdeš na ceste: ") << path << std::move("\n");
    std::ofstream file(std::move(path));
    file << std::move(output_->str());
    file.close();
    std::cout << std::move("Zápis prebehol úspešne!\n");
}