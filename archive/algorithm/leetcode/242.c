#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/valid-anagram/description/
 * 有效的字母异位词
 */

bool isAnagram(char *s, char *t)
{
    int hash[26] = {0}, hash2[26] = {0}, i = 0;
    while (1)
    {
        if (s[i] == '\x0')
        {
            if (t[i] == '\x0')
            {
                break;
            }
            return 0;
        }
        if (t[i] == '\x0')
        {
            return 0;
        }
        hash[s[i] - 'a']++;
        hash2[t[i] - 'a']++;
        i++;
    }
    for (int i = 0; i < 26; i++)
    {
        if (hash[i] != hash2[i])
        {
            return 0;
        }
    }
    return 1;
}
int main()
{
    getchar();
    return 0;
}