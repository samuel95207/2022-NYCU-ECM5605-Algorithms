#include "stdio.h"
#include "stdlib.h" 
#include <iostream>
#include "windows.h"
#include "psapi.h"
#pragma comment(lib, "strmiids.lib") 
#pragma comment(lib, "psapi.lib")

#define SIZE 10

using namespace std;
void printarray(int * a, int n) { 
  for (int i=0; i<n; i++) {
  	cout << a[i] << " ";
  }
}
//type 1: Insertion Sort
void insertsort(int* ary, int l, int r) {

}

//type 2: Merge Sort
void mergesort(int *ary, int l, int r) {

}

//type 3: Heap Sort
void heapify(int *a, int i, int max) {

}
void heapsort(int *a, int n) {

}
//type 4: Quick Sort 
void quicksort(int *a, int l, int r) {

}
//type 5: Median Sort 
void mediansort(int *a, int l, int r) {

}


int MySort(int* ary, int size, int type) {
  //Enter your code here
  if (type==1) {   //insertion sort
    insertsort(ary,0,size-1); //arguments are array, left and right (not size)
  } if (type==2) { //merge sort
	  mergesort(ary,0,size-1); //arguments are array, left and right (not size)
	} if (type==3) { //heap sort
		heapsort(ary, size);
  } if (type==4) { //quick sort
	  quicksort(ary,0,size-1); //array, left and right (not size)
	} if (type==5) { //median sort
	  mediansort(ary,0,size-1); //array, left and right (not size)
	} else {         //do nothing
  	
  }
  return 0;
}

int main(int argc, char *argv[]) {
  //int SortType = strtol(argv[1], NULL, 10);
  PROCESS_MEMORY_COUNTERS pmc;

  // your code here
  int i;
  int iary[SIZE]; 
  srand(0); 	  
  for (i=0; i<SIZE; i++) {
  	iary[i] = rand()/100+100;
  }
  
  //for debug only
  cout << "Before sorting:\t"; 
  printarray(iary, SIZE);
  cout << endl;
  
  //call your sort funciton
  MySort(iary, SIZE, 5);
   
  //for debug only
  cout << "After sorting:\t"; 
  printarray(iary, SIZE);
  cout << endl;
  
  //display memory usage 
  GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)); 
  size_t usedMemory = pmc.PeakWorkingSetSize; 
  cout << "Memory usage: " << usedMemory/1000000 << " Mb\n"; 

  return 0;	
}
