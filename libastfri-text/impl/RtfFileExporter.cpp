#include <libastfri-text/inc/RtfFileExporter.hpp>
#include <iostream>
#include <fstream>

RtfFileExporter::RtfFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(std::move(conf)) {}

void RtfFileExporter::make_export() {
    check_file(std::move(".rtf"));
}

void RtfFileExporter::write_file(std::string path) {
    std::cout << std::move("Súbor nájdeš na ceste: ") << path << std::move("\n");
    std::ofstream file(std::move(path));
    file << std::move(output_->str());
    file.close();
    std::cout << std::move("Zápis prebehol úspešne!\n");
}