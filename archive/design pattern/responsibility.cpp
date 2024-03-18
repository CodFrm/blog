#include <iostream>
using namespace std;
class Info
{
public:
    string name;
    int day;
    Info(string name, int day)
    {
        this->name = name;
        this->day = day;
    }
};
class Approval
{
protected:
    string name;
    Approval *next;

public:
    Approval(string name)
    {
        this->name = name;
    }
    void setSuccessor(Approval *next)
    {
        this->next = next;
    }
    virtual void process(Info *info) = 0;
};

class Director : public Approval
{
public:
    Director(string name) : Approval(name) {}
    void process(Info *info)
    {
        if (info->day <= 3)
        {
            cout << this->name << "同意" << info->name << "请假" << info->day << "天" << endl;
        }
        else
        {
            this->next->process(info);
        }
    }
};

class Manager : public Approval
{
public:
    Manager(string name) : Approval(name) {}
    void process(Info *info)
    {
        if (info->day <= 10)
        {
            cout << this->name << "同意" << info->name << "请假" << info->day << "天" << endl;
        }
        else
        {
            this->next->process(info);
        }
    }
};

class Boss : public Approval
{
public:
    Boss(string name) : Approval(name) {}
    void process(Info *info)
    {
        cout << this->name << "同意" << info->name << "请假" << info->day << "天" << endl;
    }
};

int main()
{
    Director *d = new Director("主管A");
    Manager *m = new Manager("经理B");
    Boss *b = new Boss("老板C");
    d->setSuccessor(m);
    m->setSuccessor(b);

    d->process(new Info("员工A", 2));
    d->process(new Info("员工B", 4));
    d->process(new Info("员工C", 15));

    return 0;
}