#include <stdio.h>

/**
 * https://leetcode-cn.com/explore/interview/card/top-interview-questions-easy/1/array/23/
 * 旋转数组
 */

void rotate(int *nums, int numsSize, int k)
{
    int end = 0;
    for (int i = 0; i < k; i++)
    {
        //记录最后一位,数组向后移动一位
        end = nums[numsSize - 1];
        for (int n = 0; n < numsSize; n++)
        {
            nums[numsSize - n - 1] = nums[numsSize - n - 2];
        }
        nums[0]=end;
    }
}

int main()
{
    int nums[] = {1, 2, 3, 4, 5, 6, 7};
    rotate(nums, 7, 3);
    return 0;
}