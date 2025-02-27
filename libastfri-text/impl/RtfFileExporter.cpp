#include <libastfri-text/inc/RtfFileExporter.hpp>
#include <iostream>
#include <fstream>

RtfFileExporter::RtfFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(std::move(conf)) {}

void RtfFileExporter::make_export() {
    check_output_file_path(std::move(".rtf"));
}

void RtfFileExporter::write_output_into_file(std::string filepath) {
    std::cout << std::move("Súbor nájdeš na ceste: ") << filepath << std::move("\n");
    std::ofstream file(std::move(filepath));
    file << std::move(output_->str());
    file.close();
    std::cout << std::move("Zápis prebehol úspešne!\n");
}