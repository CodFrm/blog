#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include "../struct/adt/u_link_list.h"
/**
 * 基数排序
 */
#define getN(number, N) (int)((float)(number / pow(10, N - 1))) % 10
#define NODE_SIZE sizeof(struct node)

/**
 * 基数排序,LDS
 * 参考地址:http://blog.csdn.net/lemon_tree12138/article/details/51695211
 */
void radix_sort_lds(int *number, int len)
{
    int *buck = malloc(sizeof(int) * len); //桶子
    int buck_sub[10] = {0};                //每一个桶的结束下标
    int now_bit = 0, max_bit = 0;          //现在的位数,最大的位数
    for (int i = 0; i < len; i++)          //获取最大的数字
    {
        if (number[i] > max_bit)
        {
            max_bit = number[i];
        }
    }
    int tmp = max_bit;
    max_bit = 0;
    do
    {
        max_bit++;
        tmp /= 10;
    } while (tmp > 0); //取出最大位数
    for (now_bit = 1; now_bit <= max_bit; now_bit++)
    {
        memset(buck_sub, 0, sizeof(buck_sub));
        // memset(buck, 0, sizeof(int) * len);
        for (int i = 0; i < len; i++) //获取每个桶中有多少个数字
        {
            buck_sub[getN(number[i], now_bit)] += 1;
        }
        for (int i = 1; i < 10; i++) //将数量转换为数组下标
        {
            buck_sub[i] = buck_sub[i] + buck_sub[i - 1];
        }
        for (int i = len - 1; i >= 0; i--) //将数组分配到桶中
        {
            int bit = getN(number[i], now_bit);
            buck[--buck_sub[bit]] = number[i];
        }
        //将桶中的数据交换过去,直接拷贝内存
        memcpy(number, buck, sizeof(int) * len);
    }
    free(buck);
}

#ifndef MAIN_FUNC
int main()
{
    int number[] = {1000, 999, 103};
    radix_sort_lds(number, sizeof(number) / 4);
    for (int i = 0; i < sizeof(number) / 4; i++)
    {
        printf("%d\t", number[i]);
    }
    getchar();
    return 0;
}
#endif