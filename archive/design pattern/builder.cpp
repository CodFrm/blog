#include <iostream>

using namespace std;

class PlayWindow
{
public:
    string menu;
    string list;
    string window;
    string control;
    void print()
    {
        cout << "菜单:" << menu << "\t列表:" << list << "\t窗口:" << window << "\t控制条:" << control << "\n";
    }
};

class UI
{
protected:
    PlayWindow *window;

public:
    UI()
    {
        this->window = new PlayWindow;
    }
    virtual void buildMenu() = 0;
    virtual void buildList() = 0;
    virtual void buildWindow() = 0;
    virtual void buildControl() = 0;
    PlayWindow *create()
    {
        return this->window;
    }
};

class Complete : public UI
{
public:
    Complete() : UI() {}
    void buildMenu()
    {
        this->window->menu = "菜单";
    }
    void buildList()
    {
        this->window->list = "播放菜单";
    }
    void buildWindow()
    {
        this->window->window = "完整模式";
    }
    void buildControl()
    {
        this->window->control = "控制条";
    }
};

class Thin : public UI
{
public:
    Thin() : UI() {}
    void buildMenu()
    {
        this->window->menu = "无";
    }
    void buildList()
    {
        this->window->list = "无";
    }
    void buildWindow()
    {
        this->window->window = "精简窗口";
    }
    void buildControl()
    {
        this->window->control = "精简控制条";
    }
};
// 其它模式窗口
class UIController
{
private:
    UI *ui;

public:
    UIController(UI *ui)
    {
        this->ui = ui;
    }
    PlayWindow *build()
    {
        this->ui->buildControl();
        this->ui->buildWindow();
        this->ui->buildList();
        this->ui->buildMenu();
        return this->ui->create();
    }
};

int main()
{
    UI *ui = new Complete(); //窗口样式
    UIController *uictl = new UIController(ui);
    PlayWindow *window = uictl->build();
    window->print();
    return 0;
}