#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * 验证回文字符串
 * https://leetcode-cn.com/explore/interview/card/top-interview-questions-easy/5/strings/36/
 */
#define bool int
#define true 1
#define false 0
bool isPalindrome(char *s)
{
    int len = strlen(s);
    if (len < 1)
    {
        return true;
    }
    for (int start = 0, end = len - 1; start < end; start++, end--)
    {
        while (start < end) //遇到我们要判断数字字母的停止
        {
            if ((s[start] >= 48 && s[start] <= 57) ||
                (s[start] >= 97 && s[start] <= 122))
            {
            }
            else if (s[start] >= 65 && s[start] <= 90)
            {
                s[start] |= 32; //转换为小写
            }
            else
            {
                start++;
                continue;
            }
            break;
        }
        while (start < end) //结束的同上
        {
            if ((s[end] >= 48 && s[end] <= 57) ||
                (s[end] >= 97 && s[end] <= 122))
            {
            }
            else if (s[end] >= 65 && s[end] <= 90)
            {
                s[end] |= 32; //转换为小写
            }
            else
            {
                end--;
                continue;
            }
            break;
        }
        if (start >= end)
        {
            break;
        }
        if (s[start] != s[end])
        {
            return false;
        }
    }
    return true;
}
/*
int is_char(char c)
{
  return ( (c>='a' && c<='z') || (c>='A' && c<='Z') );
}

int is_num(char c)
{
  return (c>='0'&&c<='9');
}

int is_equal(char c1,char c2){
  if(is_num(c1))
    return c1 == c2; 
  else//c1 is charactrer
  {
    if(is_num(c2))
      return 0;
    
    c1 = c1  > 96 ? c1-32:c1;
    c2 = c2  > 96 ? c2-32:c2;
        
    return c1 == c2; 
  }

}

int isPalindrome(char* s) {
  int len;
  if(s == NULL || (len = strlen(s)) == 0)
    return 1;

  char *p = s;
  char *q = s + len-1;

  while(p < q)
  {
    if((!is_num(*p)) && (!is_char(*p)))
    {   
      p = p + 1;
      continue;
    }   

    if((!is_num(*q)) && (!is_char(*q)))
    {   
      q = q - 1;
      continue;
    }   

    if(is_equal(*p,*q) == 0)
      return 0;

    p = p + 1;
    q = q - 1;
  }
  return 1;
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
    char a[] = "A man, a plan, a canal: Panama";
    printf("%d\n", isPalindrome("qwewq"));
    printf("%d\n", isPalindrome(a));
    printf("%d\n", isPalindrome("race a car"));
    char b[] = "A man\r\na plan\r\na canal: Panama";
    printf("%d\n", isPalindrome(b));
    printf("%d\n", isPalindrome(".,"));
    getchar();
    return 0;
}