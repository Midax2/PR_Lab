#include <stdio.h>
int main() {
	printf("Power of 12 is %d\n", power(12));
	printf("Sqrt of 345 is %d\n", mySqrt(345));
	printf("Number of divisors of 256 is %d\n", countDivisors(256));
	if (isPrime(547)) printf("547 is a prime number\n");
	else printf("547 is not a prime number\n");
	if (isPrime(7)) printf("7 is a prime number\n");
	else printf("7 is not a prime number\n");
	return 0;
}