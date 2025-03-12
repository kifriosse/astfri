class Cisla {
private:
    int a = 3;
    int b;
public:
    Cisla(int x, int y);
    int add_funkcia(int a, int b) {
        if (a > b) {
            int cislo = 5 + 23 + 4, y = 123456;
            return a + b + cislo;
        }
        return a;
    }
    void urob_nieco(int nieco = 0) {
        nieco++;
    }
    ~Cisla(){}
};

Cisla::Cisla(int x, int y) : a(x), b(y) {
    int koniec = 1;
};

int main(int argc, char* argv[]) {
    Cisla cisla(3, 4);
    int vysledok = cisla.add_funkcia(1,2);
    int a = 3 + 4;
    a++;
    a = 3 - 4;
    a = 3 * 4;
    a = 3 / 4;
    a *= 3;
    a /= 3;

    a = 3 % 4;
    a %= 4;
    a = 3 >> 4;
    a = 3 << 4;
    a >>= 4;
    a <<= 4;

    a = 3 & 4;
    a &= 4;
    a = 3 | 4;
    a |= 4;
    a = 3 ^ 4;
    a = 4, a;

    if (a < 0 || a > 1) {
        a *= 0;
    } else {
        if (a == 3 && a == 0) {
            if (a != 3) {
                a = 1;
            }
            if (a <= 2 && a >= 2) {
            
            }
        } else {
            a = a++;
            a--;
        }
    }
    return 0;
}