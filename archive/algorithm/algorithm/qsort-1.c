#include <stdio.h>
#include <stdlib.h>
//练习快排+去重
#define AMOUNT 1000

void my_qsort(int* num, int len) {
    //选第一位为基数
    if (len <= 1) {
        return;
    }
    int base = num[0];  //基数
    int n = len - 1, i = 1;
    for (; i < len; i++) {
        //从左往右扫描,遇到比base大的定住,然后右往左扫描,比len小的交换,然后继续
        // 4 2 3 5 7 2 1    4 2 3 1 7 2 5    4 2 3 1 2 7 5
        // 2 2 3 1 4 7 5
        if (num[i] > base) {
            for (; n > i; n--) {
                if (num[n] < base) {
                    //交换
                    int tmp = num[i];
                    num[i] = num[n];
                    num[n] = tmp;
                    break;
                }
            }
        }
        if (i == n) {
            //相遇了,结束,并交换基数
            int tmp = num[i - 1];
            num[i - 1] = num[0];
            num[0] = tmp;
            break;
        }
    }
    //对位置左右两边排序,分治的思路
    my_qsort(num, i);
    my_qsort(&num[i], len - i);
}

int main() {
    int num[AMOUNT] = {0};
    srand(1);
    for (int i = 0; i < AMOUNT; i++) {
        num[i] = rand();
        printf("%d\t", num[i]);
    }
    printf("\n");
    my_qsort(num, AMOUNT);
    printf("%d\t", num[0]);
    for (int i = 1; i < AMOUNT; i++) {
        //去重输出
        if (num[i] != num[i - 1]) {
            printf("%d\t", num[i]);
        }
    }
    getchar();
}
