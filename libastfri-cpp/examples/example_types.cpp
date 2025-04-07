#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <string>
#include <vector>

namespace example_types {

int* funkcia_co_vracia_intptr() {
    return nullptr;
}
int funkcia_co_vracia_int() {
    return 0;
}

bool* global_bool_ptr;
int* global_int_ptr;
float** global_float_ptr;

std::string string = "toto je string";
std::vector<int> cisla = {0, 1, 2};

int global_int = 100;
float global_float = 100.123;
char global_char = 'J';
const char* global_string = "Jojo";

class Trieda {
public:
    Trieda(int* pCislo) : cislo(*pCislo){};
    Trieda(float* pCislo) : cislo(*pCislo){
        const char* konstruktor2 = "Toto je naozaj druhy konstruktor\n";
    };
    int cislo;
    ~Trieda(){
        int* smernik_na_nic_mozno = &this->cislo;
        this->cislo = 0;
        delete smernik_na_nic_mozno;
        std::string string;
        string = "toto je string";
        std::string string2 = "toto je string2";
        std::vector<int> cisla;
        cisla.push_back(0);
    };
};


int main(int argc, char** argv) {
    Trieda** trieda_ptr;
    int* pointer_int;
    float* pointer_float;
    char* pointer_char;
    const char* pointer_string = "Jozef";
    int* haldovy_int = new int(5);
    delete haldovy_int;
    return 0;
}

};