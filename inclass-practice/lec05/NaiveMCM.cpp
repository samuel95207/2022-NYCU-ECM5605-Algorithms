#include <cstdio>
#include <climits>
#include <iostream>

long int nNMC = 0;
// Naive Multiplication Chain (NMC): Matrix Ai has dimension p[i-1] x p[i] for i = 1..n
int NMC(int p[], int i, int j) {
	if (i == j) return 0;
	int min = INT_MAX; //32767 (2^15-1) or greater

	//min comes from one of these parenthesizations i <= k < j
	for (int k = i; k < j; k++) {
		nNMC +=2;
		int count = NMC(p, i, k) + NMC(p, k+1, j) + p[i-1]*p[k]*p[j];
		if (count < min)
			min = count;
	}
	return min;
}

// Driver program to test above function
int main() {
	//int arr[] = {4, 2, 5, 1}; //test 1
	int arr[] = {5, 2, 3, 4, 6, 7, 8}; //test 2
	//int arr[] = {30, 35, 15, 5, 10, 20, 25}; //test 3 in CLRS p.337
	int n = sizeof(arr)/sizeof(arr[0]);
	nNMC++;
	printf("min # of multiplications = %d \n", NMC(arr, 1, n-1));
	printf("# of calls = %d", nNMC);
	return 0;
}
