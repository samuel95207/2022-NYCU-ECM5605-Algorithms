// C++ implementation of randomized quickSelect

#include "stdio.h"
#include "stdlib.h" 
#include <iostream>
#include <climits>
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

// This function returns k'th smallest element in arr[l..r] using
// QuickSort based method. ASSUMPTION: ELEMENTS IN ARR[] ARE DISTINCT
int OrderStatistics(int arr[], int l, int r, int k)
{
    // If k is smaller than number of elements in array
    if (k > 0 && k <= r - l + 1)
    {
        // Partition the array around a random element and
        // get position of pivot element in sorted array
        int pos = randomPartition(arr, l, r);
 
        // If position is same as k
        if (pos-l == k-1)
            return arr[pos];
        if (pos-l > k-1)  // If position is more, recur for left subarray
            return OrderStatistics(arr, l, pos-1, k);
 
        // Else recur for right subarray
        return OrderStatistics(arr, pos+1, r, k-pos+l-1);
    }
 
    // If k is more than number of elements in array
    return INT_MAX;
}
 
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
 
// Standard partition process of QuickSort().  It considers the last
// element as pivot and moves all smaller element to left of it and
// greater elements to right. This function is used by randomPartition()
int partition(int arr[], int l, int r)
{
    int x = arr[r], i = l;
    for (int j = l; j <= r - 1; j++)
    {
        if (arr[j] <= x)
        {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[r]);
    return i;
}
 
// Picks a random pivot element between l and r and partitions
// arr[l..r] arount the randomly picked element using partition()
int randomPartition(int arr[], int l, int r)
{
    int n = r-l+1;
    int pivot = rand() % n;
    swap(&arr[l + pivot], &arr[r]);
    return partition(arr, l, r);
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
  	int num = 0;
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
