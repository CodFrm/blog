#include <stdio.h>

/**
 * 位运算乘法(要不循环加法,滑稽)
 * 按照算10位一样,列个式子
 * 0011
 * x010
 * ----
 *   00
 *  11
 * ----
 * 0110
 * 
 * 
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

int bit_mul(int num1, int num2)
{
    int result = 0;
    //只考虑了正数,负数的话,先右移31,看结果为1还是0,为1则是负数(补码)
    //然后将数字转换为正数(负负得正,再转换成一次负数)
    //最后算出结果的时候,如果是负再转换回去
    while (num2)
    {
        if (num2 & 1)
        {
            result = add(result, num1);
        }
        num1 = num1 << 1;
        num2 = num2 >> 1;
    }
    return result;
}

int main()
{
    printf("%d\n", bit_mul(8, 2));
    getchar();
    return 0;
}