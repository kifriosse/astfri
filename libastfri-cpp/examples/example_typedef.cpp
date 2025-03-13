template <typename T, typename X>
class Cislo {
private:
    int come_with_me;
public:
    Cislo() {}
    int if_you_wanna_live() {
        this->TeeOneOhOne(this->come_with_me);
        return this->come_with_me;
    };
    void TeeOneOhOne(int arg) {
        this->come_with_me = arg;
    }
};

class Cislo_bez_T {
public:
    int cislo;
};

int main() {
    Cislo<int, float> cislo;
    cislo.TeeOneOhOne(5);
    int nieco = cislo.if_you_wanna_live();
    return 0;
}