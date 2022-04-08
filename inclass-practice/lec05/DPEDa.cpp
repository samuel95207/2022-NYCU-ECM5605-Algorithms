/* Dynamic Programming C/C++ implementation of LCS problem */
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
	int D[m+1][n+1];
	int i, j;
	for (i=0; i<=m; i++) {
		for (j=0; j<=n; j++) {
			count++;
			if (i == 0)					// insert all y_j's
				D[i][j] = j;
			else if (j == 0) 			// delete all x_i's
				D[i][j] = i;
			else if (X[i-1] == Y[j-1]) 	// same character => no cost
				D[i][j] = D[i-1][j-1];
			else 						// x_i and y_j are different
				D[i][j] = 1+min(min(D[i-1][j], D[i][j-1]), D[i-1][j-1]);
		}
	}
	return D[m][n];
}

/* Driver program to test above function */
int main() {
	char X[] = "BDCAB";
	char Y[] = "ABCB";
	//char X[] = "INTHEBEGINNING";
	//char Y[] = "ALLTHEINGSARELOST";
	int m = strlen(X);
	int n = strlen(Y);

	printf("Length of EditDistance(%s,%s) is %d ", X, Y, ed(X, Y, m, n));
	printf("from %d comparisons.\n", count);

	return 0;
}
