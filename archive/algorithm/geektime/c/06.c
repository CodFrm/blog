#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct node;
typedef struct node Node;

struct node
{
    int val;
    Node *next;
};

void insert(Node *header, int val, int pos)
{
    Node *tmp = header;
    int tmpPos = 0;
    if (pos == -1)
    {
        pos = __INT_MAX__;
    }

    while (tmp->next != NULL && tmpPos < pos)
    {
        tmpPos++;
        tmp = tmp->next;
    }
    Node *i = malloc(sizeof(Node));
    i->next = tmp->next;
    i->val = val;
    tmp->next = i;
}

void delete (Node *header, int pos)
{
    Node *tmp = header;
    int tmpPos = 0;
    while (tmp->next != NULL && tmpPos < pos)
    {
        tmpPos++;
        tmp = tmp->next;
    }
    Node *del = tmp->next;
    if (del == NULL)
    {
        return;
    }
    tmp->next = tmp->next->next;
}

Node *reverse(Node *header)
{
    Node *last = NULL;
    Node *tmp = header;
    while (tmp = tmp->next)
    {
        Node *now = malloc(sizeof(Node));
        memset(now, 0, sizeof(Node));
        now->val = tmp->val;
        now->next = last;
        last = now;
    }
    Node *newHeader = malloc(sizeof(Node));
    memset(newHeader, 0, sizeof(Node));
    newHeader->next = last;
    return newHeader;
}

int isRing(Node *header)
{
    if (header == NULL)
    {
        return 0;
    }
    Node *k = header, *m = header;
    do
    {
        if (k->next == NULL)
        {
            return 0;
        }
        k = k->next;
        if (k->next == NULL)
        {
            return 0;
        }
        k = k->next;
        m = m->next;
    } while (m != k);
    return 1;
}

int main()
{
    Node *header = malloc(sizeof(Node));
    header->val = -1;
    header->next = NULL;

    insert(header, 1, -1);
    insert(header, 2, -1);
    insert(header, 3, -1);
    insert(header, 4, 2);
    insert(header, 5, 0);
    insert(header, 6, 0);

    delete (header, 0);
    Node *tmp = header;
    while (tmp = tmp->next)
    {
        printf("%d\t", tmp->val);
    }

    Node *header2 = malloc(sizeof(Node));
    header2->val = -1;
    header2->next = NULL;
    insert(header2, 10, -1);
    insert(header2, 12, -1);
    delete (header2, 0);
    tmp = header2;
    printf("\nheader2\n");
    while (tmp = tmp->next)
    {
        printf("%d\t", tmp->val);
    }

    Node *r = reverse(header);
    tmp = r;
    printf("\reverse\n");
    while (tmp = tmp->next)
    {
        printf("%d\t", tmp->val);
    }

    printf("\n%d\n", isRing(header));
    header->next->next->next->next->next = header->next->next;
    printf("\n%d\n", isRing(header));

    return 0;
}