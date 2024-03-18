#include <stdio.h>
#include "time.h"

#define MAIN_FUNC
#include "../algorithm/radix_sort.c"
#include "../algorithm/bubble.c"
#include "../algorithm/qsort.c"

/**
 * 排序耗时
 */
int big_rand()
{
    return (((rand() << 8) & 0xFF00ul) | ((rand()) & 0x00FFul));
}
/**
 * 生成随机数
 */
int *rand_generate(int len)
{
    // srand(time(NULL));
    int *list = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++)
    {
        list[i] = big_rand();
    }
    return list;
}
int *copy_int(int *source, int len)
{
    int *tmp = malloc(len * sizeof(int));
    memcpy(tmp, source, len * sizeof(int));
    return tmp;
}
void print_int(int *n)
{
    for (int i = 0; i < 100; i++)
    {
        printf("%d\t", n[i]);
    }
}

int main()
{
    
    int len = 10000;
    int *number = rand_generate(len);
    int *tmp = copy_int(number, len);
    clock_t start_time = clock(), t = 0;
    // 基数排序
    radix_sort_lds(tmp, len);
    t = clock();
    print_int(tmp);
    printf("radix_sort_lds number:%d\ttime:%ldms\n", len, t - start_time);
    //快速排序
    tmp = copy_int(number, len);
    start_time = clock();
    quick_sort(tmp, len);
    t = clock();
    print_int(tmp);
    printf("quick_sort\tnumber:%d\ttime:%ldms\n", len, t - start_time);
    //插入排序
    tmp = copy_int(number, len);
    start_time = clock();
    insert_sort(tmp, len);
    t = clock();
    print_int(tmp);
    printf("insert_sort number:%d\ttime:%ldms\n", len, t - start_time);
    //冒泡排序
    tmp = copy_int(number, len);
    start_time = clock();
    bubble_sort(tmp, len);
    t = clock();
    print_int(tmp);
    printf("bubble_sort number:%d\ttime:%ldms\n", len, t - start_time);
    getchar();
    return 0;
}