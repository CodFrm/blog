#include <stdio.h>

/**
 * 位运算减法
 * 减法就是加法,只是后者换成了一个负数
 * 负数补码:求负整数的补码，将其对应正数二进制表示所有位取反（包括符号位，0变1，1变0）后加1
 */

int add(int num1, int num2)
{
    while (num2)
    {
        int tmp = num1 ^ num2;
        num2 = (num1 & num2) << 1;
        num1 = tmp;
    }
    return num1;
}

int bit_sub(int num1, int num2)
{
    return add(num1, add(~num2, 1));
}

int main()
{
    printf("%d\n", bit_sub(32, 1));
    getchar();
    return 0;
}