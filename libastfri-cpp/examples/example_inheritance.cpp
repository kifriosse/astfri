class Auto {
private:
    bool is_good();
protected:
    int cost_;
public:
    Auto(int cost) : cost_(cost) {};
    virtual void ride() {}; // neskor testnut na = 0
};

bool Auto::is_good() {
    return true;
}

class Audi : public Auto {
public:
    Audi(int cost);
    void ride() override;
};

Audi::Audi(int cost) : Auto(cost){
    this->cost_ = this->cost_ * 2;
    int nieco = 0;
};

void Audi::ride() {
    int time = 300;
    for (int i = 0; i < time; i++) {
        time--;
        time++;
    }
}

class Chrysler : public Auto {
private:
    int time_;
public:
    Chrysler(int cost) : Auto(cost) {
        this->time_ = 500;
    }
    void ride() override;
};

void Chrysler::ride() {
    while (this->time_ != 0) {
        this->time_--;
    }
}

int main() {
    Chrysler chr(500);
    Audi au(1500);
    chr.ride();
    au.ride();
    return 0;
}