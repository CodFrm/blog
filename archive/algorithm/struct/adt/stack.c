#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
/**
 * 数据结构,栈
 */
struct stack;
typedef struct stack *prt_stack;

struct stack
{
    int data;
    prt_stack last;
};

//1<-2   header=2 header->last=1
//1      header=1 header->last=null
/**
 * 出栈
 */
int pop(prt_stack *h)
{
    if (*h == NULL)
    {
        return 0;
    }
    int ret_data = (*h)->data;
    prt_stack *tmp = h;
    *h = (*h)->last;
    free(tmp);
    return ret_data;
}
//1   header=1   header->last=null
//1<-2  header=2 header->last=1
/**
 * 入栈
 */
prt_stack push(int data, prt_stack *h)
{
    prt_stack tmp_node = malloc(sizeof(struct stack));
    memset(tmp_node, 0, sizeof(struct stack));
    tmp_node->data = data;
    tmp_node->last = *h;
    *h = tmp_node;
    return tmp_node;
}
/**
 * 创建栈
 */
prt_stack create_stack(int data)
{
    prt_stack header = malloc(sizeof(struct stack));
    memset(header, 0, sizeof(struct stack));
    header->data = data;
    return header;
}

void print_stack(prt_stack *h)
{
    int data = 0;
    while ((data = pop(h)) != 0)
    {
        printf("%d\t", data);
    }
}
#ifndef MAIN_FUNC
int main()
{
    prt_stack stack = create_stack(1);
    push(23, &stack);
    push(15, &stack);
    push(123, &stack);
    push(36, &stack);
    push(89, &stack);
    push(16, &stack);
    push(324, &stack);
    push(526, &stack);
    push(166, &stack);
    push(984, &stack);
    push(85, &stack);
    pop(&stack);
    print_stack(&stack);
    getchar();
    return 0;
}
#endif