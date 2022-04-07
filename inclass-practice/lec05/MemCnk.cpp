#include <cstdio>
#include <climits>
#include <iostream>

using namespace std;
long int count = 0;

int LUC(int **m, int i, int j) {
	if (m[i][j] < INT_MAX) return m[i][j];
	if (j==0 || j==i) {
		m[i][j] = 1;
		count++;
		return m[i][j];
	}
	count +=2; 
	return LUC(m, i-1, j-1) + LUC(m, i-1, j);
}

int memBinomial(int **m, int n, int k) {
	count +=2;
	return  LUC(m, n-1, k-1) + LUC(m, n-1, k);
}

/* Driver program to test above function*/
int main() {
	//int n = 5, k = 2; //test 1
	int n = 30, k = 20; //test 2
	int **m = new int*[n];
	for (int i=0; i<n; ++i)
		m[i] = new int[k+1];
	for (int i=0; i<n; ++i)
		for (int j=0; j<=k; ++j)
			m[i][j] = INT_MAX;
	count++; 
	cout << "C(" << n << ", " << k << ") = " << memBinomial(m, n, k) << endl;
	cout << "Using " << count << " calls";
	return 0;
}
