#include <stdio.h>
int main() {
	double arr[] = { 4.3, 12.3, 6.0, 2.7, 2.9, 8.4 };
	int n = 6;
	printf("Fibonaci(12) is %d\n", fib(12));
	printf("Factorial of 6 is %d\n", factorial(6));
	printf("Average of array = {4.3, 12.3, 6.0, 2.7, 2.9, 8.4} is %f\n", avg(arr, n));
	printf("Standard deviation of array = {4.3, 12.3, 6.0, 2.7, 2.9, 8.4} is %f\n", std(arr, n));
	return 0;
}