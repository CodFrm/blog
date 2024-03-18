#include <stdio.h>
#include <malloc.h>

//https://leetcode-cn.com/problems/reverse-linked-list/
//反转链表
struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *reverseList(struct ListNode *head)
{
    if (head == NULL)
    {
        return NULL;
    }
    struct ListNode *tail = head, *next = head->next, *tmp = NULL;
    while (next != NULL)
    {
        tmp = next->next;
        tail->next = tmp;
        next->next = head;
        head = next;
        next = tmp;
    }
    return head;
}

// a->b->c->d->e
// b->a->c->d->e
// c->b->a->d->e
// c->b->a->d->e

int main()
{
    struct ListNode *head = malloc(sizeof(struct ListNode)), *next = malloc(sizeof(struct ListNode));
    head->val = 1;
    head->next = next;
    next->val = 2;
    next->next = malloc(sizeof(struct ListNode));
    next = next->next;

    next->val = 3;
    next->next = malloc(sizeof(struct ListNode));
    next = next->next;

    next->val = 4;
    next->next = malloc(sizeof(struct ListNode));
    next = next->next;

    next->val = 5;
    next->next = NULL;

    reverseList(head);
    return 0;
}