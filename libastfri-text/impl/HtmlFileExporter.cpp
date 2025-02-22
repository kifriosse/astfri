#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <iostream>
#include <fstream>

HtmlFileExporter::HtmlFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(std::move(conf)) {}

void HtmlFileExporter::make_export() {
    check_file(std::move(".html"));
}

void HtmlFileExporter::write_file(std::string path) {
    std::cout << std::move("Súbor nájdeš na ceste: ") << path << std::move("\n");
    std::ofstream file(std::move(path));
    file << std::move("<!DOCTYPE html>\n");
    file << std::move("<html lang=\"sk\">\n");
    file << std::move("<head>\n");
    file << std::move("<meta charset=\"UTF-8\">\n");
    file << std::move("<style>\n");
    file << std::move(".output { font-size: 20px; font-family: ") + config_->get_font()->str() + std::move(" }\n") ;
    file << std::move(".row-num { font-size: ") << std::move(config_->get_row_num_size()) << std::move("px }\n");
    file << std::move(".int-type { color: ") << std::move(" }\n");
    file << std::move(".user-type { color: brown") << std::move(" }\n");
    file << std::move(".method-name { color: ") << std::move(config_->get_method_name_color()->str()) << std::move(" }\n");
    file << std::move(".class-name { color: ") << std::move(config_->get_class_name_color()->str()) << std::move(" }\n");
    file << std::move(".class-word { color: ") << std::move(config_->get_class_word_color()->str()) << std::move(" }\n");
    file << std::move(".data-type { color: ") << std::move(config_->get_data_type_color()->str()) << std::move(" }\n");
    file << std::move(".access-mod { color: ") << std::move(config_->get_access_mod_color()->str()) << std::move(" }\n");
    file << std::move("</style>\n");
    file << std::move("</head>\n");
    file << std::move("<body>\n");
    file << std::move("<span class=\"output\">\n");
    file << std::move(output_->str());
    file << std::move("</span>\n");
    file << std::move("</body>\n");
    file << std::move("</html>");
    file.close();
    std::cout << std::move("Zápis prebehol úspešne!\n");
}

void HtmlFileExporter::write_indentation() {
    config_->sh_row_number() ? write_row_number() : void();
    for (int i = 0; i < config_->get_len_left_margin(); ++i) {
        *output_ << std::move("&nbsp;");
    }
    for (int i = 0; i < currentIndentation_; ++i) {
        for (int j = 0; j < config_->get_len_tab_word(); ++j) {
            *output_ << std::move("&nbsp;");
        }
    }
    startedLine_ = true;
}

void HtmlFileExporter::write_row_number() {
    *output_ << std::move("<span class=\"row-num\">");
    if (row_ > 99) {
        *output_ << row_ << std::move(".");
    } else if (row_ > 9) {
        *output_ << std::move("&nbsp;") << row_ << std::move(".");
    } else {
        *output_ << std::move("&nbsp;&nbsp;") << row_ << std::move(".");
    }
    *output_ << std::move("</span>");
}

void HtmlFileExporter::write_new_line() {
    write_word(std::move("<br>"));
    Exporter::write_new_line();
}

void HtmlFileExporter::write_int_type() {
    std::string intword = std::move("<span class=\"int-type\">") + config_->get_int_word()->str() + std::move("</span>");
    write_data_type(std::move(intword));
}

void HtmlFileExporter::write_user_type(std::string usertype) {
    std::string type = std::move("<span class=\"user-type\">") + std::move(usertype) + std::move("</span>");
    write_data_type(std::move(type));
}

void HtmlFileExporter::write_method_name(std::string name) {
    write_word(std::move("<span class=\"method-name\">"));
    write_word(std::move(name));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_class_name(std::string name) {
    write_word(std::move("<span class=\"class-name\">"));
    write_word(std::move(name));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_private_word() {
    write_access_mod(config_->get_private_word()->str());
}

void HtmlFileExporter::write_protected_word() {
    write_access_mod(config_->get_protected_word()->str());
}

void HtmlFileExporter::write_public_word() {
    write_access_mod(config_->get_public_word()->str());
}

void HtmlFileExporter::write_class_word() {
    write_word(std::move("<span class=\"class-word\">"));
    write_word(config_->get_class_word()->str());
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_data_type(std::string datatype) {
    write_word(std::move("<span class=\"data-type\">"));
    write_word(std::move(datatype));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_access_mod(std::string accmod) {
    write_word(std::move("<span class=\"access-mod\">"));
    write_word(std::move(accmod));
    write_word(std::move("</span>"));
}