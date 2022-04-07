/* Dynamic Programming C/C++ implementation of LCS problem */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static long int count = 0;
/* Utility function to get max of 2 integers */
int max(int a, int b) {
  return ((a>b)? a:b);
}  

/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
int lcs(char *X, char *Y, int m, int n) {
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

  printf("Length of LCS(%s,%s) is %d ", X,Y,lcs(X, Y, m, n));
	printf("from %d comparisons.\n", count);

  return 0;
}
