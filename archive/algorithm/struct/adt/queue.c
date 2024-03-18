#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
/**
 * 数据结构,队列   感觉和栈差不多吧,一个先进后出,一个先进先出
 * 用双向链表的速度将快一些,本例采用双向链表,就不需要再检索倒数第二个节点,记录最后一个节点即可
 */
struct queue;
typedef struct queue *prt_queue;
//3->2->1->3
struct queue
{
    int data;
    prt_queue last; //上一个
    prt_queue next; //下一个
};

//3<=>2<=>1
//3<=>2
//3
/**
 * 出队列
 */
int pop(prt_queue *end)
{
    if (*end == NULL)
    {
        return 0;
    }
    int ret_data = (*end)->data;
    prt_queue *tmp = end;
    *end = (*end)->last;
    if ((*end) != NULL)
    {
        (*end)->next = NULL;
    }
    free(tmp);
    return ret_data;
}
//1
//2<=>1
//3<=>2<=>1
/**
 * 进入队列
 */
prt_queue push(int data, prt_queue *h)
{
    prt_queue tmp_node = malloc(sizeof(struct queue));
    (*h)->last = tmp_node;
    memset(tmp_node, 0, sizeof(struct queue));
    tmp_node->data = data;
    tmp_node->next = *h;
    *h = tmp_node;
    return tmp_node;
}
/**
 * 创建栈
 */
prt_queue create_queue(int data)
{
    prt_queue header = malloc(sizeof(struct queue));
    memset(header, 0, sizeof(struct queue));
    header->data = data;
    return header;
}

void print_queue(prt_queue *end)
{
    int data = 0;
    while ((data = pop(end)) != 0)
    {
        printf("%d\t", data);
    }
}
#ifndef MAIN_FUNC
int main()
{
    prt_queue queue = create_queue(1);
    prt_queue end_node = queue;
    push(23, &queue);
    push(15, &queue);
    push(123, &queue);
    push(36, &queue);
    push(89, &queue);
    push(16, &queue);
    push(324, &queue);
    push(526, &queue);
    push(166, &queue);
    push(984, &queue);
    push(85, &queue);
    pop(&end_node);
    pop(&end_node);
    print_queue(&end_node);
    getchar();
    return 0;
}
#endif