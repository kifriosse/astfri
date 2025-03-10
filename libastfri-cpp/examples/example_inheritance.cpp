class Auto {
protected:
    int cost_;
public:
    Auto(int cost) : cost_(cost) {};
    virtual void ride() {}; // neskor testnut na = 0
};

class Audi : public Auto {
public:
    Audi() : Auto(500){
        // this->cost_ = this->cost_ * 2;
        int nieco = 0;
    };
    void ride() override {
        int time = 300;
        for (int i = 0; i < time; i++) {
            time--;
            time++;
        }
    }
};

class Chrysler : public Auto {
private:
    int time_;
public:
    Chrysler() : Auto(1500) {
        this->time_ = 500;
    }
    void ride() override {
        while (this->time_ != 0) {
            this->time_--;
        }
    }
};