#include <iostream>
#include <vector>
using namespace std;

class Component
{
public:
    virtual void click() = 0;
};
class Button : public Component
{
public:
    void click()
    {
        cout << "按钮按下\n";
    }
};
class Text : public Component
{
public:
    void click()
    {
        cout << "文本框按下\n";
    }
};

class Window : public Component
{
protected:
    vector<Component *> vec;

public:
    void add(Component *component)
    {
        vec.push_back(component);
    }
    void remove(Component *component)
    {
        for (int i = 0; i < vec.size(); i++)
        {
            if (vec[i] == component)
            {
                vec.erase(vec.begin() + i);
            }
        }
    }
    void click()
    {
        cout << "窗口按下\n";
        for (int i = 0; i < vec.size(); i++)
        {
            vec[i]->click();
        }
    }
};

class Container : public Window
{
public:
    void click()
    {
        cout << "容器按下\n";
        for (int i = 0; i < vec.size(); i++)
        {
            vec[i]->click();
        }
    }
};

int main()
{
    Window *w = new Window();
    w->add(new Text());
    w->add(new Button());
    Container *c = new Container();
    w->add(c);
    c->add(new Button());
    c->add(new Text());
    c->add(new Button());
    w->click();
    cout << "\n\n";
    c->click();
    return 0;
}
