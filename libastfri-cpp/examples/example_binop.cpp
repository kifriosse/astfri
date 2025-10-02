bool global_bool = true;
char global_char = 'J';

class Cisla
{
private:
    int a = 3;
    int b;

public:
    Cisla(int x, int y);
    int add_funkcia(int a, int b);
    void urob_nieco(int nieco = 0);
    char get_char();
    ~Cisla();
};

Cisla::Cisla(int x, int y) :
    a(x),
    b(y)
{
    int koniec = 1;
};

Cisla::~Cisla() { };

void Cisla::urob_nieco(int nieco)
{
    nieco++;
};

int Cisla::add_funkcia(int a, int b)
{
    if (a > b)
    {
        int cislo = 5 + 23 + 4, y = 123456;
        return a + b + cislo;
    }
    switch (a + b)
    {
    case 0:
    {
        return 100;
    }
    break;
    case 1:
    {
        return 101;
    }
    break;
    case 2:
    {
        return 102;
    }
    break;
    case 5:
    {
        return 'o';
    }
    break;
    default:
        return a - b;
        break;
    }
    return a;
}

char Cisla::get_char()
{
    return 'j';
}

int main(int argc, char* argv[])
{
    Cisla cisla(3, 4);
    int vysledok = cisla.add_funkcia(1, 2);
    int a        = 3 + 4;
    a++;
    a = 3 - 4;
    a = 3 * 4;
    a = 3 / 4;
    a *= 3;
    a /= 3;

    a = 3 % 4;
    a %= 4;
    a = 3 >> 4;
    a = 3 << 4;
    a >>= 4;
    a <<= 4;

    a = 3 & 4;
    a &= 4;
    a = 3 | 4;
    a |= 4;
    a = 3 ^ 4;
    a = 4, a;

    throw 6;

    if (a < 0 || a > 1)
    {
        a *= 0;
    }
    else
    {
        if (a == 3 && a == 0)
        {
            if (a != 3)
            {
                a = 1;
            }
            if (a <= 2 && a >= 2)
            {
                cisla.urob_nieco();
            }
        }
        else
        {
            a = a++;
            a--;
        }
    }
    return 0;
}