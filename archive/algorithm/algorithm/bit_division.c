#include <stdio.h>

/**
 * 位运算除法
 * 好像可以直接的用减法来算,到不能减了停止
 * 但是这个方法,需要算太多次,O(N)
 * 那么我们每次将被除数扩大一倍(使用左移),从最大的开始
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

int bit_division(int num1, int num2)
{
    int result = 0;
    for (int i = 31; i >= 0; i--)
    {
        if ((num1 >> i) >= num2)
        {
            result = add(result, 1 << i);
            num1 = sub(num1, num2 << i);
        }
    }
    return result;
}

int main()
{
    printf("%d\n", bit_division(10, 3));
    getchar();
    return 0;
}