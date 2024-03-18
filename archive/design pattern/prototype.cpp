#include <iostream>
#include <string>

using namespace std;

class Object
{
public:
    virtual Object *colne() { return NULL; };
    virtual void display() = 0;
};

class Address
{
public:
    string address;
    Address(string addr)
    {
        this->address = addr;
    }
};

class Customer : public Object
{
public:
    Address *address;
    string user;
    Object *clone() // 深拷贝
    {
        Customer *a = new Customer();
        a->user = this->user;
        a->address = new Address(this->address->address);
        return a;
    }
    void display()
    {
        cout << this->user << ":" << this->address->address << endl;
    }
};

int main()
{
    Customer *a = new Customer();
    a->user = "test";
    a->address = new Address("haha");

    Customer *b = (Customer *)a->clone();
    b->user = "test123";
    a->display();
    b->display();

    return 0;
}