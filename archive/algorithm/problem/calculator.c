#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAIN_FUNC
#include "../struct/stack.c"
/**
 * 用栈实现计算器
 * 先将中缀表达式转换成后缀表达式
 */

/**
 * 字符串转int
 */
int str2int(const char *str, int len)
{
    int result = 0;
    for (int i = 0; i < len; i++)
    {
        double t = pow(10, i);
        result += (str[len - i - 1] - 48) * t;
    }
    return result;
}
/**
 * 数字转换运算符,有内存泄漏问题,emmm不管了,反正我只管跑
 */
const char *get_symbol(int symbol)
{
    char *ret = malloc(2);
    memset(ret, 0, 2);
    switch (symbol)
    {
    case 0:
    {
        ret[0] = '+';
        break;
    }
    case 2:
    {
        ret[0] = '*';
        break;
    }
    }
    return ret;
}
/**
 * 生成后缀表达式 
 */
void suffix_expression(const char *exp, char *sufExp)
{
    prt_stack pSal = create_stack(-1);
    int last = 0;
    for (int i = 0; i < strlen(exp) + 1; i++)
    {
        if (exp[i] < '0' || exp[i] > '9') //提取数字
        {
            memcpy(&sufExp[strlen(sufExp)], &exp[last], i - last);
            sufExp = strcat(sufExp, "|"); //|用作分割
            last = i + 1;
            int symbol = 0; //123*+45*6+7*+
            if (exp[i] != 0)
            {
                switch (exp[i])
                {
                case '+':
                {
                    symbol = 0;
                    break;
                }
                case '*':
                {
                    symbol = 2;
                    break;
                }
                case '(':
                {
                    symbol = 5;
                    break;
                }
                case ')':
                {
                    symbol = 6;
                    break;
                }
                }
                push(symbol, &pSal);
            }
            int lastSymbol = 0;
            while (lastSymbol >= 0)
            {
                symbol = pop(&pSal);     //本次运算符
                lastSymbol = pop(&pSal); //上一次运算符
                if (symbol == 6)         //遇到)时,弹出栈元素,直到遇到(
                {
                    if (lastSymbol != 5)
                    {
                        sufExp = strcat(sufExp, get_symbol(lastSymbol));
                        push(symbol, &pSal);
                    }
                }
                else if (symbol > lastSymbol || lastSymbol == 5) //上一个为(不弹出
                {
                    push(lastSymbol, &pSal);
                    push(symbol, &pSal);
                    break;
                }
                else
                {
                    sufExp = strcat(sufExp, get_symbol(lastSymbol));
                    push(symbol, &pSal);
                }
            }
        }
    }

    int lastSymbol = 0;
    while ((lastSymbol = pop(&pSal)) >= 0) //将剩下的全部弹出
    {
        sufExp = strcat(sufExp, get_symbol(lastSymbol));
    }
}
/**
 * 计算后缀表达式
 */
int calculate(char *exp)
{
    //先将字符处理入栈
    prt_stack pSal = create_stack(-1);
    int len = strlen(exp);
    for (int i = 0; i < len; i++) //先处理分割符号|,替换成0x00
    {
        if (exp[i] == '|')
        {
            exp[i] = 0;
        }
    }
    int last = 0;
    for (int i = 0; i < len + 1; i++)
    {
        if (exp[i] == 0 || (exp[i] < '0' || exp[i] > '9')) //提取数字(符号)
        {
            int result = -1;
            switch (exp[i])
            {
            case '+':
            {
                result = pop(&pSal) + pop(&pSal);
                break;
            }
            case '*':
            {
                result = pop(&pSal) * pop(&pSal);
                break;
            }
            default:
            {
                if (i != last)
                {
                    push(str2int(&exp[last], i - last), &pSal);
                }
            }
            }
            if (result != -1)
            {
                push(result, &pSal);
            }
            // memcpy(&sufExp[strlen(sufExp)], &exp[last], i - last);
            // sufExp = strcat(sufExp, "|"); //|用作分割
            last = i + 1;
            // int symbol = 0; //123*+45*6+7*+
        }
    }
    return pop(&pSal);
}
int main()
{
    char exp[128] = {0}, *sufExp = malloc(128);
    memset(sufExp, 0, 128);
    printf("请输入前缀表达式:\n");
    scanf("%s", exp);
    suffix_expression(exp, sufExp);
    printf("后缀表达式为:%s\n", sufExp);
    int result = calculate(sufExp);
    printf("计算结果为:%d\n", result);
    system("pause");
    return 0;
}