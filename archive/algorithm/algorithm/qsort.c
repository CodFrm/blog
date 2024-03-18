#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**
 * 快速排序O(NlogN)和插入排序O(N^2)
 * 根据《数据结构和算法》学习
 */
void swap(int *array, int pos, int pos2);
int *insert_sort(int *array, int len)
{
    for (int p = 1; p < len; p++)
    {
        int tmp = array[p], i;
        for (i = p; i > 0 && array[i - 1] > tmp; i--) //插入排序，将前一个一直往后面插入，直到遇到，比tmp小的情况
        {
            array[i] = array[i - 1];
        }
        array[i] = tmp;
    }
    return array;
}
/**
 * 快速排序，递归，利用分治的方法
 * 首先确定一个基数，比其小的移向左，大的移向右
 * 然后递归调用
 */
int *quick_sort(int *array, int len)
{
    if (len <= 20) //长度小于20的采用插入排序法
    {
        return insert_sort(array, len);
    }
    //先取 左 中 右 三个数字
    int l = 0, m, r = len - 2, pos = r / 2; //-2是取倒数第二个
    //将三个数字排序,左最小,右最大
    if (array[l] > array[r])
        swap(array, l, r);
    if (array[l] > array[pos])
        swap(array, l, pos);
    if (array[pos] > array[r])
        swap(array, pos, r);

    swap(array, pos, len - 1); //交换中间的数字和最后一个

    m = array[len - 1];
    while (1)
    {
        while (array[++l] < m)
            ; //左边的一直向右移动，直到遇到大于m的
        while (array[--r] > m)
            ; //右边的一直向左移动，直到遇到小于m的
        if (l >= r)
        {
            break;
        }
        swap(array, l, r);
    }
    //交换l和最后的
    swap(array, l, len - 1);
    quick_sort(array, l);
    quick_sort(&array[l + 1], len - l - 1);
    return array;
}

void swap(int *array, int pos, int pos2)
{
    int tmp = array[pos];
    array[pos] = array[pos2];
    array[pos2] = tmp;
}

void print_r(int *a, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

#ifndef MAIN_FUNC
int main()
{
    int a[] = {8, 1, 4, 7, 7, 6, 2, 1, 1, 6, 68, 1, 5, 5, 4, 54, 89, 42, 7, 0};
    quick_sort(a, sizeof(a) / sizeof(int));
    print_r(a, sizeof(a) / sizeof(int));
    getchar();
    return 0;
}
#endif