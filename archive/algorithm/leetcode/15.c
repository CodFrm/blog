#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * https://leetcode-cn.com/problems/3sum/description/
 * 三数之和
 */
int **threeSum(int *nums, int numsSize, int *returnSize)
{
    int **ret = malloc(sizeof(int *) * numsSize); //行
    int len = numsSize;                           //数组现在的长度
    for (int i = 0; i < numsSize; i++)
    {
        ret[i] = malloc(sizeof(int) * 3);
        memset(ret[i], 0, sizeof(int) * 3);
    }
    *returnSize = 0;
    //排序
    int cmp(const void *a, const void *b)
    {
        return (*(int *)a - *(int *)b);
    }
    qsort(nums, numsSize, sizeof(int), &cmp);
    //双指针的方法
    //-4,-1,-1,0,1,2
    for (int i = 0; i < numsSize - 2; i++)
    {
        if (nums[i] > 0) //有序的情况下,为正数是不可能=0的,跳出循环
        {
            break;
        }
        if (i > 0 && nums[i] == nums[i - 1]) //向前检查数字重复
            continue;
        int h = i + 1, w = numsSize - 1, target = -nums[i];
        //向中间移动
        while (h < w)
        {
            if (nums[h] + nums[w] == target)
            {
                //判断内存足够么
                if (len - 1 < *returnSize)
                {
                    ret = realloc(ret, sizeof(int *) * (len + numsSize));
                    //重新分配内存
                    len += numsSize;
                    for (int i = *returnSize; i < len; i++)
                    {
                        ret[i] = malloc(sizeof(int) * 3);
                        memset(ret[i], 0, sizeof(int) * 3);
                    }
                }
                //符合条件
                ret[*returnSize][0] = nums[i];
                ret[*returnSize][1] = nums[h];
                ret[*returnSize][2] = nums[w];
                *returnSize = *returnSize + 1;
                //跳过相同的数字
                while (h < w && nums[h] == nums[h + 1])
                    h++;
                while (h < w && nums[w] == nums[w - 1])
                    w--;
                h++;
                w--;
            }
            else if (nums[h] + nums[w] < target)
            {
                //小一点,就往右移动
                h++;
            }
            else
            {
                //大一点
                w--;
            }
        }
    }

    return ret;
}

/*
int cmpfunc(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}

int** threeSum(int* nums, int numsSize, int* returnSize)
{
	int front, back;
	int i = 0;
	int **result = 0;
	int s = 0;

	qsort(nums, numsSize, sizeof(int), cmpfunc);

	while (i < numsSize)
	{
		int target = -nums[i];
		if (target < 0)
		{
			break;
		}

		front = i + 1;
		back = numsSize - 1;

		while (front < back)
		{
			int sum = nums[front] + nums[back];
			if (sum < target)
			{
				front++;
			}
			else if (sum > target)
			{
				back--;
			}
			else
			{
				s++;
				result = (int **)realloc(result, sizeof(int *)*s);
				int *temp = (int *)malloc(sizeof(int) * 3);
				temp[0] = nums[i];
				temp[1] = nums[front];
				temp[2] = nums[back];
				result[s-1] = temp;

				while (front < back && nums[front] == temp[1])
				{
					front++;
				}
				while (front < back && nums[back] == temp[2])
				{
					back--;
				}
			}
		}
		while ((i + 1) < numsSize && nums[i] == nums[i + 1])
		{
			i++;
		}
		i++;
	}

	//returnSize = (int *)malloc(sizeof(int));
	*returnSize = s;
	return result;
}
*/

int main()
{
    // int a[] = {-7, -4, -6, 6, 4, -6, -9, -10, -7, 5, 3, -1, -5, 8, -1, -2, -8, -1, 5, -3, -5, 4, 2, -5, -4, 4, 7}, size;
    int a[] = {0, 0, 0, 0}, size;
    int **ret = threeSum(a, sizeof(a) / sizeof(int), &size);
    getchar();
    return 0;
}