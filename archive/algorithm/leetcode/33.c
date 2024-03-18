#include <stdio.h>

// https://leetcode-cn.com/problems/search-in-rotated-sorted-array/
// 搜索旋转排序数组

int search(int *nums, int numsSize, int target)
{
    int start = 0, mid = 0, end = numsSize - 1;
    while (start <= end)
    {
        mid = start + ((end - start) >> 1);
        if (nums[mid] == target)
        {
            return mid;
        }
        if (nums[start] <= nums[mid])
        {
            //前半部分有序
            if (target < nums[mid] && nums[start] <= target)
            {
                //目标数在前半部分中
                end = mid - 1;
            }
            else
            {
                //在无序的后半部分
                start = mid + 1;
            }
        }
        else
        {
            //后半部分有序
            if (target <= nums[end] && nums[mid] < target)
            {
                //目标数在后半部分中
                start = mid + 1;
            }
            else
            {
                //在无序的前半部分
                end = mid - 1;
            }
        }
    }
    return -1;
}

int main()
{
    int a[] = {4, 5, 6, 7, 0, 1, 2};
    search(a, sizeof(a) / sizeof(int), 0);
    return 0;
}