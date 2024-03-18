#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int climbStairs(int n)
{
    int *tmp = malloc((n + 1) * sizeof(int));
    memset(tmp, sizeof(int) * (n + 1), 0);
    tmp[0] = 1;
    tmp[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        tmp[n] = tmp[n - 1] + tmp[n - 2];
    }
    return tmp[n];
}
int main()
{
    getchar();
    return 0;
}