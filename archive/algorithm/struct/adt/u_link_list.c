#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
/**
 * 单向链表 增,删,查,改
 */
struct node;
typedef struct node *prt_node;

prt_node insert(int, int, prt_node);
prt_node find_node(int, prt_node);
void print_list(prt_node);
prt_node after_node(prt_node);
void del(int, prt_node);
void modify(int, int, prt_node);

struct node
{
    int data;
    prt_node next;
};
/**
 * 在n位置后面插入一个数据,n为-1时插入到最后
 */
prt_node insert(int n, int data, prt_node h)
{
    prt_node tmp_node;
    prt_node last_node = n == -1 ? after_node(h) : find_node(n, h); //取得前一个节点
    tmp_node = last_node->next;
    last_node->next = malloc(sizeof(struct node));
    last_node->next->next = tmp_node;
    last_node->next->data = data;
    tmp_node = last_node->next;
    return tmp_node; //返回插入的节点
}
/**
 * 寻找前一个节点,n为-1时查找最后节点
 */
prt_node find_node(int n, prt_node h)
{
    prt_node ret = h;
    for (int i = 0; i < n; i++)
    {
        if (ret->next == NULL)
        {
            break;
        }
        ret = ret->next;
    }
    return ret;
}
/**
 * 最后一个
 */
prt_node after_node(prt_node h)
{
    while (h->next != NULL)
        h = h->next;
    return h;
}
/**
 * 删除数据 
 */
void del(int n, prt_node h)
{
    h = find_node(n, h);
    prt_node tmp_node = h->next;
    h->next = tmp_node->next;
    free(tmp_node);
}
/**
 * 修改数据
 */
void modify(int n, int data, prt_node h)
{
    prt_node tmp_node = find_node(n, h)->next;
    tmp_node->data = data;
}
/**
 * 输出链表
 */
void print_list(prt_node h)
{
    while ((h = h->next) != NULL)
    {
        printf("%d\t", h->data);
    }
    printf("\n");
}

int main()
{
    prt_node header = memset(malloc(sizeof(struct node)), 0, sizeof(struct node));
    //添加
    insert(-1, 153, header);
    insert(-1, 2343, header);
    insert(-1, 323, header);
    insert(-1, 564, header);
    //删除
    del(2, header);
    //插入
    insert(2, 23333, header);
    insert(4, 312, header);
    //修改
    modify(0, 888, header);
    print_list(header);
    getchar();
    return 0;
}