#include <stdio.h>
/**
 * 调试Makefile
*/
#define MIN(a, b) (a > b ? b : a)
int main()
{
    printf("Hello World!\n");
    int a = 1, b = 2, c = 3;
    int d = (a > b) && (c++);
    printf("%d\t%d\n", c, MIN(100, 20));
    getchar();
    return 0;
}
