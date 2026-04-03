class Animal {
public:
    int age;
    
    virtual void make_sound() {
    }

    void sleep() {
        age += 1;
    }
};

// Dedičnosť
class Dog : public Animal {
public:
    int breed_id;

    // Preťažená metóda
    void make_sound() override {
        int volume = 10;
    }

    void bark() {
        make_sound();
    }
};

class Robot {
public:
    void charge() {}
};

// Viacnásobná dedičnosť
class RoboDog : public Dog, public Robot {
public:
    void perform_task() {
        charge();
        bark();
    }
};