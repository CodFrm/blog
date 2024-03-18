#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**
 * https://leetcode-cn.com/problems/longest-substring-without-repeating-characters/description/
 * 无重复字符的最长子串
 */

int lengthOfLongestSubstring(char *s)
{
    //初始化变量
    int len = strlen(s);
    int *hash = malloc(128 * sizeof(int));
    memset(hash, -1, sizeof(int) * 128);
    int maxlen = 0, nowlen = 0;
    for (int i = 0; i < len; i++, nowlen++)
    {
        if (hash[s[i]] != -1 && hash[s[i]] >= i - nowlen) //利用一个数组增加来判断是否重复
        {
            if (nowlen > maxlen)
            {
                maxlen = nowlen;
            }
            //上一次位置：hash[s[i]，现在这一次位置：i
            //重新计算长度 nowlen=i-hash[s[i]]-1
            nowlen = i - hash[s[i]] - 1;
        }
        hash[s[i]] = i;//记录位置
    }
    if (nowlen > maxlen) //最后的没有判断
    {
        maxlen = nowlen;
    }
    return maxlen;
}

int main()
{
    printf("%d\n", lengthOfLongestSubstring("abcabcbb"));
    printf("%d\n", lengthOfLongestSubstring("aab"));
    printf("%d\n", lengthOfLongestSubstring("dvdf"));
    printf("%d\n", lengthOfLongestSubstring("bbbbb"));
    printf("%d\n", lengthOfLongestSubstring("pwwkew"));
    getchar();
    return 0;
}