#include "stdio.h"
#include "stdlib.h" 
#include <iostream>
//#include "windows.h"
//#include "psapi.h"
//#pragma comment(lib, "strmiids.lib") 
//#pragma comment(lib, "psapi.lib")

#define SIZE 10

using namespace std;
void printarray(int * a, int n) { 
  for (int i=0; i<n; i++) {
  	cout << a[i] << " ";
  }
}

//type 1: Insertion Sort
void insertsort(int* ary, int l, int r) {
	for (int i=l; i<(r-l+1); i++) {
	  int key = ary[i];
	  int j = i-1;
	  while (j>=0 && key<ary[j]) {
	    ary[j+1] = ary[j];
		  j--;		
	  }
	  ary[j+1] = key;
	}
}

//type 2: Merge Sort
void merge(int *ary, int l, int r, int m) {
    int i = l;
    int j = m + 1;
    int k = l;
    int * bry = new int [SIZE];
    while (i<=m && j<=r) {
      if (ary[i] < ary[j]) {
      	bry[k] = ary[i];
        i++;
      } else {
        bry[k] = ary[j];
        j++;
      }
      k++;
    }
    while (i<=m) {
      bry[k] = ary[i];
      k++;
      i++;
    }
    while (j<=r) {
      bry[k] = ary[j];
      k++;
      j++;
    }
    for (i=l; i<k; i++)
    {
      ary[i] = bry[i];
    }
}
int mergesort(int *ary, int l, int r) {
  if (l < r) {
		int m = (l+r)/2;
    mergesort(ary,l,m);
		mergesort(ary,m+1,r);
		merge(ary,l,r,m);
	}
	return 0;
}

//type 3: Heap Sort
void heapify(int *a, int i, int max) {
	int l = 2*i+1;
	int r = 2*i+2;
	int k = i;
	if (l<max && a[l]>a[i]) 
		k = l;
	if (r<max && a[r]>a[k]) 
		k = r;
	if (k!=i) {
		int tmp = a[i];
		a[i] = a[k];
		a[k] = tmp;
		heapify(a, k, max);	
	}	
}
void heapsort(int *a, int n) {
	for (int i=n/2-1; i>=0; i--) { //build heap 
		heapify(a,i,n);
  }
  for (int i=n-1; i>=1; i--) {
    int temp = a[0];
    a[0] = a[i];
    a[i] = temp;
    heapify(a, 0, i);
  }
}
//type 4: Quick Sort 
void swap(int* a, int i, int j) {
		int tmp = a[j];
		a[j] = a[i];
		a[i] = tmp;
}
int partition(int* a, int l, int r) {
	swap(a,(l+r)/2,r); //pick the middle as the pivot
	int p = l;
	for (int i=l; i<r; i++) {
		if (a[i]<=a[r]) {
			swap(a,i,p);
			p++;
		}
	} 
	swap(a,p,r);
	return p;
}
void quicksort(int *a, int l, int r) {
	if (l<r) {
		int p = partition(a,l,r);
		quicksort(a,l,p-1);
		quicksort(a,p+1,r);
	}
}

//type 5: stupid sort
void stupidsort(int *a, int n) {
	int i=1;
	while (i<n) {
		if (a[i]<a[i-1]) {
			swap(a,i,i-1);
			i=i-1;
			if (i==0) i=1;
		} else {
			i=i+1;
		}
	}
}

//type 6: Bubble Sort
void bubblesort(int* ary, int n) {
	for (int i=(n-1); i>=0; i--) {
	  for (int j=0; j<i-1; j++) {
	    if (ary[j]>ary[j+1])
	      swap(ary, j, j+1);	
	  }
	}
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
	} if (type==5) { //quick sort
	  stupidsort(ary,size); //array and size
	} else {         //do nothing
  	  bubblesort(ary,size);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  //int SortType = strtol(argv[1], NULL, 10);
  //PROCESS_MEMORY_COUNTERS pmc;

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
  MySort(iary, SIZE, 6);
   
  //for debug only
  cout << "After sorting:\t"; 
  printarray(iary, SIZE);
  cout << endl;
  
  //display memory usage 
  //GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)); 
  //size_t usedMemory = pmc.PeakWorkingSetSize; 
  //cout << "Memory usage: " << usedMemory/1000000 << " Mb\n"; 

  return 0;	
}
