#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/plus-one/description/
 * +1(s)
 * 不断优化，达到了0ms。。。。虽然是道简单题
 */
int *plusOne(int *digits, int digitsSize, int *returnSize)
{
    int *ret = malloc(sizeof(int) * (digitsSize + 1));
    *returnSize = digitsSize;
    int next = 1, pos;
    for (int i = digitsSize; i > 0; i--)
    {
        ret[i] = digits[i - 1] + next;
        if (ret[i] == 10)
        {
            ret[i] = 0;
            next = 1;
        }
        else
        {
            next = 0;
        }
    }
    if (next)
    {
        ret[0] = 1;
        (*returnSize)++;
    }
    else
    {
        ret = &ret[1];
    }
    return ret;
}

int main()
{
    int a[] = {9, 8}, ret = 0;
    int *retArray = plusOne(a, 2, &ret);
    for (int i = 0; i < ret; i++)
    {
        printf("%d\t", retArray[i]);
    }
    printf("%d\n", ret);
    getchar();
    return 0;
}