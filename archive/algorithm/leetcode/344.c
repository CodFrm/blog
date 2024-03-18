#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/reverse-string/description/
 * 反转字符串
 */
char *reverseString(char *s)
{
    int len = strlen(s);
    for (int i = 0; i < len/2; i++)
    {
        s[i] ^= s[len - 1 - i];
        s[len - 1 - i] ^= s[i];
        s[i] ^= s[len - 1 - i];
    }
    return s;
}

void print_r(int *a, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

int main()
{
    char s[] = "hello";
    printf("%s\n", reverseString(s));
    getchar();
    return 0;
}