#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/explore/interview/card/top-interview-questions-easy/5/strings/37/
 * 字符串转整数（atoi）
 */
#define INT_MIN -2147483648
#define INT_MAX 0x7fffffff
int myAtoi(char *str)
{
    long long ret = 0;
    char flag = 1, len = 0;
    while (*str == ' ') //删除空
    {
        str++;
    }
    if (*str == '-')
    {
        flag = -1;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }
    else if (*str < '0' || *str > '9')
    {
        return 0;
    }
    while (*str >= '0' && *str <= '9' && len < 11)
    {
        ret = ret * 10 + (*str - '0');
        str++;
        if (ret != 0)
        {
            len++;
        }
    }
    ret *= flag;
    if (ret < INT_MIN)
    {
        return INT_MIN;
    }
    if (ret > INT_MAX)
    {
        return INT_MAX;
    }
    return ret;
}
/*
int myAtoi(char* str) {
    if(strlen(str)==0)
        return 0;
    char* xianzai= str;
    long long fanhui = 0;
    int flag = 1;//记录符号位正负
    long long max = pow(2,31)-1;
    long long min = pow(2,31);//溢出的上下界不相同
    while (*xianzai == ' ')
    {
        xianzai++;//跳过空格位置
    }
    if (*xianzai == '-')
    {
        flag = -1;
        xianzai++;
    }
    else if(*xianzai == '+')
    {
        xianzai++;
    }
    else if (*xianzai < '0' || *xianzai > '9')
    {
             return 0;
    }
    while (*xianzai >= '0' && *xianzai <= '9')
    {
        fanhui= fanhui*10;
        fanhui=fanhui+*xianzai-'0';//利用符号本身的ascii码直接转换成数字
        if(flag==1 && fanhui>max)
        {
            //正数溢出，返回INT_MAX
            return max;
        }
        else if(flag==-1&&fanhui>min)
        {
            return min;//注意int类型上下界数值上相差1
        }
        xianzai++;
    }
    return flag*fanhui;
}
*/
int main()
{
    printf("%d\n", myAtoi("-1"));
    printf("%d\n", myAtoi("456"));
    printf("%d\n", myAtoi("   -42"));
    printf("%d\n", myAtoi("a65r"));
    printf("%d\n", myAtoi("9223372036854775808"));
    printf("%d\n", myAtoi("-91283472332"));
    printf("%d\n", myAtoi("4193 with words"));
    printf("%ld,%ld\n", -pow(2, 31), pow(2, 31) - 1);
    getchar();
    return 0;
}