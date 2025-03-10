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