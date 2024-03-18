#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) a > b ? a : b;
int rob(int* nums, int numsSize) {
    if (numsSize <= 0) {
        return 0;
    }
    if (numsSize == 1) {
        return nums[0];
    }
    int *dp = malloc(sizeof(int) * numsSize), max = 0;
    dp[0] = nums[0];
    dp[1] = MAX(nums[0], nums[1]);
    for (int i = 2; i < numsSize; i++) {
        dp[i] = MAX(dp[i - 1], dp[i - 2] + nums[i]);
    }
    max = MAX(dp[numsSize - 1], dp[numsSize - 2]);
    return max;
}
/*
int rob(int* nums, int numsSize) {
    int i=0;
    int *dp=(int*)malloc(sizeof(int)*(numsSize+1));
    dp[0]=nums[0];
    dp[1]=nums[0]>nums[1]?nums[0]:nums[1];
    for(i=2;i<numsSize;++i)
    {
        dp[i]=dp[i-1]>dp[i-2]+nums[i]?dp[i-1]:dp[i-2]+nums[i];
    }
    return dp[numsSize-1];
}
*/
int main() {
    getchar();
    return 0;
}