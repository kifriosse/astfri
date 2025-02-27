#include<libastfri-serialize/inc/JsonFormatChecker.hpp>

JsonFormatChecker& JsonFormatChecker::getInstance(){
    static JsonFormatChecker instance;
    return instance;
}
