#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef int bool;

#define false 0
#define true 1

#define MAX_NUM 500
char *backspace(char *s)
{
    char stack[MAX_NUM];
    int len = 0, pos = 0;
    while (s[pos] != '\x0')
    {
        if (s[pos] == '#')
        {
            if (len > 0)
            {
                stack[len--] = '\x0';
            }
        }
        else
        {
            stack[len++] = s[pos];
        }
        pos++;
    }
    char *ret = malloc(sizeof(char) * (len + 1));
    for (int i = 0; i < len; i++)
    {
        ret[i] = stack[i];
    }
    ret[len] = '\x0';
    return ret;
}

bool backspaceCompare(char *S, char *T)
{
    char *s1 = backspace(S), *s2 = backspace(T);
    return strcmp(s1, s2) == 0;
}

int main()
{
    bool a = backspaceCompare("y#f#o##f", "y#fo##f");
}