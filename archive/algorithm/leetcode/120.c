#define MIN(a, b) (a > b ? b : a)
int minimumTotal(int **triangle, int triangleRowSize, int *triangleColSizes) {
    int **dp = malloc(sizeof(int *) * triangleRowSize), ret = 0;
    for (int i = 0; i < triangleRowSize; i++) {
        dp[i] = malloc(sizeof(int) * triangleRowSize);
    }
    dp[0][0] = triangle[0][0];
    for (int i = 1; i < triangleRowSize; i++) {
        for (int n = 0; n <= i; n++) {
            if (n == 0) {
                dp[i][n] = dp[i - 1][n];
            } else if (n == i) {
                dp[i][n] = dp[i - 1][n - 1];
            } else {
                dp[i][n] = MIN(dp[i - 1][n], dp[i - 1][n - 1]);
            }
            dp[i][n] += triangle[i][n];
        }
    }
    ret = dp[triangleRowSize - 1][0];
    for (int i = 1; i < triangleRowSize; i++) {
        ret = MIN(ret, dp[triangleRowSize - 1][i]);
    }
    return ret;
}

/*
int minimumSubTotal(int** triangle, int triangleRowSize, int *triangleColSizes, int level, int* minTotal) {
	int i = 0;
	int min = INT_MAX;
	int res = 0;
	int temp = 0;
    if(level == triangleRowSize - 1){
		for(i = 0; i <= level; i++){
			minTotal[i] = triangle[level][i];
			min = minTotal[i] < min ? minTotal[i] : min;
		}
		return min;
	}
	minimumSubTotal(triangle, triangleRowSize, triangleColSizes, level + 1, minTotal);

	for(i = 0, temp = minTotal[0]; i <= level; i++){
		res = (temp > minTotal[i + 1] ? minTotal[i + 1] : temp) + triangle[level][i];
		temp = minTotal[i + 1];
		minTotal[i] = res;
		min = minTotal[i] < min ? minTotal[i] : min;
	}

	return min;
}

int minimumTotal(int** triangle, int triangleRowSize, int *triangleColSizes) {
    int ret = 0;
	int* minTotal = (int*)malloc(sizeof(int) * triangleRowSize);
	ret = minimumSubTotal(triangle, triangleRowSize, triangleColSizes, 0, minTotal);
	free(minTotal);

	return ret;
}
*/