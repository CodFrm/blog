#include <stdio.h>

/**
 * https://leetcode-cn.com/explore/interview/card/top-interview-questions-easy/1/array/24/
 * 数组存在重复
 */

int containsDuplicate(int *nums, int numsSize)
{
    for (int i = 0; i < numsSize - 1; i++)
    {
        for (int n = i + 1; n < numsSize; n++)
        {
            if (nums[i] == nums[n])
            {
                return 1;
            }
        }
    }
    return 0;
}

int main()
{
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 7};
    containsDuplicate(nums, 8);
    return 0;
}