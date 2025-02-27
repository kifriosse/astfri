int global = 20;

class MyClass {
    public:
        MyClass(int initialValue);
        void setValue(int newValue);
        int getValue() const;
        bool isPositive() const;
    
    private:
        int value = 20;
    };
    
    // Definícia konštruktora
    MyClass::MyClass(int initialValue) : value(initialValue) {}
    
    // Metóda na nastavenie novej hodnoty s `if` podmienkou
    void MyClass::setValue(int newValue) {
        if (newValue >= 0) {
            value = newValue;  // Priradenie (BinaryOperator)
        } else {
            value = 0;  // Ak je hodnota záporná, nastavíme 0
        }
    }
    
    // Metóda na získanie hodnoty
    int MyClass::getValue() const {
        return value;
    }
    
    // Metóda, ktorá kontroluje, či je hodnota kladná
    bool MyClass::isPositive() const {
        if (value > 0) {
            return true;
        } else if (value == 0) {
            return false;
        }
        return false;
    }

    void metodaOdVeci(void* nieco, int druhy) {
        nieco = nieco;
    }
    
    // Hlavná funkcia
    int main() {
        MyClass* obj = new MyClass(-5);
        obj->setValue(10); 
        obj->setValue(-3);
        delete obj;
        return 0;
    }
    