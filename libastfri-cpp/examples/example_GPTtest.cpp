// Triedy a dedičnosť
class Base {
    public:
        virtual void speak() { }
        virtual ~Base() {}
    };
    
    class Derived : public Base {
    public:
        void speak() override { }
    };
    
    // Generická trieda a šablóna funkcie
    template <typename T>
    class Box {
        T value;
    public:
        Box(T val) : value(val) {}
        T get() { return value; }
    };
    
    template <typename T>
    T add(T a, T b) { return a + b; }
    
    // Operátory
    struct Number {
        int value;
        
        Number(int v) : value(v) {}
    
        // Binárne operátory
        //Number operator+(const Number* other) { return Number(value + other->value); }
        //Number operator-(const Number* other) { return Number(value - other->value); }
    
        // Unárne operátory
        //Number operator-() { return Number(-value); }
        //Number& operator++() { ++value; return *this; }
    };
    
    // Enum a union
    // enum Color { RED, GREEN, BLUE };
    
    // union Data {
    //     int i;
    //     float f;
    // };
    
    // Funkcia s rôznymi parametrami
    void process(int a, float b, char c, int* ptr) { }
    
    // Lambda výraz
    //auto lambda = [](int x) { return x * x; };
    
    // Hlavná funkcia s rôznymi štruktúrami jazyka
    int main() {
        // Premenné rôznych typov
        int a = 5, b = 10;
        float f = 3.14;
        char ch = 'A';
        bool flag = true;
    
        // Operácie a operátory
        a += b;
        f /= 2.0f;
        ch = 'B' + 1;
        bool test = (a > b) && flag;
    
        // Polia a ukazovatele
        //int arr[3] = {1, 2, 3};
        int* ptr = nullptr;
    
        // Dynamická alokácia
        int* dynArr = new int[5];
        delete[] dynArr;
    
        // Štruktúry a triedy
        Derived obj;
        obj.speak();
    
        Box<double> box(2.5);
        double val = box.get();
    
        // Použitie enum a union
        // Color color = GREEN;
        // Data data;
        // data.i = 42;
    
        // Cyklus for, while, do-while
        for (int i = 0; i < 5; ++i) { }
        int counter = 0;
        while (counter < 3) { ++counter; }
        do { --counter; } while (counter > 0);
    
        int number = 5;

        // Prepínač (switch)
        switch (number) {
            case 0: break;
            case 1: break;
            case 2: break;
            default: break;
        }
    
        // Volanie funkcie a lambda výrazu
        process(a, f, ch, &b);
        int square = 123; // lambda(4);
    
        return 0;
    }
    