#pragma once
#include <sstream>

struct Configurator {
private:
    const std::stringstream* tabWord_;
    const std::stringstream* intWord_;
public:
    Configurator(const std::string& configFileName);
    ~Configurator();
    const std::stringstream* get_tab_word() const { return tabWord_; };
    const std::stringstream* get_int_word() const { return intWord_; };
};