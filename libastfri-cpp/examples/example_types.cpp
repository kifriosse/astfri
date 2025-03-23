int* funkcia_co_vracia_intptr() {
    return nullptr;
}
int funkcia_co_vracia_int() {
    return 0;
}

bool* global_bool_ptr;
int* global_int_ptr;
float** global_float_ptr;


int global_int = 100;
float global_float = 100.123;
char global_char = 'J';
const char* global_string = "Jojo";

class Trieda {
public:
    Trieda(int* pCislo) : cislo(*pCislo){};
    int cislo;
};


int main(int argc, char** argv) {
    Trieda** trieda_ptr;
    int* pointer_int;
    float* pointer_float;
    char* pointer_char;
    const char* pointer_string = "Jozef";
    return 0;
}