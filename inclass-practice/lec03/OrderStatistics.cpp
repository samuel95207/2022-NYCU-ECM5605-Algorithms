// C++ implementation of randomized quickSelect

#include "stdio.h"
#include "stdlib.h" 
#include <iostream>
#include<climits>
#include "windows.h"
#include "psapi.h"
#pragma comment(lib, "strmiids.lib") 
#pragma comment(lib, "psapi.lib")


using namespace std;
int randomPartition(int arr[], int l, int r);
 
void printarray(int * a, int n) { 
  for (int i=0; i<n; i++) {
  	cout << a[i] << " ";
  }
}

// This function returns k'th order statistics in arr[l..r] using
// QuickSort based method. ASSUMPTION: ELEMENTS IN ARR[] ARE DISTINCT
int OrderStatistics(int arr[], int l, int r, int k)
{
 	return ;	
}

// Driver program to test above methods
int main(int argc, char** argv ) {
 //int SortType = strtol(argv[1], NULL, 10);
  PROCESS_MEMORY_COUNTERS pmc;

  // your code here
  int n = atoi(argv[1]);
  int iary[n]; 
  srand(0); 	  
  for (int i=0; i<n; i++) {
  	int num;
		while (num==0) {
			num = rand()/100+100;
			for (int j=0; j<i; j++)
				if (iary[j]==num) num = 0;
		} 
  	iary[i] = num;
  }
  printarray(iary, n);
  cout << endl;
  
  int k = atoi(argv[2]);
  cout << k << "-th order statistics = " << OrderStatistics(iary, 0, n-1, k);
  
  //display memory usage 
  GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)); 
  size_t usedMemory = pmc.PeakWorkingSetSize; 
  cout << endl<< "Memory usage: " << usedMemory/1000000 << " Mb\n"; 

  return 0;	
}
