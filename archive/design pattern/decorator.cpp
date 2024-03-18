#include <iostream>
using namespace std;

class text
{
public:
    virtual void display() = 0;
};

class original : public text
{
public:
    void display()
    {
        cout << "简单加密\n";
    }
};

class encrypt : public text
{
protected:
    text *t;

public:
    encrypt(text *t)
    {
        this->t = t;
    }
    void display()
    {
        t->display();
    }
};

class rsa : public encrypt
{
public:
    rsa(text *t) : encrypt(t)
    {
    }
    void display()
    {
        encrypt::display();
        cout << "rsa加密\n";
    }
};

class bit : public encrypt
{
public:
    bit(text *t) : encrypt(t)
    {
    }
    void display()
    {
        encrypt::display();
        cout << "位加密\n";
    }
};

int main()
{
    text *a, *b, *c;
    a = new original();
    b = new rsa(a);
    c = new bit(b);
    c->display();
    return 0;
}