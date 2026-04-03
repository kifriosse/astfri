int main() {
    // 1. Aritmetické operátory (BinaryOperator)
    int a = 10;
    int b = 3;
    int c = a * b + (a - b) / 2;
    int d = a % b;

    // 2. Relačné a logické operátory
    bool condition = (a > b) && (a != 0) || !(b < 0);

    // 3. Bitové operátory
    int bitwise = (a << 1) | (b >> 1) ^ 0xFF;
    bitwise &= 0x0F;

    // 4. Unárne operátory (UnaryOperator)
    a++;
    --b;
    int* ptr = &a;
    int val = *ptr;

    // 5. Ternárny operátor (ConditionalOperator)
    int max = (a > b) ? a : b;

    // 7. Zložené priradenia
    int x = 5;
    x += 10;
    x *= 2;

    return 0;
}