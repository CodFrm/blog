int numTrees(int n) {
    int *dp = malloc(sizeof(int) * (n + 1));
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int n = 1; n <= i; n++) {
            dp[i] += (dp[n - 1] * dp[i - n]);
        }
    }
    return dp[n];
}
// 1 2 5
// 2   1
// 1     2
//