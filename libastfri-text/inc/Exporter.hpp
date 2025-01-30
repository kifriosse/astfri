#include "Configurator.hpp"

struct Exporter {
protected:
    const Configurator* config_;
    std::stringstream* output_;
private:
    int currentIndentation_;
    bool startedLine_;
public:
    Exporter(const Configurator* conf, std::stringstream* output) {
        config_ = conf;
        output_ = output;
        currentIndentation_ = 0;
        startedLine_ = false;
    };
    virtual ~Exporter() = default;
    virtual void make_export() = 0;
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
    void make_export() override {};
};

struct RtfFileExporter : Exporter {
    RtfFileExporter(const Configurator* conf, std::stringstream* output) : Exporter(conf, output) {};
    void make_export() override {};
};