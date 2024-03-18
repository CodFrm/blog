#include <stdio.h>

/**
 * 优先队列--堆 堆排序
 * 堆是一个完全二叉树
 */

void swap(int *array, int pos, int pos2)
{
    int tmp = array[pos];
    array[pos] = array[pos2];
    array[pos2] = tmp;
}

void heap_sort(int *heap, int len)
{
    /**
     * 构造堆
     */
    for (int i = (len / 2 - 1); i >= 0; i--)
    {
        if (heap[i] > heap[(i + 1) * 2 - 1])
        {
            swap(heap, i, (i + 1) * 2 - 1);
        }
        if (len > (i + 1) * 2 && heap[i] > heap[(i + 1) * 2])
        {
            swap(heap, i, (i + 1) * 2);
        }
    }
    /**
     * 调整堆
     */
    if (len > 1)
    {
        heap_sort(&heap[1], len - 1);
    }
}
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
    int heap[] = {17, 489, 5, 23, 6746, 64, 49, 8, 41, 56, 46, 17, 15,
                  461, 54, 61, 165, 89, 15, 64, 14, 664};
    heap_sort(heap, sizeof(heap) / sizeof(int));
    print_r(heap, sizeof(heap) / sizeof(int));
    return 0;
}
