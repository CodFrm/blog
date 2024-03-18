#include <stdio.h>

typedef struct Stack Stack;
// 大数组
// TODO: 优化(骗人的todo)
typedef struct Stack
{
    int val;
    Stack *next;
};

typedef struct
{
    Stack *min_stack;
    Stack *stack;
} MinStack;

/** initialize your data structure here. */

Stack *NewStack(int val)
{
    Stack *ret = malloc(sizeof(Stack));
    ret->val = val;
    ret->next = NULL;
    return ret;
}

MinStack *minStackCreate()
{
    MinStack *ret = malloc(sizeof(MinStack));
    ret->min_stack = NewStack(-1);
    ret->stack = NewStack(-1);
    return ret;
}

void minStackPush(MinStack *obj, int x)
{
    Stack *tmp = obj->min_stack->next, *prev = obj->min_stack;
    while (tmp != NULL)
    {
        if (x < tmp->val)
        {
            prev->next = NewStack(x);
            prev->next->next = tmp;

            tmp = obj->stack->next;
            obj->stack->next = NewStack(x);
            obj->stack->next->next = tmp;
            return;
        }
        tmp = tmp->next;
        prev = prev->next;
    }
    //最大
    prev->next = NewStack(x);

    tmp = obj->stack->next;
    obj->stack->next = NewStack(x);
    obj->stack->next->next = tmp;
}

// void

void minStackPop(MinStack *obj)
{
    int val = obj->stack->next->val;
    Stack *tmp = obj->stack->next, *prev = obj->min_stack;
    obj->stack->next = tmp->next;
    free(tmp);
    tmp = obj->min_stack->next;
    while (tmp != NULL)
    {
        if (val == tmp->val)
        {
            prev->next = tmp->next;
            free(tmp);
            return;
        }
        tmp = tmp->next;
        prev = prev->next;
    }
}

int minStackTop(MinStack *obj)
{
    return obj->stack->next->val;
}

int minStackGetMin(MinStack *obj)
{
    return obj->min_stack->next->val;
}

void minStackFree(MinStack *obj)
{
    freeStack(obj->stack);
    freeStack(obj->min_stack);
}

void freeStack(Stack *stack)
{
    Stack *tmp;
    while (stack != NULL)
    {
        tmp = stack;
        stack = stack->next;
        free(tmp);
    }
}

/**
 * Your MinStack struct will be instantiated and called as such:
 * MinStack* obj = minStackCreate();
 * minStackPush(obj, x);
 
 * minStackPop(obj);
 
 * int param_3 = minStackTop(obj);
 
 * int param_4 = minStackGetMin(obj);
 
 * minStackFree(obj);
*/

int main()
{
    MinStack *minStack = minStackCreate();
    minStackPush(minStack, -2);
    minStackPush(minStack, 0);
    minStackPush(minStack, -3);
    minStackGetMin(minStack);
    minStackPop(minStack);
    minStackTop(minStack);
    minStackGetMin(minStack);

    minStackFree(minStack);
}