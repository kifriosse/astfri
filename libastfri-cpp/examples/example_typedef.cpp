template<typename T, typename X>
class Cislo
{
private:
    T jedna;
    X dva;
    int come_with_me;

public:
    int cislo = 0;

    Cislo(T jedna, X dva) :
        jedna(jedna),
        dva(dva)
    {
        int nieco = 0;
    }

    int if_you_wanna_live()
    {
        this->cislo = 3;
        this->TeeOneOhOne(this->come_with_me);
        return this->come_with_me;
    };

    void TeeOneOhOne(int arg);
};

template<typename T, typename X>
void Cislo<T, X>::TeeOneOhOne(int arg)
{
    Cislo<int, int> c(0, 0);
    c.TeeOneOhOne(3);
    c.cislo      = 2;
    come_with_me = arg;
}

void Funkcia_len_tak(int nieco);

class Cele_cislo : public Cislo<int, int>
{
public:
    Cele_cislo(int ake, int dalsie) :
        cele_cislo_public_field(5),
        Cislo<int, int>(ake, dalsie)
    {
    }

    int cele_cislo_public_field;
};

int main()
{
    Cislo<int, float> cislo(0, 0);
    cislo.TeeOneOhOne(5);
    int nieco = cislo.if_you_wanna_live();
    Funkcia_len_tak(5);
    Cele_cislo cc(4, 5);
    cc.cele_cislo_public_field = 4;
    cc.cislo                   = 6;
    return 0;
}

void Funkcia_len_tak(int nieco)
{
    int nic = 0;
}