#include <stdio.h>
#include <malloc.h>
#include <math.h>

// 链表的中间结点
// https://leetcode-cn.com/problems/middle-of-the-linked-list/

struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *middleNode(struct ListNode *head)
{
    int len = 0;
    struct ListNode *p1 = head, *p2 = head->next;
    while (p2 != NULL)
    {
        p1 = p1->next;
        p2 = p2->next;
        len++;
        if (p2 == NULL)
        {
            break;
        }
        p2 = p2->next;
    }
    return p1;
}