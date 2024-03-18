#define MIN(a, b) (a > b ? b : a)
int minPathSum(int** grid, int gridRowSize, int gridColSize) {
    int** dp = malloc(sizeof(int*) * gridRowSize);
    for (int i = 0; i < gridRowSize; i++) {
        dp[i] = malloc(sizeof(int) * gridColSize);
    }
    for (int m = 0; m < gridRowSize; m++) {
        for (int n = 0; n < gridColSize; n++) {
            if (m == 0 && n == 0) {
                dp[m][n] = grid[m][n];
            } else if (m == 0) {
                dp[m][n] = dp[m][n - 1] + grid[m][n];
            } else if (n == 0) {
                dp[m][n] = dp[m - 1][n] + grid[m][n];
            } else {
                dp[m][n] = MIN(dp[m - 1][n], dp[m][n - 1]) + grid[m][n];
            }
        }
    }
    return dp[gridRowSize - 1][gridColSize - 1];
}
