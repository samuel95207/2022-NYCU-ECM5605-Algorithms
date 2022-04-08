#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <climits>
//#include "windows.h"
//#include "psapi.h"
//#pragma comment(lib, "strmiids.lib")
//#pragma comment(lib, "psapi.lib")

using namespace std;

const int MM = 5;
int SelectionIdx(int[], int, int, int);
int medianOfMedians(int[], int, int);

void printarray(int * a, int n)
{
	for (int i=0; i<n; i++)
	{
		cout << a[i] << " ";
	}
}

int SelectionIdx(int a[], int l, int r, int k)
{
	/** k = 0, 1, 2, ... return k-th smallest value. */
	if(r-l+1 <= MM)  // insert sort.
	{
		int i, j, v;
		for(i = l+1; i <= r; i++)
		{
			v = a[i], j = i;
			while(j-1 >= l && a[j-1] > v)
				a[j] = a[j-1], j--;
			a[j] = v;
		}
		return l+k;
	}
	//printf("%d %d\n", l, r);
	int pivot = medianOfMedians(a, l, r);
	// partition begin.
	swap(a[l], a[pivot]);
	int i, j, t = a[l];
	for(i = l, j = l+1; j <= r; j++)
	{
		if(a[j] <= t)
			i++, swap(a[i], a[j]);
	}
	swap(a[l], a[i]);
	// partition end.
	int position = i;
	if(position-l == k)    return position;
	if(position-l < k)
		return SelectionIdx(a, position+1, r, k-(position-l+1));
	else
		return SelectionIdx(a, l, position, k);
}

int medianOfMedians(int a[], int l, int r)
{
	int numMedians = (r-l+MM-1)/MM;
	int i, subl, subr;
	int medianIdx;
	for(i = 0; i < numMedians; i++)
	{
		subl = l + i*MM;
		subr = subl + MM-1;
		if(subr > r)    subr = r;
		medianIdx = SelectionIdx(a, subl, subr, (subr-subl)/2);
		swap(a[l+i], a[medianIdx]);
	}
	//printf("median %d %d\n", l, r);
	return SelectionIdx(a, l, l+numMedians, numMedians/2);
}

int main(int argc, char** argv)
{
	//int SortType = strtol(argv[1], NULL, 10);
	//PROCESS_MEMORY_COUNTERS pmc;

	// your code here
	int n = atoi(argv[1]);
	int iary[n];
	srand(0);
	for (int i=0; i<n; i++)
	{
		int num = 0;
		while (num==0)
		{
			num = rand()/100+100;
			for (int j=0; j<i; j++)
				if (iary[j]==num) num = 0;
		}
		iary[i] = num;
	}
	printarray(iary, n);
	cout << endl;

	int k = atoi(argv[2]);
	int idx = SelectionIdx(iary, 0, n-1, k-1); //modified from Selection Aglorithm so k-1
	cout << k << "-th order statistics = " << iary[idx];

	//display memory usage
	//GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	//size_t usedMemory = pmc.PeakWorkingSetSize;
	//cout << endl<< "Memory usage: " << usedMemory/1000000 << " Mb\n";

	return 0;
}
