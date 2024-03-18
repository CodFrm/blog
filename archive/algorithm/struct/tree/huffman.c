#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hfm *HaffmanTree;

struct hfm
{
    HaffmanTree node;
    int val;
    int priority;
};

int hashQ[26]; //权
HaffmanTree tree;
//构建列表
HaffmanTree buildList(char *str)
{
    //先计算权
    for (int i = 0; i < strlen(str); i++)
    {
        hashQ[str[i] - 'a']++;
    }
    return NULL;
}

int main()
{
    tree = malloc(hfm) char str[] = "athisteststring";

    buildList(str);

    return 0;
}