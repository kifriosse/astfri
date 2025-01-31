#include <libastfri-text/inc/Configurator.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>

struct Exporter {
private:
    const Configurator* config_;
    std::stringstream* output_;
    int currentIndentation_;
    bool startedLine_;
public:
    Exporter(const Configurator* conf, std::stringstream* output) {
        config_ = conf;
        output_ = output;
        currentIndentation_ = 0;
        startedLine_ = false;
    };
    virtual ~Exporter() {
        config_ = nullptr;
        output_ = nullptr;
    };
    virtual void make_export() = 0;
    std::stringstream* get_output() { return output_; };
    void increase_indentation() { ++currentIndentation_; };
    void decrease_indentation() { --currentIndentation_; };
    void write_word(const std::string ss, bool newLine = false) {
        if (!startedLine_) {
            write_indentation();
        }
        (*output_) << ss;
        if (newLine) {
            startedLine_ = false;
        }
    };
    void write_indentation() {
        for (int i = 0; i < currentIndentation_; ++i) {
            (*output_) << config_->get_tab_word()->str();
            startedLine_ = true;
        }
    };
    virtual void write_int_type() {
        write_word(config_->get_int_word()->str());
    };
};

struct TxtFileExporter : Exporter {
    TxtFileExporter(const Configurator* conf, std::stringstream* output) : Exporter(conf, output) {};
    void make_export() override {
        namespace fs = std::filesystem;
        std::string userInput;
        std::cout << "Zadaj názov súboru bez prípony: ";
        std::getline(std::cin, userInput);
        userInput.append(".txt");
        fs::path filePath = fs::current_path().parent_path().parent_path() / userInput;
        std::ofstream file(filePath);
        if (file) {
            file << get_output()->str();
            std::cout << "Zápis prebehol úspešne!\n";
        }
        file.close();
    };
};

struct RtfFileExporter : Exporter {
    RtfFileExporter(const Configurator* conf, std::stringstream* output) : Exporter(conf, output) {};
    void make_export() override {};
};