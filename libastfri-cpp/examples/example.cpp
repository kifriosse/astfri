namespace jojoNamespace {

    class AstClass {
    private:
        const char* name = "Name";
        int cislo_field = 20;
    public:
        int spocitaj(int a, int b) {
            return a + b;
        }
        int getCislo() {
            return cislo_field;
        }
        const char* getName() {
            return "Jozef";
        }
    };

}