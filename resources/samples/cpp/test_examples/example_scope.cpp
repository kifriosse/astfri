float global_float;

namespace first {
    char first_char;
    
    namespace second {
        int second_int;
        class SecondTrieda {
            public:
            int cislo;
            SecondTrieda(int param) {
                char constructor_char;
                this->cislo = param;
            }
            private:
            int druhe_cislo;
        };

    } // namespace second
} // namespace first