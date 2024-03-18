#include <stdio.h>
#include <string.h>

typedef int bool;

#define false 0
#define true 1

bool isValid(char *s)
{
    int len = strlen(s), stackpos = 0;
    char stack[len / 2 + 1];
    int pos = 0;
    len = len / 2 + 1;
    while (s[pos] != '\x0')
    {
        if (stackpos >= len)
        {
            return false;
        }
        if (s[pos] == '(')
            stack[stackpos++] = ')';
        else if (s[pos] == '{')
            stack[stackpos++] = '}';
        else if (s[pos] == '[')
            stack[stackpos++] = ']';
        else if (stackpos <= 0 || s[pos] != stack[--stackpos])
            return false;

        pos++;
    }
    return stackpos == 0;
}

int main()
{
    isValid("(([])){}");
    return 0;
}