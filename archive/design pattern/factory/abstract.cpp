#include <iostream>
#include <string>
using namespace std;

class Init
{
public:
    virtual void init() = 0;
};

class ArrowKeys
{
public:
    virtual void arrowKeys() = 0;
};

class SystemFactory
{
public:
    virtual Init *CreateInit() = 0;
    virtual ArrowKeys *CreateArrowKeys() = 0;
};

class AndroidInit : public Init
{
public:
    void init()
    {
        cout << "安卓初始化" << endl;
    }
};

class AndroidArrowKeys : public ArrowKeys
{
public:
    void arrowKeys()
    {
        cout << "安卓方向键" << endl;
    }
};

class AndroidFactory : public SystemFactory
{
public:
    Init *CreateInit()
    {
        return new AndroidInit();
    }
    ArrowKeys *CreateArrowKeys()
    {
        return new AndroidArrowKeys();
    }
};

int main()
{
    SystemFactory *s = new AndroidFactory();
    s->CreateInit()->init();
    s->CreateArrowKeys()->arrowKeys();

    return 0;
}
