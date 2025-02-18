#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <iostream>
#include <fstream>

TxtFileExporter::TxtFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(conf) {}

void TxtFileExporter::make_export() {
    std::string filePath = config_->get_output_file_path()->str() + ".txt";
    std::ofstream file(std::move(filePath));
    if (file) {
        file << std::move(output_->str());
        file.close();
        std::cout << "Zápis prebehol úspešne!\n";
        return;
    }
    std::cout << "Nastala chyba. Použijem predvolenú cestu!\n";
    filePath = config_->get_default_output_path()->str() + ".txt";
    std::ofstream defFile(std::move(filePath));
    if (defFile) {
        defFile << std::move(output_->str());
        defFile.close();
        std::cout << "Zápis prebehol úspešne!\n";
    } else {
        std::cout << "Nastala chyba pri zápise!\n";
    }
}