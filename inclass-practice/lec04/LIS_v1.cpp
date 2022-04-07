//A: array of number
//Table: array of int. keep LIS of every position
//n: length of array "A"
//Time Complexity: O(n^2)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static long int count = 0;
int LIS(int *A, int *T, int n) {
	for (int i=0; i<n; i++) {
		T[i] = 1;
	}
	/*
	for (int i=0; i<n; i++) {   // find if s[i] can be appended before s[j]
		for (int j=i+1; j<n; j++) { //only j=i+1..n
			count++;
			if (a[i] < a[j]) {
				Table[j] = (Table[j]>Table[i]+1)? Table[j]: Table[i]+1;
			}
		}
	}
	*/
	for (int i=0; i<n; i++) {
		for (int j=0; j<i; j++) {
			if (A[i]>A[j]) {
				if (T[j]>=T[i])
					T[i] = T[j]+1;
				count++;	
			}
		}
	}
	//get LIS value from Table
	int len=0;
	for (int i=0; i<n; i++) {
		if (T[i] > len)
			len = T[i];
	}
	return len;
}

int main() {
	int n = 11;
	int A[11] = {10, 22, 9, 33, 21, 50, 41, 60, 80, 12, 11};
	int T[11] = {0};

	printf("Length of LIS is %d ", LIS(A, T, n));
	printf("from %d comparisons.\n", count);

	return 0;
}
