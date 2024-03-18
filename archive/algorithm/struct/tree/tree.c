#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
/**
 * 树
 */
struct tree;
typedef struct tree *prt_tree;

struct tree
{
    int data;
    prt_tree child;   //子节点链表
    prt_tree brother; //兄弟节点链表
};

prt_tree create_tree()
{
    void *tmp = malloc(sizeof(struct tree));
    memset(tmp, 0, sizeof(struct tree));
    return tmp;
}
/**
 * 最后一个兄弟节点
 */
prt_tree end_node_brother(prt_tree node)
{
    while (1)
    {
        if (node->brother != NULL)
        {
            node = node->brother;
        }
        else
        {
            break;
        }
    }
    return node;
}

/**
 * 最后一个子节点
 */
prt_tree end_node_child(prt_tree node)
{
    while (1)
    {
        if (node->child != NULL)
        {
            node = node->child;
        }
        else
        {
            break;
        }
    }
    return node;
}
/**
 * 子节点增加
 */
prt_tree child_add(int data, prt_tree root)
{
    //判断子节点是不是空的,不是空就插入子节点的最后一个
    //应该子节点是只有一个的,其余的子节点通过兄弟节点链接
    prt_tree node = malloc(sizeof(struct tree));
    prt_tree child;
    memset(node, 0, sizeof(struct tree));
    child = end_node_child(root);
    child->child = node;
    node->data = data;
    return node;
}

/**
 * 兄弟节点增加
 */
prt_tree brother_add(int data, prt_tree root)
{
    //判断兄弟节点是不是空的,不是空就插入至兄弟节点的最后一个
    prt_tree node = malloc(sizeof(struct tree));
    prt_tree brother;
    memset(node, 0, sizeof(struct tree));
    brother = end_node_brother(root);
    brother->brother = node;
    node->data = data;
    return node;
}
/**
 * 添加树的内容
 */
void add_tree(prt_tree root)
{
    char action = 0;
    printf("当前节点数据:%d 操作:\n", root->data);
    scanf("%s", &action);
    switch (action)
    {
    case 'i':
    {
        int data = 0;
        printf("子节点数据:\n");
        scanf("%d", &data);
        prt_tree child = child_add(data, root);
        add_tree(child);
        //子节点插入
        break;
    }
    case 'o':
    {
        int data = 0;
        printf("兄弟节点数据:\n");
        scanf("%d", &data);
        prt_tree child = brother_add(data, root);
        add_tree(child);
        //兄弟节点插入
        break;
    }
    case 'q':
        //滚回上一个节点
        return;
    default:
        break;
    }
    add_tree(root);
}

/**
 * 输出数
 */
void print_tree(prt_tree root)
{
    if (root != NULL)
    {
        //输出子节点
        printf("%d的子节点:\n", root->data);
        prt_tree child = root->child;
        print_tree(child);
        //输出兄弟节点
        printf("%d的兄弟节点:\n", root->data);
        prt_tree brother = root->brother;
        print_tree(brother);
        printf("%d\n", root->data);
    }
}

int main()
{
    prt_tree root = NULL;
    root = create_tree();
    add_tree(root);
    print_tree(root);
    system("pause");
    return 0;
}