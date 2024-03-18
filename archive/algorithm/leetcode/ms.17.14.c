#include <stdio.h>
#include <malloc.h>

//面试题 17.14. 最小K个数
//https://leetcode-cn.com/problems/smallest-k-lcci/

void swap(int *array, int pos, int pos2)
{
    int tmp = array[pos];
    array[pos] = array[pos2];
    array[pos2] = tmp;
}

int *topk(int *arr, int start, int end, int k, int *returnSize)
{
    int q = start, p = end + 1, mid = (end - start) / 2 + start, flag = 0;
    if (arr[start] > arr[end])
    {
        swap(arr, start, end);
    }
    if (arr[mid] > arr[end])
    {
        swap(arr, mid, end);
    }
    if (arr[start] > arr[mid])
    {
        swap(arr, mid, end);
    }
    flag = arr[mid];
    swap(arr, start, mid);
    while (1)
    {
        while (arr[++q] < flag)
            ;
        while (arr[--p] > flag)
            ;
        if (q > p)
        {
            break;
        }
        swap(arr, q, p);
    }
    swap(arr, start, p);
    if (p > k)
    {
        //大于k,搜索左边
        topk(arr, start, p - 1, k, returnSize);
    }
    else if (p < k)
    {
        //小于k,搜索右边
        topk(arr, p + 1, end, k, returnSize);
    }
    else
    {
        //等于k,返回
        return arr;
    }
    return arr;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *smallestK(int *arr, int arrSize, int k, int *returnSize)
{
    if (k == arrSize)
    {
        return arr;
    }
    *returnSize = k;
    topk(arr, 0, arrSize - 1, k, returnSize);

    return arr;
}

int main()
{
    int arr[] = {4, -5051, 6, 10156, 1, -39670, 2, -686, -19101, 42719, 8216, -56, -30716, -6, 0, -14, 97, -37023, 0, -28, 5094, 884, -508, 36504, 47475, 8, 37603, 2, -5021, -65, 0, 3229, 6068, 7, 5534, -577, -184, 92, 4, -797, -126, 460};
    int ret = 0;
    smallestK(arr, sizeof(arr) / sizeof(int), 34, &ret);
}