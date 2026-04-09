// Testovací kód pre lambda funkcie

int main() {
    int x = 10;
    int y = 20;

    // 1. Najjednoduchšia lambda bez zachytávania (capture)
    auto hello = []() {
        int a = 1;
    };
    hello();

    // 2. Lambda so zachytávaním hodnotou [=]
    auto captureValue = [x, y]() mutable {
        x += 1; // mutable umožňuje meniť kópie zachytených hodnôt
        return x + y;
    };

    // 3. Lambda so zachytávaním referenciou [&]
    auto captureRef = [&x](int pridaj) {
        x += pridaj;
    };
    captureRef(5);

    // 4. Lambda s explicitným návratovým typom
    auto sNavratom = [](double d) -> int {
        return (int)d * 2;
    };

    // 5. Generická lambda (C++14) - používa 'auto' v parametroch
    auto generic = [](auto a, auto b) {
        return a + b;
    };

    int vysledok = generic(x, 5);
    
    return 0;
}