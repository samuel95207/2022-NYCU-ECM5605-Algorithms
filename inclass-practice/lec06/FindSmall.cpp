// C++ program to find the smallest number that can be
// formed from given sum of digits and number of digits.
#include <iostream>
using namespace std;
 
// Prints the smallest possible number with digit sum 's'
// and 'm' number of digits.
void findSmallest(int m, int s)
{
  //create an array to store digits of result
  int res[m];
  //write your own code below
  
  
 
  cout << "Smallest number is ";
  for (int i=0; i<m; i++)
      cout << res[i];
}
 
// Driver code
int main() {
    int s = 9, m = 2;  //test 1 = 19 
    //int s = 20, m = 3; //test 2 = 299
    findSmallest(m, s);
    return 0;
}
