#include <iostream>
using namespace std;

const int X = 5, Y = 5;
int a[X+1][Y+1] = { {0, 0, 0, 0, 0, 0},
			 {0, 0, 1, 3, 2, 4},
	     {0, 4, -2, 2, 4, 6},
			 {0, 1, 5, -3, 8, 5},
			 {0, 6, 4, -1, 3, 2},
			 {0, 7, 8, 3, 0, 1}};
int c[X+1][Y+1] = {0};
int p[X+1][Y+1] = {0};
int out[X+Y-1] = {0};
 
void staircase_walk()
{
    // [Initial]
 
    // [Compute]
    for (int i=1; i<=X; i++)
        for (int j=1; j<=Y; j++)
            if (c[i-1][j] < c[i][j-1])
            {
                c[i][j] = c[i-1][j] + a[i][j];
                p[i][j] = 0;    // 從上走來
            }
            else /*if (c[i-1][j] >= c[i][j-1])*/
            {
                c[i][j] = c[i][j-1] + a[i][j];
                p[i][j] = 1;    // 從左走來
            }
 
    // 反向追蹤路線源頭
    int n = 0;  // out size
    for (int i=X, j=Y; i>0 && j>0; )
    {
        out[n++] = p[i][j];
        if (p[i][j] == 0) i--;
        else if (p[i][j] == 1) j--;
    }
 
    // 印出路線
    for (int i=n-1; i>=0; --i)
        cout << out[i];
}

int main() {
	
	staircase_walk();
	
	return 0;
}
