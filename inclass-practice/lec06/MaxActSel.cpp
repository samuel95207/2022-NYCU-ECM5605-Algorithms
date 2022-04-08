// when input activities may not be sorted.
#include <iostream>
#include <algorithm>
using namespace std;
 
// A job has start time, finish time and profit.
struct Activitiy {
    int start, finish;
};
 
// sorting activities according to finish time
bool activityCompare(Activitiy s1, Activitiy s2) {
    return (s1.finish < s2.finish);
}
 
// print the maximum set of activities 
void printMaxActivities(Activitiy arr[], int n) {
    // Sort jobs according to finish time
    sort(arr, arr+n, activityCompare);
 
    cout << "The following activities are selected \n";
 
    // The first activity always gets selected
    int i = 0;
    cout << "(" << arr[i].start << ", " << arr[i].finish << "), ";
 
    // Consider rest of the activities
    for (int j = 1; j < n; j++) {
      if (arr[j].start >= arr[i].finish) {
        cout << "(" << arr[j].start << ", "
             << arr[j].finish << "), ";
        i = j;
      }
    }
}
 
// Driver program
int main()
{
    Activitiy arr[] = {{5, 9}, {1, 2}, {3, 4}, {0, 6}, {5, 7}, {8, 9}};
    //Activitiy arr[] = {	{1,4}, {3,5}, {0,6}, {5,7}, {3,8}, {5,9}, {6,10}, {8,11}, {8,12}, {9,13}, {12,14} };
    int n = sizeof(arr)/sizeof(arr[0]);
    printMaxActivities(arr, n);
    return 0;
}
