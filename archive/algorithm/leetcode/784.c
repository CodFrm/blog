#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dfs(char** ret, int* num, char* S, int pos) {
    if (pos > strlen(S)) {
        ret[*num] = S;
        *num += 1;
        return;
    }
    dfs(ret, num, S, pos + 1);
    if ((S[pos] >= 'a' && S[pos] <= 'z') || (S[pos] >= 'A' && S[pos] <= 'Z')) {
        //是字母
        char* b = malloc(strlen(S));
        strcpy(b, S);
        b[pos] ^= 32;
        dfs(ret, num, b, pos + 1);
    }
}

char** letterCasePermutation(char* S, int* returnSize) {
    char** ret = malloc(pow(2, strlen(S)) * sizeof(char*));
    dfs(ret, returnSize, S, 0);
    return ret;
}

int main() {
    char s[] = "qwertyuiopas";
    int num = 0;
    char** ret = letterCasePermutation(s, &num);
    for (int i = 0; i < num; i++) {
        printf("%s\n", ret[i]);
    }
    getchar();
    return 0;
}