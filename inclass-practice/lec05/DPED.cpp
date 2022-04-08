/* Dynamic Programming C/C++ implementation of Edit-Distance problem */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static long int count = 0;
/* Utility function to get max of 2 integers */
int min(int a, int b) {
	return (a > b)?  b: a;
}

/* Returns length of ED for X[1..m], Y[1..n] */
int ed(char *X, char *Y, int m, int n) {
//write your own code below


}

/* Driver program to test above function */
int main() {
	char X[] = "BDCAB";
	char Y[] = "ABCB";
	//char X[] = "INTHEBEGINNING";
	//char Y[] = "ALLTHEINGSARELOST";
	int m = strlen(X);
	int n = strlen(Y);

	printf("Length of Edit-Distance(%s,%s) is %d ", X, Y, ed(X, Y, m, n));
	printf("from %d comparisons.\n", count);

	return 0;
}
