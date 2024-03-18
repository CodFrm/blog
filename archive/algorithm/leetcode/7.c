#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/reverse-integer/description/
 * 反转整数
 */
int reverse(int x)
{
    long long ret = 0;
    while (x)
    {
        ret = x % 10 + ret * 10;
        x /= 10;
    }
    if (ret <= 2147483647 && ret >= -2147483648)
    {
        return ret;
    }
    return 0;
}
/*
最快的dalao解法
int reverse(int x) {
    int y=0;
    while(x)
    {
        int temp=y;
        y=y*10+x%10;
        if((y-x%10)/10!=temp)
        {
            return 0;
        }
        x/=10;    
    }
    return y;
}
*/
int main()
{
    int a = pow(2, 31);
    printf("%d\n", reverse(1534236469));
    printf("%d\n", reverse(-321));
    printf("%d\n", reverse(-2147483648));
    printf("%d\n", reverse(32006));
    printf("%d\n", reverse(-1563847412));
    printf("%d\n", pow(2, 31));
    printf("%d\n", a);
    getchar();
    return 0;
}