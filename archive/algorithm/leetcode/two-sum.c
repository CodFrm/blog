#include <stdio.h>
#include <stdlib.h>

/**
 * https://leetcode-cn.com/problems/two-sum/description/
 * 两数之和
 */
int *twoSum(int *nums, int numsSize, int target)
{
    int *s = malloc(sizeof(int) * 2);
    memset(s, 0, sizeof(int) * 2);
    for (int i = 0; i < numsSize; i++)
    {
        int tmp = target - nums[i];
        for (int n = i + 1; n < numsSize; n++)
        {
            if (nums[n] == tmp)
            {
                s[0] = i;
                s[1] = n;
                return s;
            }
        }
    }
    return 0;
}
int main()
{
    int nums[] = {2, 7, 11, 15};
    int *s = twoSum(nums, sizeof(nums) / 4, 9);
    printf("%d,%d\n", s[0], s[1]);
    getchar();
    return 0;
}