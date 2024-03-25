int power(int a, int b) {
    int wynik = 1;
    for (int i = 0; i < b; i++) {
        wynik *= a;
    }
    return wynik;
}