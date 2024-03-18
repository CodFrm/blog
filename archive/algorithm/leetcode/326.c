#include <stdio.h>
#include <math.h>

#define bool int
#define true 1
#define false 0

bool isPowerOfThree(int n)
{
    double a = log10l(n) / log10l(3);
    if (a - ((int)a) == 0)
    {
        return true;
    }
    return false;
}
/*
第二种不用循环的解法
bool isPowerOfThree(int n) {
    if(n <= 0)  return false;
     while ( n % 3 == 0) {
            n /= 3;
         if( n==0 )  return false;
        }
        return n == 1;  
}
*/

int main()
{
    isPowerOfThree(27);
    isPowerOfThree(243);
    isPowerOfThree(4782968);
    getchar();
    return 0;
}