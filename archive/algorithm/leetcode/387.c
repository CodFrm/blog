#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/first-unique-character-in-a-string/description/
 * 字符串中的第一个唯一字符
 */

int firstUniqChar(char *s)
{
    struct hash_pos
    {
        char num;
        short pos;
    };
    struct hash_pos hash[26] = {0};
    int pos = -1, i = 0;
    while (s[i] != '\x0')
    {
        hash[s[i] - 97].num += 1;
        hash[s[i] - 97].pos = i++;
    }
    for (int i = 0; i < 26; i++)
    {
        if (hash[i].num == 1 && (pos > hash[i].pos || pos == -1))
        {
            pos = hash[i].pos;
        }
    }
    return pos;
}
/*
int firstUniqChar(char* s) {
    int i,j;
    int len=strlen(s);
    int b[26];
    for(int k=0;k<26;k++)
    {
        b[k]=0;
    }
    for(i=0;i<len;i++)
    {
        b[s[i]-'a']+=1;
    }
    for(j=0;j<len;j++)
    {
        if(b[s[j]-'a']==1)
        {
            return j;
        }
    }
    return -1;
}
*/
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
    char s[] = "qq";
    firstUniqChar(s);
    getchar();
    return 0;
}