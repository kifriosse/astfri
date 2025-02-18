#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <iostream>
#include <fstream>

HtmlFileExporter::HtmlFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(conf) {}

void HtmlFileExporter::make_export() {
    std::string filePath = config_->get_output_file_path()->str() + ".html";
    std::ofstream file(std::move(filePath));
    if (file) {
        file << "<!DOCTYPE html>\n";
        file << "<html lang=\"sk\">\n";
        file << "<head>\n";
        file << "<meta charset=\"UTF-8\">\n";
        file << "<style>\n";
        file << ".output { font-size: 20px; font-family: Bell MT }\n";
        file << ".class-word { font-weight: bold; color: " << std::move(config_->get_class_word_color()->str()) << "; font-size: 20px; }\n";
        file << ".class-name { color: " << std::move(config_->get_class_name_color()->str()) << " }\n";
        file << "</style>\n";
        file << "</head>\n";
        file << "<body>\n";
        file << "<span class=\"output\">\n";
        file << std::move(output_->str());
        file << "</span>\n";
        file << "</body>\n";
        file << "</html>";
        file.close();
        std::cout << "Zápis prebehol úspešne!\n";
        return;
    }
    std::cout << "Nastala chyba. Použijem predvolenú cestu!\n";
    filePath = config_->get_default_output_path()->str() + ".html";
    std::ofstream defFile(std::move(filePath));
    if (defFile) {
        defFile << "<!DOCTYPE html>\n";
        defFile << "<html lang=\"sk\">\n";
        defFile << "<head>\n";
        defFile << "<meta charset=\"UTF-8\">\n";
        defFile << "<style>\n";
        defFile << ".output { font-size: 20px; font-family: Bell MT }\n";
        defFile << ".class-word { font-weight: bold; color: " << std::move(config_->get_class_word_color()->str()) << "; font-size: 20px; }\n";
        defFile << ".class-name { color: " << std::move(config_->get_class_name_color()->str()) << " }\n";
        defFile << "</style>\n";
        defFile << "</head>\n";
        defFile << "<body>\n";
        defFile << "<span class=\"output\">\n";
        defFile << std::move(output_->str());
        defFile << "</span>\n";
        defFile << "</body>\n";
        defFile << "</html>";
        defFile.close();
        std::cout << "Zápis prebehol úspešne!\n";
    } else {
        std::cout << "Nastala chyba pri zápise!\n";
    }
}

void HtmlFileExporter::write_indentation() {
    config_->sh_row_number() ? write_row_number() : void();
    for (int i = 0; i < config_->get_len_left_margin(); ++i) {
        *output_ << "&nbsp;";
    }
    for (int i = 0; i < currentIndentation_; ++i) {
        for (int j = 0; j < config_->get_len_tab_word(); ++j) {
            *output_ << "&nbsp;";
        }
    }
    startedLine_ = true;
}

void HtmlFileExporter::write_new_line() {
    write_word(std::move("<br>"));
    Exporter::write_new_line();
}

void HtmlFileExporter::write_class_word() {
    write_word(std::move("<span class=\"class-word\">"));
    write_word(config_->get_class_word()->str());
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_class_name(std::string name) {
    write_word(std::move("<span class=\"class-name\">"));
    write_word(std::move(name));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_row_number() {
    if (row_ > 99) {
        *output_ << row_ << std::move(".");
    } else if (row_ > 9) {
        *output_ << std::move("&nbsp;&nbsp;") << row_ << std::move(".");
    } else {
        *output_ << std::move("&nbsp;&nbsp;&nbsp;&nbsp;") << row_ << std::move(".");
    }
}