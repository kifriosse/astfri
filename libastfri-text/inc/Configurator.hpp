#include <sstream>

struct Configurator {
private:
    const std::stringstream* tabWord_;
    const std::stringstream* intWord_;
public:
    Configurator() {
        tabWord_ = new std::stringstream("     ");
        intWord_ = new std::stringstream("INTEGER");
    };
    ~Configurator() {
        delete tabWord_;
        delete intWord_;
    };
    const std::stringstream* get_tab_word() const { return tabWord_; };
    const std::stringstream* get_int_word() const { return intWord_; };
};