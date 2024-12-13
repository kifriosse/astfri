//#include <cstring>
namespace jojoNamespace {

    class AstClass {
    private:
        char* name;
        int cislo_field = 20;
    public:
        AstClass() {
            this->name = new char[100];
        }
        int spocitaj(int a, int b) {
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

}