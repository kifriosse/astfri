
void nieco() {
    int intDecl;
    auto lam = [&intDecl](int a) -> bool {
        a = a + 3;
        intDecl = 0;
        return true;
    };
    lam(intDecl);
}
