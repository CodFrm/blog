#include <stdio.h>
#include <stdlib.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *mergeTwoLists(struct ListNode *l1, struct ListNode *l2)
{
    if (l1 == NULL)
    {
        return l2;
    }
    if (l2 == NULL)
    {
        return l1;
    }
    struct ListNode *node = l1, *node2 = l2, *prev = node;
    while (node != NULL)
    {
        if (node2 != NULL && node->val > node2->val)
        {
            //第一个链表的值大于第二个链表的值,合并链表直到node2大于node1
            if (prev == node)
            {
                //首节点
                l1 = node2;
                while (node2 != NULL && node2->val <= node->val)
                {
                    prev = node2;
                    node2 = node2->next;
                }
                prev->next = node;
            }
            else
            {
                while (node2 != NULL && node2->val <= node->val)
                {
                    prev->next = node2;
                    prev = node2;
                    node2 = node2->next;
                }

                prev->next = node;
            }
        }
        prev = node;
        node = node->next;
    }
    if (node2 != NULL)
    {
        prev->next = node2;
    }
    return l1;
}
/*
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) 
{
    struct ListNode *previous = NULL;
    struct ListNode *t2 = l2;
    struct ListNode *head = l1;

    while (t2)
    {
        if (!l1 || (l1->val > t2->val))
        {
            l2 = l2->next;
            t2->next = l1;
            
            if (previous)
            {
                previous->next = t2;
            }
            else
            {
                head = t2;
            }
            
            t2 = l2;
            l1 = head;
        }
        
        previous = l1;
        l1 = l1->next;
    }
    
    return head;
}
*/

int main()
{
    struct ListNode *node1 = malloc(sizeof(struct ListNode)), *node2 = malloc(sizeof(struct ListNode));
    node1->val = 1;
    node1->next = NULL;
    node2->val = 3;
    node2->next = NULL;
    mergeTwoLists(node1, node2);
    return 0;
}