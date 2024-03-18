#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dfs(char** ret, int* num, char* S, int n, int use, int ruse) {
    if (use >= n && ruse >= n) {
        //左括号用完了
        ret[*num] = S;
        *num += 1;
        return;
    }
    if (use < n) {
        char* tmp = malloc((n * 2) + 1);
        memset(tmp, 0, (n * 2) + 1);
        strcpy(tmp, S);
        tmp[strlen(S)] = '(';
        dfs(ret, num, tmp, n, use + 1, ruse);
    }
    if (ruse < use) {
        char* tmp = malloc((n * 2) + 1);
        memset(tmp, 0, (n * 2) + 1);
        strcpy(tmp, S);
        S[strlen(S)] = ')';
        dfs(ret, num, S, n, use, ruse + 1);
    }
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char** generateParenthesis(int n, int* returnSize) {
    char** ret = malloc(pow(n, n) * sizeof(char*));
    char* S = malloc((n * 2) + 1);
    memset(S, 0, (n * 2) + 1);
    dfs(ret, returnSize, S, n, 0, 0);
    return ret;
}
// 1 2 5 14 42 132
int main() {
    int num = 0;
    char** ret = generateParenthesis(10, &num);
    for (int i = 0; i < num; i++) {
        printf("%s\n", ret[i]);
    }
    getchar();
    return 0;
}