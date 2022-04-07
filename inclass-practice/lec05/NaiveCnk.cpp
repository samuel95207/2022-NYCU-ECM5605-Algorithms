#include <cstdio>
#include <climits>
#include <iostream>

using namespace std;
long int count = 0;

//Binomial Coefficient C(n, k) = C(n-1, k-1) + C(n-1, k)
int binomialCoeff(int n, int k) {
	if (k==0 || k==n) { // Base Cases
		count++;
		return 1;
	}
	count +=2;
	return  binomialCoeff(n-1, k-1) + binomialCoeff(n-1, k);
}

/* Driver program to test above function*/
int main() {
	//int n = 5, k = 2; //test 1
	int n = 30, k = 20; //test 2
	count++; 
	cout << "C(" << n << ", " << k << ") = " << binomialCoeff(n, k) << endl;
	cout << "Using " << count << " calls";
	return 0;
}
