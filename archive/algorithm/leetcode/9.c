#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * https://leetcode-cn.com/problems/palindrome-number/description/
 * 回文数
 */
void atoi_(int x, char *s);
int isPalindrome(int x)
{
    if (x < 0) //负数直接返回false
    {
        return 0;
    }
    char s[16]={0};
    atoi_(x, s);
    int len = strlen(s), n = len / 2;
    for (int i = 0; i < n; i++)
    {
        if (s[i] != s[len - i - 1])
        {
            return 0;
        }
    }
    return 1;
}
void atoi_(int x, char *s)
{
    int i = 0;
    while (x > 0)
    {
        s[i++] = 48 + x % 10;
        x /= 10;
    }
}
/*
bool isPalindrome(int x) {
    int y=0;
    int z=x;
    int p=0;
    if(z<0){
        return false;
    }
    while(z>0){
        p=z%10;
        y=y*10+p;
        z=z/10;
    }
    if(y==x){
        return true;
    }
    return false;   
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
    printf("%d", isPalindrome(12321));
    printf("%d", isPalindrome(-121));
    printf("%d", isPalindrome(1024));
    printf("%d", isPalindrome(1));
    getchar();
    return 0;
}