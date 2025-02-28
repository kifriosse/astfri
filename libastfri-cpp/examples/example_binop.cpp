int global_int = 20;
float global_float = 123.321;
char global_string[] = "Globalny string";
bool global_bool = true;

class Cisla {
private:
    int a = 3;
    int b;
public:
    // int add_funkcia(int a, int b) {
    //     if (a > b) {
    //         int cislo = 5 + 23 + 4;
    //         return a + b + cislo;
    //     }
    //     return a;
    // }
};

int main(int argc, char* argv[]) {
    Cisla cisla;
    int a = 3;
    if (a < 0) {
        a = 0;
    } else {
        if (a > 0) {
            a = 1;
        } else {
            a = 2;
        }
    }
    return 0;
}

void switch_skuska() {
    char pismenko = 'j';
    // switch (pismenko) {
    // case 0: {
        
    //     }
    //     break;
    // case 'j': {
        
    //     }
    //     break;
    // default:
    // break;
    // }
}