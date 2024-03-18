#include <stdio.h>
/**
 * 冒泡排序,网上copy来的,只是想对比速度 
 */
void bubble_sort(int a[], int n)
{
    int i, j, temp;
    for (j = 0; j < n - 1; j++)
        for (i = 0; i < n - 1 - j; i++)
            if (a[i] > a[i + 1])
            {
                temp = a[i];
                a[i] = a[i + 1];
                a[i + 1] = temp;
            }
}

#ifndef MAIN_FUNC
int main()
{
    int number[] = {10, 56, 4894, 32, 894, 2354, 819, 15468, 1654, 89};
    bubble_sort(number, 10);
    getchar();
    return 0;
}
#endif