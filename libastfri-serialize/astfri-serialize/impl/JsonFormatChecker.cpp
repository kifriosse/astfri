#include <astfri-serialize/JsonFormatChecker.hpp>

JsonFormatChecker& JsonFormatChecker::get_instance() {
    static JsonFormatChecker instance;
    return instance;
}
