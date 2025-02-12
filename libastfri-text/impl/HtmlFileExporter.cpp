#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <iostream>
#include <fstream>

HtmlFileExporter::HtmlFileExporter(const Configurator* conf, std::stringstream* output) : Exporter(conf, output) {}

void HtmlFileExporter::make_export() {
    std::string userInput = get_config()->get_output_file_name()->str();
    userInput.append(".html");
    std::ofstream file("/mnt/c/users/marek/desktop/" + userInput);
    if (file) {
        file << "<!DOCTYPE html>\n";
        file << "<html lang=\"sk\">\n";
        file << "<head>\n";
        file << "<meta charset=\"UTF-8\">\n";
        file << "<title>" << userInput << "</title>\n";
        //formatovanie
        file << "<style>\n";
        file << ".class-word { font-weight: bold; color: " << get_config()->get_class_word_color()->str() << "; font-size: 20px; }\n";
        file << ".class-name { color: " << get_config()->get_class_name_color()->str() << " }\n";
        file << "</style>\n";
        file << "</head>\n";
        file << "<body>\n";
        file << get_output()->str();
        file << "</body>\n";
        file << "</html>";
        std::cout << "Zápis prebehol úspešne!\n";
    } else {
        std::cout << "Nastala chyba pri zápise do súboru!\n";
    }
    file.close();
}

void HtmlFileExporter::write_indentation() {
    for (int i = 0; i < get_indentation(); ++i) {
        for (size_t j = 0; j < get_config()->get_tab_word()->str().length(); ++j) {
            *get_output() << "&nbsp;";
            set_started_line(true);
        }
    }
}

void HtmlFileExporter::write_new_line() {
    write_word("<br>\n", true);
}

void HtmlFileExporter::write_class_word() {
    write_word("<span class=\"class-word\">");
    write_word(get_config()->get_class_word()->str());
    write_word("</span>");
}

void HtmlFileExporter::write_class_name(const std::string& name) {
    write_word("<span class=\"class-name\">");
    write_word(name);
    write_word("</span>");
}