#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/implement-strstr/description/
 * 实现strStr()
 */
//a
int strStr(char *haystack, char *needle)
{
    if (*needle == '\x0')
    {
        return 0;
    }
    int pos = 0;
    int sublen = strlen(needle);
    while (*haystack != '\x0')
    {
        if (*haystack == *needle)
        {
            char *sub = haystack;
            char *tmp = needle;
            int len = 0;
            while (*sub == *tmp && len < sublen)
            {
                len++;
                sub++;
                tmp++;
            }
            if (len == sublen)
            {
                return pos;
            }
        }
        haystack++;
        pos++;
    }
    return -1;
}
/*
int strStr(char* haystack, char* needle) {
    int i=0, j=0, temp;
    
    if( *needle == 0 ) return 0;
    if( strlen(haystack) < strlen(needle) ) return -1;
    
    while( haystack[i] != 0) {
        while( haystack[i] != 0 && haystack[i] != needle[j] ) i++;
        if( haystack[i] == 0 ) return -1;
        i++; j++;
        temp = i;
        while( haystack[i] != 0 && needle[j] != 0 ) {
            if( haystack[i] != needle[j] ) {
                j = 0;
                i = temp;
                break;
            }
            i++; j++;
        }
       // printf("haystack = %d, sizeof(needle) = %d, haystackTmp = %d", haystack, strlen(needleTmp), haystackTmp);
        if( needle[j] == 0 ) return (i - j);
    }
    return -1;
}
*/

int main()
{
    printf("%d\n", strStr("hello", "lll"));
    getchar();
    return 0;
}