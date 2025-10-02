int main()
{
    char x = 'x';
    while (true)
    {
        x++;
        if (x % 2 == 0)
        {
            break;
        }
        else
        {
            continue;
        }
    }

    int koniec = 100;
    for (int i = 0; i < koniec; i++)
    {
        koniec = koniec << 3;
        if (koniec > 50)
        {
            break;
        }
        continue;
    }

    do
    {
        char slovo[] = "Ahoj";
        if (slovo[2] == 'a')
        {
            break;
        }
        if (slovo[0] == 'A')
        {
            continue;
        }
    } while (false);
}