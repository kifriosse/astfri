class Auto
{
private:
    bool is_good();

protected:
    int cost_;
    float speed_;

public:
    Auto(int cost, float speed) :
        cost_(cost),
        speed_(speed) { };
    virtual void ride(); // neskor testnut na = 0
};

void Auto::ride()
{
    int limit = 10;
    for (int i = 0; i < limit; i++)
    {
        limit++;
    }
};

bool Auto::is_good()
{
    return true;
}

// po tialto vsetko dobre

class Audi : public Auto
{
private:
    int nieco_private_void();
public:
    Audi(int cost) :
        Auto(cost, 200.64)
    {
        this->cost_ = this->cost_ >> 2;
        int nieco   = 0;
    };

    void ride() override;
};

int Audi::nieco_private_void()
{
    return 5;
}

// Audi::Audi(int cost) : Auto(cost){
//     this->cost_ = this->cost_ * 2;
//     int nieco = 0;
// };

void Audi::ride()
{
    int time = 300;
    for (int i = 0; i < time; i++)
    {
        time--;
        time++;
    }
}

class Chrysler : public Auto
{
private:
    int time_;

public:
    Chrysler(float speed) :
        Auto(25000, speed)
    {
        this->time_ = 500;
    };

    void ride() override
    {
        while (this->time_ != 0)
        {
            this->time_--;
        }
    };

    int* return_intptr()
    {
        return nullptr;
    }
};

int main()
{
    Chrysler chr(130.128);
    Audi* au = new Audi(50000);
    chr.ride();
    au->ride();

    delete au;
    return 0;
}