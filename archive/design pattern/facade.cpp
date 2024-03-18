#include <iostream>
using namespace std;

class A
{
public:
    void printA()
    {
        cout << "A\n";
    }
};

class B
{
public:
    void printB()
    {
        cout << "B\n";
    }
};

class C
{
public:
    void printC()
    {
        cout << "C\n";
    }
};

class AbstractSystemFacade
{
public:
    virtual void Func() = 0;
};

class ABFacade : public AbstractSystemFacade
{
    A *a;
    B *b;

public:
    ABFacade()
    {
        a = new A();
        b = new B();
    }
    void Func()
    {
        a->printA();
        b->printB();
    }
};
class ACFacade : public AbstractSystemFacade
{
    A *a;
    C *c;

public:
    ACFacade()
    {
        a = new A();
        c = new C();
    }
    void Func()
    {
        a->printA();
        c->printC();
    }
};

int main()
{
    AbstractSystemFacade *f = new ABFacade();
    f->Func();
    f=new ACFacade();
    f->Func();
    return 0;
}