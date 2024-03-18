#include <stdio.h>
#include <stdlib.h>
// 0 1
/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* countBits(int num, int* returnSize) {
    int *dp = malloc(sizeof(int) * (num + 1)), count = 2, pos = 0;
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= num; i++) {
        pos = i % count;
        if (!pos && count != i) {
            count = count * 2;
        }
        dp[i] = dp[pos] + 1;
    }
    *returnSize = num + 1;
    return dp;
}
/*
int* countBits(int num, int* rS) {
    *rS = num+1;
    int *res = calloc(num+2, sizeof(int));
    res[0] = 0; res[1] = 1;
    int i=2, j;
    while(i<=num) {
        for(j=i; j<=num&&j<(i<<1); j++) {
            res[j] = 1+res[j-i];
        }
        i <<= 1;
    }
    return res;
}
*/