#include <stdio.h>
#include <math.h>

#define bool int
#define true 1
#define false 0
bool isPowerOfFour(int num)
{
    while (num % 4 == 0)
    {
        num /= 4;
        if (num == 0)
        {
            return false;
        }
    }
    return num == 1;
}

int main()
{
    isPowerOfFour(27);
    isPowerOfFour(4);
    isPowerOfFour(16);
    getchar();
    return 0;
}