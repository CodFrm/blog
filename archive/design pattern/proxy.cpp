#include <iostream>
using namespace std;

class method
{
public:
    void Method()
    {
        cout << "方法Method调用成功" << endl;
    }
};

class methodProxy
{
protected:
    method *m;

public:
    methodProxy()
    {
        m = new method();
    }
    void Method(int uid)
    {
        cout << "用户" << uid << "方法Method()被调用" << time(NULL) << endl;
        if (uid > 10)
        {
            cout << "用户" << uid << "方法Method()调用失败" << time(NULL) << endl;
            return;
        }
        this->m->Method();
        cout << "用户" << uid << "方法Method()调用结束" << time(NULL) << endl;
    }
};

int main()
{
    methodProxy *proxy = new methodProxy();
    proxy->Method(1);
    proxy->Method(11);
    return 0;
}