#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/intersection-of-two-arrays-ii/description/
 * 两个数组的交集 II
 */

int *intersect(int *nums1, int nums1Size, int *nums2, int nums2Size, int *returnSize)
{
    int *ret = 0, len = nums1Size;
    if (nums1Size > nums2Size) //num1换为长的
    {
        // int *tmp = nums1;
        // nums1 = nums2;
        // nums2 = tmp;
        len = nums2Size;
    }
    ret = malloc(sizeof(int) * len);
    len = 0;
    for (int i = 0; i < nums1Size; i++)
    {
        for (int n = len; n < nums2Size; n++) //从len开始
        {
            if (nums1[i] == nums2[n]) //每当相等时，将这个位置的数字和len位置的对调
            {
                nums2[n] = nums2[len];
                nums2[len] = nums1[i];
                ret[len++] = nums1[i];
                break;
            }
        }
    }
    *returnSize = len;
    return ret;
}

/*
//大佬的写法
int cmp(const void * a, const void * b)
{
    if(*(int *)a == INT_MIN)
        return (*(int *)a);
    else if (*(int *)b == INT_MIN)
        return 1;
    else
        return (*(int*)a - *(int*)b);
}
int* intersect(int* nums1, int nums1Size, int* nums2, int nums2Size, int* returnSize) {
    qsort(nums1,nums1Size,sizeof(int),cmp);
    qsort(nums2,nums2Size,sizeof(int),cmp);
    int len = nums1Size < nums2Size ? nums1Size : nums2Size;
    int * res = malloc(sizeof(int)*len);
    int length = 0;
    int i = 0 ,j = 0;
    while(i<nums1Size && j<nums2Size)
    {
        if(nums1[i]>nums2[j])
            j++;
        else if(nums1[i]<nums2[j])
            i++;
        else if(nums1[i] == nums2[j])
        {
            res[length++] = nums1[i];
            i++;
            j++;
        }
    }
    *returnSize = length;
    return res;    
}
*/

int main()
{
    // int a1[] = {3, 1, 2};
    // int a2[] = {1};
    int a1[] = {61, 24, 20, 58, 95, 53, 17, 32, 45, 85, 70, 20, 83, 62, 35, 89, 5, 95, 12, 86, 58, 77, 30, 64, 46, 13, 5, 92, 67, 40, 20, 38, 31, 18, 89, 85, 7, 30, 67, 34, 62, 35, 47, 98, 3, 41, 53, 26, 66, 40, 54, 44, 57, 46, 70, 60, 4, 63, 82, 42, 65, 59, 17, 98, 29, 72, 1, 96, 82, 66, 98, 6, 92, 31, 43, 81, 88, 60, 10, 55, 66, 82, 0, 79, 11, 81};
    int a2[] = {5, 25, 4, 39, 57, 49, 93, 79, 7, 8, 49, 89, 2, 7, 73, 88, 45, 15, 34, 92, 84, 38, 85, 34, 16, 6, 99, 0, 2, 36, 68, 52, 73, 50, 77, 44, 61, 48};
    int len = 0;
    intersect(a1, sizeof(a1) / sizeof(int), a2, sizeof(a2) / sizeof(int), &len);
    printf("%d\n", len);
    getchar();
    return 0;
}