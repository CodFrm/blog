#include <stdio.h>
#include <stdlib.h>

/**
 * https://leetcode-cn.com/problems/add-two-numbers/description/
 * 两数相加
 */

struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2)
{
    struct ListNode *head = malloc(sizeof(struct ListNode));
    memset(head, 0, sizeof(struct ListNode));
    struct ListNode *next = head;
    int j = 0;
    if (l1->val == 0 && l1->next == NULL)
    {
        l1 = NULL;
    }
    if (l2->val == 0 && l2->next == NULL)
    {
        l2 = NULL;
    }
    do
    {
        int num1 = 0, num2 = 0;
        if (l1 != NULL)
        {
            num1 = l1->val;
            l1 = l1->next;
        }
        if (l2 != NULL)
        {
            num2 = l2->val;
            l2 = l2->next;
        }

        num1 += num2 + j;
        if (num1 >= 10)
        {
            num1 %= 10;
            j = 1;
        }
        else
        {
            j = 0;
        }
        next->val = num1;
        if (l1 == NULL && l2 == NULL)
        {
            break;
        }
        else
        {
            next->next = malloc(sizeof(struct ListNode));
            next = next->next;
            memset(next, 0, sizeof(struct ListNode));
        }
    } while (1);
    if (j == 1)
    {
        next->next = malloc(sizeof(struct ListNode));
        next = next->next;
        memset(next, 0, sizeof(struct ListNode));
        next->val = j;
    }
    return head;
}

struct ListNode *zh(long long num)
{
    struct ListNode *head = malloc(sizeof(struct ListNode));
    memset(head, 0, sizeof(struct ListNode));
    struct ListNode *next = head;
    do
    {
        next->val = num % 10;
        num = num / 10;
        if (num > 0)
        {
            next->next = malloc(sizeof(struct ListNode));
            next = next->next;
            memset(next, 0, sizeof(struct ListNode));
        }
        else
        {
            break;
        }
    } while (1);
    return head;
}
void listNumber(struct ListNode *a)
{
    do
    {
        printf("%d", a->val);
    } while ((a = a->next) != NULL);
    printf("\n");
}

int main()
{
    struct ListNode *a = addTwoNumbers(zh(0), zh(1));
    listNumber(a);
    return 0;
}