//#include <cstring>
namespace jojoNamespace {
    
    int global_int = 20;
    float global_float = 123.321;
    long global_long = 12345678900000000;
    char global_string[] = "Globalny string";

    class AstClass {
    private:
        char* name;
        int cislo_field = 20;
    public:
        AstClass(int parmConst) {
            this->name = new char[100];
            this->cislo_field = parmConst;
        }
        int spocitaj(int a, int b = 5) {
            return a + b;
        }
        int getCislo() {
            return cislo_field;
        }
        const char* getName() {
            return "Jozef";
        }
        void setName(char* name) {
            this->name = name;
        }
        void loop() {
            int a = 0;
            for (int i = 0; i < 10; i++) {
                a++;
            }
        }
        ~AstClass() {
            delete name;
        }
    };
    
    void metodaOdVeci2(void* nieco) {
        nieco = nieco;
    }
    int metodaOdVeci() {
        char easterEgg[] = "Toto nieje easter egg";
        long java = 3405691581; // Toto uz je easter egg
        java++;
        metodaOdVeci2(easterEgg);
        metodaOdVeci2(&java);
        return 0;
    }
    
    int main(int argc, char* argv[]) {
        int cislo;
        cislo = 0;
        for(int i = 0; i < 10; i++) {
            cislo = cislo + i;
            if (cislo > 5) {
                int nieco_len_tak = 0;
                metodaOdVeci2(&cislo);
            }
            while(cislo < 10) {
                cislo = cislo + 1;
            }
        }
        return 0;
    }

}