int mySqrt(int a) {
    if (a <= 1) return a;

    int sqrt = 1;
    while (sqrt * sqrt <= a) {
        sqrt++;
    }
    return sqrt - 1;
}