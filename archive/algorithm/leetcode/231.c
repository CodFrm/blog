#include <stdio.h>
#include <math.h>

#define bool int
#define true 1
#define false 0
bool isPowerOfTwo(int num)
{
    while (num % 2 == 0)
    {
        num /= 2;
        if (num == 0)
        {
            return false;
        }
    }
    return num == 1;
}

int main()
{
    isPowerOfTwo(27);
    isPowerOfTwo(4);
    isPowerOfTwo(16);
    getchar();
    return 0;
}