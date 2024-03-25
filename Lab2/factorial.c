unsigned long long factorial(int n) {
    unsigned long long wynik = 1;
    for (int i = 2; i <= n; ++i) {
        wynik *= i;
    }
    return wynik;
}