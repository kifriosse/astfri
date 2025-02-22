#ifndef LIBASTFRI_TEXT_INC_HTML_EXPORTER
#define LIBASTFRI_TEXT_INC_HTML_EXPORTER

#include <libastfri-text/inc/Exporter.hpp>

struct HtmlFileExporter : Exporter {
    HtmlFileExporter(std::shared_ptr<TextConfigurator> conf);
    void make_export() override;
private:
    void write_file(std::string path) override;
    void write_indentation() override;
    void write_row_number() override;
public:
    void write_new_line() override;
    void write_int_type() override;
    void write_user_type(std::string usertype) override;
    void write_method_name(std::string name) override;
    void write_class_name(std::string name) override;
    void write_private_word() override;
    void write_protected_word() override;
    void write_public_word() override;
    void write_class_word() override;
private:
    void write_data_type(std::string datatype);
    void write_access_mod(std::string accmod);
};

#endif