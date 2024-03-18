#include <stdio.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

typedef int bool;
#define true 1;
#define false 0;

bool hasCycle(struct ListNode *head)
{
    if (head == NULL)
    {
        return false;
    }
    struct ListNode *k = head->next, *m = head;
    while (k != m)
    {
        if (m == NULL || k == NULL)
        {
            return false;
        }
        m = m->next;
        k = k->next;
        if (k == NULL)
        {
            return false;
        }
        k = k->next;
    }
    return true;
}

int main()
{
    struct ListNode *head = malloc(sizeof(struct ListNode)), *next = malloc(sizeof(struct ListNode)), *tmp;
    head->val = 3;
    head->next = next;
    tmp = next;
    next->val = 2;
    next->next = malloc(sizeof(struct ListNode));
    next = next->next;

    next->val = 0;
    next->next = malloc(sizeof(struct ListNode));
    next = next->next;

    next->val = -4;
    next->next = NULL;

    hasCycle(head);
    return 0;
}