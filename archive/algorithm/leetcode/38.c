#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/count-and-say/description/
 * 报数
 */
//111221
//312211
//13112221
char *countAndSay(int n)
{
    char *s = malloc(n*n*n), *tmp_ret = malloc(n*n*n);
    int len = 1;
    memset(s, 0, n*n*n);
    memset(tmp_ret, 0, n*n*n);
    strcpy(s, "1");
    for (int i = 1; i < n; i++)
    {
        char last_chr = s[0];
        int last_num=1;
        for (int n = 1; n < len; n++)
        {
            if (s[n] != last_chr)
            {
                char tmp[3] = {0};
                tmp[0] = '0' + last_num;
                tmp[1] = last_chr;
                strcat(tmp_ret, tmp);
                last_chr = s[n];
                last_num = 1;
            }
            else
            {
                last_num++;
            }
        }
        char tmp[3] = {0};
        tmp[0] = '0' + last_num;
        tmp[1] = last_chr;
        strcat(tmp_ret, tmp);
        strcpy(s, tmp_ret);
        memset(tmp_ret, 0, n*n*n);
        len = strlen(s);
    };
    free(tmp_ret);
    return s;
}
/*
char* countAndSay(int n) {
    char *num = (char*)malloc(sizeof(char)*2);
    char *num1, *p, *q;
    int index;
    // char c;
    
    num[0] = '1';
    num[1] = '\0';
    while(n>1){
        index = 0;
        p = q = num;
        num1 = (char*)malloc(sizeof(char)*strlen(num)*2+1);
        
        while(*q){
            if(*p != (*(q + 1))){
                num1[index++] = q - p + '1';
                num1[index++] = *p;
                p = q + 1;
            }
            q++;
        }
        num1[index] = '\0';
        
        free(num);
        num = num1;
        
        n--;
    }
    
    return num;
}
*/

int main()
{
    countAndSay(40);
    countAndSay(20);
    countAndSay(10);
    countAndSay(12);
    countAndSay(14);
    getchar();
    return 0;
}