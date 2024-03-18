#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * https://leetcode-cn.com/problems/longest-common-prefix/description/
 * 最长公共前缀
 */
char *longestCommonPrefix(char strs[3][10], int strsSize)
{
    if (strsSize <= 0)
    {
        char *tmp = malloc(1);
        tmp[0] = 0;
        return tmp;
    }
    char *ret = strs[0];
    for (int i = 1; i < strsSize; i++)
    {
        char *tmp = &strs[i][0], *tmp1 = ret;
        int len = 0;
        while (*tmp1 == *tmp && *tmp!=0)
        {
            len++;
            tmp1++;
            tmp++;
        }
        ret[len]=0;
    }
    return ret;
}

int main()
{
    char a[3][10] = {"aa", "aa", "aa"};
    longestCommonPrefix(a, 3);
    getchar();
    return 0;
}