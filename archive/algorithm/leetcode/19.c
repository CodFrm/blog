#include <stdio.h>
#include <malloc.h>
#include <math.h>

// 删除链表的倒数第N个节点
// https://leetcode-cn.com/problems/remove-nth-node-from-end-of-list/

// 更优解:双指针
struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *removeNthFromEnd(struct ListNode *head, int n)
{
    if (head->next == NULL)
    {
        return NULL;
    }
    int pos = 0, len = n + 1;
    struct ListNode **arr = malloc(sizeof(struct ListNode *) * len);
    struct ListNode *temp = malloc(sizeof(struct ListNode));
    temp->next = head;
    head = temp;
    while (temp != NULL)
    {
        arr[pos % len] = temp;
        pos++;
        temp = temp->next;
    }
    pos = abs((pos - len) % len);
    if (arr[pos]->next == NULL)
    {
        arr[abs((pos - 1) % len)]->next = NULL;
    }
    else
    {
        arr[pos]->next = arr[pos]->next->next;
    }

    return head->next;
}

int main()
{
    struct ListNode *head = malloc(sizeof(struct ListNode)), *next = malloc(sizeof(struct ListNode)), *tmp;
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

    removeNthFromEnd(head, 1);
    return 0;
}