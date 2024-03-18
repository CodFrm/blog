#include <iostream>
#include <vector>
using namespace std;

class AbstractIterator
{
public:
    virtual void *next() = 0;
};
class PageIterator : public AbstractIterator
{
protected:
    vector<void *> v;
    int pos = 0;

public:
    PageIterator(vector<void *> v)
    {
        this->v = v;
    }
    void *next()
    {
        if (pos >= v.size())
        {
            return NULL;
        }

        return v[pos++];
    }
};
class List
{
protected:
    vector<void *> v;

public:
    void add(void *data)
    {
        v.insert(v.end(), data);
    }
    AbstractIterator *page(int page)
    {
        vector<void *> p(v.begin() + ((page - 1) * 5), v.begin() + (page * 5));
        return new PageIterator(p);
    }
};

int main()
{
    List *list = new List();
    for (int i = 0; i < 8; i++)
    {
        int *tmp = (int *)malloc(sizeof(int));
        *tmp = i;
        list->add(tmp);
    }
    AbstractIterator *iterator = list->page(1);
    void *next;
    cout << "page1:\n";
    while ((next = iterator->next()) != NULL)
    {
        cout << *(int *)next << "\t";
    }
    cout << "\npage2:\n";
    iterator = list->page(2);
    while ((next = iterator->next()) != NULL)
    {
        cout << *(int *)next << "\t";
    }
    return 0;
}