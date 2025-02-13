#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <iostream>
#include <fstream>

HtmlFileExporter::HtmlFileExporter(const Configurator* conf, std::stringstream* output) : Exporter(conf, output) {}

void HtmlFileExporter::make_export() {
    std::string userInput = config_->get_output_file_name()->str() + ".html";
    std::string user = getenv("USER");
    std::ofstream file("/mnt/c/Users/" + user + "/Desktop/" + userInput);
    if (file) {
        file << "<!DOCTYPE html>\n";
        file << "<html lang=\"sk\">\n";
        file << "<head>\n";
        file << "<meta charset=\"UTF-8\">\n";
        file << "<title>" << userInput << "</title>\n";
        //formatovanie
        file << "<style>\n";
        file << ".output { font-size: 20px; font-family: Calibri }\n";
        file << ".class-word { font-weight: bold; color: " << config_->get_class_word_color()->str() << "; font-size: 21px; }\n";
        file << ".class-name { color: " << config_->get_class_name_color()->str() << " }\n";
        file << "</style>\n";
        file << "</head>\n";
        file << "<body>\n";
        file << "<span class=\"output\">\n";
        file << output_->str();
        file << "</span>\n";
        file << "</body>\n";
        file << "</html>";
        std::cout << "Zápis prebehol úspešne!\n";
    } else {
        std::cout << "Nastala chyba pri zápise do súboru!\n";
    }
    file.close();
}

void HtmlFileExporter::write_indentation() {
    config_->show_row_number() ? write_row_number() : void();
    *output_ << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    for (int i = 0; i < currentIndentation_; ++i) {
        for (size_t j = 0; j < config_->get_tab_word()->str().length(); ++j) {
            *output_ << "&nbsp;";
        }
    }
    startedLine_ = true;
}

void HtmlFileExporter::write_new_line() {
    write_word("<br>");
    Exporter::write_new_line();
}

void HtmlFileExporter::write_class_word() {
    write_word("<span class=\"class-word\">");
    write_word(config_->get_class_word()->str());
    write_word("</span>");
}

void HtmlFileExporter::write_class_name(const std::string& name) {
    write_word("<span class=\"class-name\">");
    write_word(name);
    write_word("</span>");
}

void HtmlFileExporter::write_row_number() {
    if (row_ > 99) {
        *output_ << row_ << ".";
    } else if (row_ > 9) {
        *output_ << "&nbsp;&nbsp;" << row_ << ".";
    } else {
        *output_ << "&nbsp;&nbsp;&nbsp;&nbsp;" << row_ << ".";
    }
}