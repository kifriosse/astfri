#include <iostream>

void repeatMessage(int count, const char* message) {
    if(count <= 0) {
        return;
    }
    for(int i = 0; i < count; i++) {
        std::cout << message << std::endl;
    }
}