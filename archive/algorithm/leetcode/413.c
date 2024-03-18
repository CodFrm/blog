int numberOfArithmeticSlices(int* A, int ASize) {
    int *dp = malloc(sizeof(int) * ASize), ret = 0;
    memset(dp, 0, sizeof(int) * ASize);
    dp[0] = 0;
    dp[1] = 0;
    for (int i = 2; i < ASize; i++) {
        if (A[i] - A[i - 1] == A[i - 1] - A[i - 2]) {
            dp[i] = dp[i - 1]+1;
        }
    }
    for(int i=2;i<ASize;i++){
        ret+=dp[i];
    }
    return ret;
}
/*
int numberOfArithmeticSlices(int* A, int ASize) {
    if(ASize<3) return 0;
    int pred = A[1]-A[0];
    int i;
    int sum = 0;
    int count = 2;
    for(i=2; i<ASize; i++) {
        if(A[i]-A[i-1] == pred) count++;
        else {
            if(count>2) sum +=  count*(count-3)/2+1;
            count = 2;
            pred = A[i]-A[i-1];
        }
    }
    if(count>2) sum +=  count*(count-3)/2+1;
    return sum;
}
*/