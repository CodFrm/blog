#include <iostream>
#include <vector>
using namespace std;

class Observer
{
public:
    virtual void sell(int) = 0;
};
class Subject
{
protected:
    vector<Observer *> vec;

public:
    void add(Observer *o)
    {
        vec.push_back(o);
    }
    virtual void notify(int) = 0;
};

class DJIA : public Subject
{
public:
    void notify(int price)
    {
        cout << "道琼斯发生了熔断,通知" << endl;
        for (int i = 0; i < vec.size(); i++)
        {
            vec[i]->sell(price);
        }
    }
};

class SSE : public Subject
{
public:
    void notify(int price)
    {
        cout << "上证突破3000点了,通知" << endl;
        for (int i = 0; i < vec.size(); i++)
        {
            vec[i]->sell(price);
        }
    }
};

class Chives : public Observer
{
    string name;

public:
    Chives(string name)
    {
        this->name = name;
    }
    void sell(int price)
    {
        cout << name << ":买就行了,加仓干\n";
    }
};

class Dalao : public Observer
{
public:
    void sell(int price)
    {
        cout << "大佬:分析一下\n";
    };
};

int main()
{
    Subject *a = new DJIA(), *b = new SSE();
    Observer *c1 = new Chives("韭菜1"), *c2 = new Chives("韭菜2"), *dl = new Dalao();
    a->add(c1);
    a->add(dl);
    b->add(c2);

    a->notify(10000);
    b->notify(3000);

    return 0;
}