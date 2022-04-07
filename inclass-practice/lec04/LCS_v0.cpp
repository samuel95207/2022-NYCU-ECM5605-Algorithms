/* Naive implementation in O(2^n) of LCS problem: no memory, just recursive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static long int count = 0;
char* substr(char *x, int len, int tgt) {
	char* z = (char*)malloc(len*sizeof(char));
	strcpy(z, "");
	int j = 0;
	for (int i=0; i<len; i++) {
		if (tgt & 1<<i) {
			z[j++] = x[i];
		}
	}
	z[j++]='\0';
	return z;
}

int lcs(char *A, int a, char * B, int b) {
		int len=0;
		for (int i=0,j=0;i<a;i++) {
			if (A[i]==B[j]) {
				len++; j++; 
			}
			count++;				
		} 
		return len;
}

/*Driver program to test printPowerSet*/
int main() {
	//char X[] = "BDCAB";
	//char Y[] = "ABCB";
	char Y[] = "INTHEBEGINNING";
	char X[] = "ALLTHEINGSARELOST";
	int m = strlen(X);
	int n = strlen(Y);
	unsigned int size = (int)pow(2, m);
	int max = 0;
	for (int i=1; i<size; i++) {
		char *Z = substr(X,m,i);
		int l = strlen(Z);
		//printf("%s vs %s \n", Z, Y);
		int cur = lcs(Y,n,Z,l);
		if (cur>max) max = cur;
		free(Z);
	}
	printf("Length of LCS(%s,%s) is %d ", X,Y,max);
	printf("from %d comparisons.\n", count);

	return 0;
}

