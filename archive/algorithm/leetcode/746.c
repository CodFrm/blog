#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) a > b ? b : a;
int minCostClimbingStairs(int* cost, int costSize) {
    int *dp = malloc(sizeof(int) * costSize), max = 0;
    memset(dp, 0, sizeof(int) * costSize);
    for (int i = 2; i < costSize; i++) {
        dp[i] = MIN(dp[i - 1] + cost[i - 1], dp[i - 2] + cost[i - 2]);
    }
    max = MIN(dp[costSize - 1] + cost[costSize - 1],
              dp[costSize - 2] + cost[costSize - 2]);
    free(dp);
    return max;
}
/*
int minCostClimbingStairs(int* cost, int costSize) {
    int sum[costSize];
    sum[costSize-1]=cost[costSize-1];
    sum[costSize-2]=cost[costSize-2];
    for(int i=costSize-3;i>=0;i--){
        sum[i]=(sum[i+1]>sum[i+2]?sum[i+2]:sum[i+1])+cost[i];
    }
    return sum[0]>sum[1]?sum[1]:sum[0];
}
*/
int main() {
    getchar();
    return 0;
}