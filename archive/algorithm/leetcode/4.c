#include <stdio.h>

/**
 * https://leetcode-cn.com/explore/interview/card/top-interview-questions-easy/1/array/25/
 * 只出现一次的数字
 */
int containsDuplicate(int *nums, int numsSize)
{
    int ret=0;
    for (int i = 0; i < numsSize; i++)
    {
       ret^=nums[i];
    }
    return ret;
}

int main()
{
    int nums[] = {1, 2,3,3,8,2,1,5,9,4,9,5,4};
    int b = containsDuplicate(nums, sizeof(nums) / 4);
    printf("%d\n", b);
    return 0;
}