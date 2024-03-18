#include <stdio.h>

/**
 * https://leetcode-cn.com/problems/divide-two-integers/description/
 * 两数相除,不能直接使用乘法除法
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
int sub(int num1, int num2)
{
    return add(num1, add(~num2, 1));
}

int negative(int num) //判断负数
{
    return (num >> 31) & 1;
}

int abs_(int num)
{
    if (negative(num)) //右移31位判断正负
    {
        return add(~num, 1); //负数
    }
    return num;
}

int divide(int dividend, int divisor)
{
    int flag = 1;
    if (negative(dividend) == negative(divisor))
    {
        flag = 0;
    }
    unsigned int x = abs_(dividend);
    unsigned int y = abs_(divisor);
    unsigned int result = 0;
    for (int i = 31; i >= 0; i--)
    {
        if ((x >> i) >= y)
        {
            result = add(result, 1 << i);
            x = sub(x, y << i);
        }
    }
    if (flag)
    {
        //负数
        if (result > 2147483648)
        {
            return 2147483647;
        }
        return add(~result,1);
    }
    else
    {
        //正数
        if (result > 2147483647)
        {
            return 2147483647;
        }
        return result;
    }
}

/*
    对于int a = 0x80000000;
        long long int b = -a;
        而言，b依然是-2147483648，只不过是长长整型的-2147483648
        可能是右边的-a运算，由于预期的结果溢出，所以就没有做负号运算
        原先认为的是-a会导致溢出从而结果为0，实际测试发现不是
    long long int d = ((long long int)dividend > 0) ? (long long int)dividend : -(long long int)dividend;
    long long int div = ((long long int)divisor > 0) ? (long long int)divisor : -(long long int)divisor;
*/
/**
int divide(int dividend, int divisor) {
    if (divisor == 1)
        return dividend;
    
    if (divisor == -1)
    {
        if (dividend == (int)0x80000000)
            return 0x7FFFFFFF;
        else
            return -dividend;
    }
    
    int cnt = 0, factor = 0, i = 0;
    long long int d = dividend;
    long long int div = divisor;
    
    d = (d > 0) ? d : -d;
    div = (div > 0) ? div : -div;
    
    if ((dividend ^ divisor) & 0x80000000)
        factor = -1;
    else
        factor = 1;
    
    while (d >= div)
    {
        i = 0;
        while (d >= (div << i))
        {
            d -= (div << i);
            cnt += (1 << i++);
        }
    }
    
    return factor*cnt;
}
*/
int main()
{
    printf("%d\n", divide(-2147483648, -1));
    getchar();
    return 0;
}