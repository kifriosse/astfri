int global_int = 20;
float global_float = 123.321;
char global_string[] = "Globalny string";
bool global_bool = true;

class Cisla {
private:
    int a = 3;
    int b;
public:
    // Cisla(int x, int y) : a(x), b(y) {}
    Cisla(int x, int y) {
        this->a = x;
        this->b = y;
    }
    int add_funkcia(int a, int b) {
        if (a > b) {
            int cislo = 5 + 23 + 4;
            return a + b + cislo;
        }
        return a;
    }
};

int main(int argc, char* argv[]) {
    Cisla cisla(3, 4);
    int vysledok = cisla.add_funkcia(1,2);
    int a = 3;
    if (a < 0) {
        a = 0;
    } else {
        if (a > 0) {
            a = 1;
        } else {
            a = 2;
            a--;
        }
    }
    return 0;
}

// void switch_skuska() {
//     char pismenko = 'j';
//     switch (pismenko) {
//     case 1: {
//         int nieco = 2;
//         nieco = nieco / 2;
//         }
//         break;
//     //case 'j': {
//     case 2: {
//         int cislo = 0;
//         cislo = cislo * 3;
//         }
//         break;
//     default:
//     break;
//     }
// }

// void loop_skuska() {
//     int a = 0;
//     for (int i = 0; i < 10; i++) {
//         a--;
//         a = a + 3;
//     }
//     while (a < 3) {
//         a++;
//         if (a == 2) {
//             a--;
//         }
//     }
//     do {
//         a++;
//         if (a == 2) {
//             a--;
//         }
//     }
//     while (a < 3);
// }