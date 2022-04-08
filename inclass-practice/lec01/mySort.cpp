// C++ program for insertion sort
#include <bits/stdc++.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
using namespace std; 
const int RUN = 32; 
const int LEN = 1000; //MAX: 10K

/* Function to sort an array using insertion sort*/
void insertionSort(int arr[], int n)
{
    //write your own code here!!!

} 

// A utility function ot print an array of size n
void printArray(int arr[], int n)
{
    for (long int i=0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}
   
/* Driver program to test insertion sort */
int main()
{
    timeval start;
    timeval end;
    //long int arr[] = {12, 11, 13, 5, 6, 1, 3, 2, 14, 19};
    int *arr = new int[LEN];
    ifstream fin("10K.in");
    for (int i=0; i<LEN; i++) {
    	fin >> arr[i];	
		//cout << i << ":" << arr[i] << " "; 		
    }		 
    //printf("Given Array is\n"); 
    printArray(arr, LEN); 
    gettimeofday(&start, NULL);
    insertionSort(arr, LEN); 
    gettimeofday(&end, NULL);
    long int diff = 1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
    cout << "Sorting " << LEN << " numbers takes " << diff << " usec.\n";
    //printf("After Sorting Array is\n"); 
    //printArray(arr, LEN); 
    return 0; 
}
