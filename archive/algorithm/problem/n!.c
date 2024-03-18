#include <stdio.h>
// 使用栈的方式计算阶乘
struct stack
{
    int val[100];
    int pos;
} s;

void push(int i)
{
    s.val[s.pos++] = i;
}

int pop()
{
    if (s.pos == 0)
    {
        return 0;
    }
    return s.val[--s.pos];
}

int fac2(int n)
{
    int result = 1;
    for (int i = n; i > 0; i--)
    {
        result *= i;
    }
    return result;
}

int fac1(int n)
{
    if (n == 1)
        return 1;
    else
        return n * fac1(n - 1);
}

//这算么？
int fac3(int n)
{
    do
    {
        push(n);
    } while (--n);

    int i = 0, result = 1;
    while (i = pop())
    {
        result *= i;
    }
    return result;
}

int main()
{
    printf("%d\n", fac1(10));
    printf("%d\n", fac2(10));
    printf("%d", fac3(10));
    getchar();
}