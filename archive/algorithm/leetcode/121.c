#include <stdio.h>
#include <stdlib.h>

int maxProfit(int *prices, int pricesSize) {
    if (pricesSize <= 1) {
        return 0;
    }
    int max = 0, min_buy = prices[0], tmp = 0;
    for (int i = 1; i < pricesSize; i++) {
        if (min_buy > prices[i]) {
            min_buy = prices[i];
        }
        tmp = prices[i] - min_buy;
        if (max < tmp) {
            max = tmp;
        }
    }
    return max;
}
/*
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) > (b) ? (b) : (a))
int maxProfit(int* prices, int pricesSize) {
    int low = INT_MAX, profit = 0;
    for(int i = 0 ; i< pricesSize; i++){
        low = min(low, prices[i]);
        profit = max(profit, prices[i] - low);
    }
    return  profit;
}
*/
int main() {
    getchar();
    return 0;
}
