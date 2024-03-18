#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
/**
 * 二叉伸展树
 * https://blog.csdn.net/u014634338/article/details/49586689
 */

struct tree;
#define bool int
#define false 0
#define true 1
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
typedef struct tree *prt_tree;
struct tree
{
    int data;    //数据
    int height;  //树深度
    prt_tree lt; //左节点
    prt_tree rt; //右节点
};

prt_tree create_tree(int data)
{
    prt_tree tmp = malloc(sizeof(struct tree));
    memset(tmp, 0, sizeof(struct tree));
    tmp->data = data;
    return tmp;
}
prt_tree add_tree(int data, prt_tree root)
{
    if (root == NULL)
    {
        prt_tree tmp = create_tree(data);
        return tmp;
    }
    if (root->data > data)
    {
        root->lt = add_tree(data, root->lt);
    }
    else if (root->data < data)
    {
        root->rt = add_tree(data, root->rt);
    }
    return root;
}

void splay_tree(prt_tree root, prt_tree tree);
prt_tree find_tree(prt_tree root, int val)
{
    if (root == NULL)
    {
        return NULL;
    }
    prt_tree tmp = NULL;
    if (root->data > val)
    {
        tmp = find_tree(root->lt, val);
    }
    else if (root->data < val)
    {
        tmp = find_tree(root->rt, val);
    }
    if (tmp != NULL)
    {
        splay_tree(root, tmp);
    }
    return root;
}
void l_rotate(prt_tree a, prt_tree b);
void r_rotate(prt_tree a, prt_tree b);
void splay_tree(prt_tree root, prt_tree tree)
{
    if (tree == NULL)
    {
        return;
    }
    if (root->lt == tree)
    {
        l_rotate(root, tree);
    }
    else if (root->rt == tree)
    {
        r_rotate(root, tree);
    }
}

void l_rotate(prt_tree a, prt_tree b)
{
    int tmp = a->data;
    a->data = b->data; //交换ab数据
    b->data = tmp;
    prt_tree tmpNode = a->rt;
    a->rt = b;
    a->lt = b->lt;
    b->lt = b->rt;
    b->rt = tmpNode;
}
void r_rotate(prt_tree a, prt_tree b)
{
    int tmp = a->data;
    a->data = b->data;
    b->data = tmp; //交换ab数据
    prt_tree tmpNode = a->lt;
    a->lt = b;
    a->rt = b->rt;
    b->rt = b->lt;
    b->lt = tmpNode;
}
bool find(prt_tree root, int val)
{
    if (root->data == val)
    {
        return true;
    }
    prt_tree tmp = find_tree(root, val);
    if (tmp == NULL || tmp == root)
    {
        return false;
    }
    return true;
}

void print_tree(prt_tree root) //中序输出就是排序啦
{
    if (root == NULL)
    {
        return;
    }
    print_tree(root->lt);
    printf("%d\t", root->data);
    print_tree(root->rt);
}

int main()
{
    prt_tree root = create_tree(201);
    for (int i = 0; i < 20; i++)
    {
        root = add_tree(rand(), root);
    }
    int s = 0;
    scanf("%d", &s);
    find(root, s);
    print_tree(root);
    system("pause");
    return 0;
}
