#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) a > b ? b : a
#define bool char
#define false 0
#define true 1

typedef struct trie *trieNode;
typedef struct trie Trie;

// 有bug的trie压缩树

struct trie
{
    char *path;       //节点路径
    int data;         //节点存放数据
    trieNode brother; //兄弟节点
    trieNode next;    //下面的节点
};
trieNode tree;

trieNode newNode()
{
    trieNode tmp = malloc(sizeof(Trie));
    memset(tmp, 0, sizeof(Trie));
    tmp->path = "";
    return tmp;
}

//插入数据
void insert(trieNode tree, char *path, int data)
{
    //判断节点路径
    if (tree->path == "")
    {
        //空路径直接插入
        tree->path = path;
        tree->data = data;
        return;
    }
    //判断路径耦合,path大于节点路径,往next节点查找
    //节点路径大于path,往brother找 (大小指相等的长度)
    bool isPath = false; //表示path长度大于tree->path
    int minLen = strlen(path);
    if (strlen(path) > strlen(tree->path))
    {
        isPath = true;
        minLen = strlen(tree->path);
    }
    int pathPos = 0;
    for (pathPos = 0; pathPos < minLen; pathPos++)
    {
        if (path[pathPos] != tree->path[pathPos])
        {
            break;
        }
    }
    if (pathPos == 0) //为0是全部不等,继续向下一个兄弟节点查询,除非下一个兄弟节点为null,直接插入
    {
        if (tree->brother == NULL)
        {
            trieNode tmp = newNode();
            tmp->data = data;
            tmp->path = path;
            tree->brother = tmp;
        }
        else
        {
            insert(tree->brother, path, data);
        }
    }
    else if (pathPos != minLen) //没有全部的相等
    {
        if (tree->next == NULL)
        {
            tree->next = newNode();
        }
        insert(tree->next, &path[pathPos], data);
        insert(tree->next, &tree->path[pathPos], tree->data);
        char *tmp = malloc(pathPos + 1);
        memcpy(tmp, tree->path, pathPos);
        tree->path = tmp;
        tree->data = 0;
    }
    else //全部的相等
    {
        if (isPath) //全等,并且path大,继续往next节点查找
        {
            insert(tree->next, &path[pathPos], data);
        }
        else //全等,node->path大,
        {
            //空节点
            if (tree->next == NULL)
            {
                tree->next = newNode();
            }
            char *tmp = malloc(pathPos + 1);
            memcpy(tmp, tree->path, pathPos);
            insert(tree->next, &tree->path[pathPos], tree->data);
            tree->path = tmp;
            tree->data = data;
        }
    }
}

trieNode search(trieNode tree, char *path)
{
    if (tree == NULL)
    {
        return NULL;
    }
    int pos1 = 0, pos2 = 0;
    for (; pos2 < strlen(path); pos2++, pos1++)
    {
        if (tree->path[pos1] != path[pos2])
        {
            if (pos1 == 0) //为0则表示一个都不相等,往兄弟节点查询
            {
                pos1 = -1;
                pos2 -= 1;
                tree = tree->brother;
            }
            else
            {
                //否则没了
                return NULL;
            }
        }
        else if (pos1 == strlen(tree->path) - 1) //如果全部相等,node->path查找完毕,继续判断pos2是不是相等
        {
            if (pos2 == strlen(path) - 1) //传入的path也全部扫描完毕,则当前节点就是搜索到的节点
            {
                return tree;
            }
            else //否则,pos1重置为0,从下一个节点继续查找
            {
                pos1 = -1;
                tree = tree->next;
            }
        }
    }

    return NULL;
}

int main()
{
    tree = newNode();
    insert(tree, "/user/login", 111);
    insert(tree, "/user/register", 222);
    insert(tree, "/user/logout", 333);
    insert(tree, "/user/message", 444);
    insert(tree, "/user/mess", 555);

    trieNode tmp = search(tree, "/user/mess");
}
