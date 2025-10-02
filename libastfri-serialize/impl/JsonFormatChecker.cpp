#include <libastfri-serialize/inc/JsonFormatChecker.hpp>

JsonFormatChecker& JsonFormatChecker::get_instance(){
    static JsonFormatChecker instance;
    return instance;
}

