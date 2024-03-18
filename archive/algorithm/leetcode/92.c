#include <stdio.h>

//https://leetcode-cn.com/problems/reverse-linked-list-ii/
//反转链表 II

struct ListNode
{
  int val;
  struct ListNode *next;
};

// b->a->c->d->e
// c->b->a->d->e

struct ListNode *reverseBetween(struct ListNode *head, int m, int n)
{
  int pos = 1;
  if (head == NULL)
  {
    return NULL;
  }
  if (m == n)
  {
    return head;
  }
  struct ListNode *tmp = malloc(sizeof(struct ListNode));
  tmp->next = head;
  head = tmp;
  while (pos <= n)
  {
    if (pos >= m)
    {
      struct ListNode *head1 = tmp->next, *tail = tmp->next, *next = tmp->next, *tmp2 = NULL;
      while (pos <= n)
      {
        tmp2 = next->next;
        tail->next = tmp2;
        next->next = head1;
        head1 = next;
        next = tmp2;
        pos++;
      }
      tmp->next = head1;
      break;
    }
    tmp = tmp->next;
    pos++;
  }
  return head->next;
}

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

  reverseBetween(head, 2, 4);
  return 0;
}