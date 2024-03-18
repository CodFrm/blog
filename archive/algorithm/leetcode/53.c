#include <stdio.h>
#include <stdlib.h>

int maxSubArray(int *nums, int numsSize) {
    if (numsSize <= 0) {
        return 0;
    }
    int *result = malloc(sizeof(int) * numsSize), max;
    memset(result, 0, sizeof(int) * numsSize);
    result[0] = nums[0];
    max = result[0];
    for (int i = 1; i < numsSize; i++) {
        result[i] = result[i - 1] + nums[i];
        if (result[i] < nums[i]) {
            result[i] = nums[i];
        }
        if (max < result[i]) {
            max = result[i];
        }
    }
    free(result);
    return max;
}
/*
#define MAX(a,b) a > b ? a:b

int maxSubArray(int* nums, int numsSize) {
    int* dp = malloc(sizeof(int) * numsSize);
    int i;
    int result;
    dp[0] = nums[0];
    for(i = 1; i < numsSize; i++)
        dp[i] = MAX(nums[i], dp[i-1] + nums[i]);
    result = dp[0];
    for(i = 1; i < numsSize; i++)
        if(result < dp[i])
            result = dp[i];
    free(dp);
    return result;
}
*/
int main() {
    getchar();
    return 0;
}
