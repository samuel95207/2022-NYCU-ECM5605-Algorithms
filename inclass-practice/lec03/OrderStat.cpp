#include <cstdio>
#include <cstdlib> 
#include <iostream>
#include <climits>
using namespace std;

int SIZE;
 
void printarray(int * a, int n) { 
  for (int i=0; i<n; i++) {
  	cout << a[i] << " ";
  }
}

//write your own code here!!!
int OrderStat(int arr[], int l, int r, int k) {




}

// Driver program to test above methods
int main(int argc, char** argv ) {
  // your code here
  SIZE = atoi(argv[1]);
  // your code here
  int i;
  int* iary = new int[SIZE]; 
  srand(100); 	  
  for (i=0; i<SIZE; i++) {
  	iary[i] = rand()/1000000;
  }
  printarray(iary, SIZE);
  cout << endl;
  
  int k = atoi(argv[2]);
  cout << k << "-th order statistics = " 
	   << OrderStat(iary, 0, SIZE-1, k) << endl;  
  return 0;	
}
