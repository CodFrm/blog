#include <iostream>
using namespace std;

class Encode1
{
public:
    void print()
    {
        cout << "编码1\n";
    }
};

class Encode2
{
public:
    void printf()
    {
        cout << "编码2\n";
    }
};

class Encrypt
{
public:
    virtual void Encode() = 0;
};

class Encode1Adapter : public Encrypt
{
private:
    Encode1 *encode;

public:
    Encode1Adapter(Encode1 *e)
    {
        encode = e;
    }
    void Encode()
    {
        encode->print();
    }
};

class ClassEncode2Adapter : public Encode2, public Encrypt
{
public:
    void Encode()
    {
        this->printf();
    }
};

class User
{
private:
    Encrypt *encrypt;

public:
    User(Encrypt *e)
    {
        this->encrypt = e;
    }
    void Use()
    {
        this->encrypt->Encode();
    }
};

int main()
{
    //对象适配器
    Encrypt *e = new Encode1Adapter(new Encode1());
    User *u = new User(e);
    u->Use();
    //类适配器
    e = new ClassEncode2Adapter();
    u = new User(e);
    u->Use();
    return 0;
}