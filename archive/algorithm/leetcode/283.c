#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * 移动零
 * https://leetcode-cn.com/problems/move-zeroes/description/
 */
void moveZeroes(int *nums, int numsSize)
{
    int zeroNumber = 0;
    for (int i = numsSize - 1; i >= 0; i--)
    {
        if (nums[i] == 0) //等于0时，往后移动
        {
            for (int n = i; n < numsSize - zeroNumber - 1; n++) //后面的数字直接的往前赋值
            {
                nums[n] = nums[n + 1];
            }
            nums[numsSize - zeroNumber - 1] = 0;
            zeroNumber++;
        }
    }
}
/*
void moveZeroes(int* nums, int numsSize) {
    int zeroNum = 0;
    int i;
    for (i = 0; i < numsSize; i++) {
        if (nums[i] == 0) {
            zeroNum++;
        } else {
            nums[i-zeroNum] = nums[i];
        }
    }
    for (i = numsSize-zeroNum; i < numsSize; i++) {
        nums[i] = 0;
    }
}
*/
void print_r(int *a, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

int main()
{
    int tmp[] = {0, 1, 0, 3, 12};
    moveZeroes(tmp, sizeof(tmp) / sizeof(int));
    print_r(tmp, sizeof(tmp) / sizeof(int));
    getchar();
    return 0;
}
