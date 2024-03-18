#include <iostream>
#include <unordered_map>
using namespace std;

class Operate
{
public:
    void Open()
    {
        cout << "打开" << endl;
    }
    void Create()
    {
        cout << "创建\n";
    }
    void Edit()
    {
        cout << "编辑\n";
    }
};

class Command
{
public:
    virtual void execute() = 0;
};

class OpenCommand : public Command
{
    Operate oper;

public:
    void execute()
    {
        oper.Open();
    }
};

class EditCommand : public Command
{
    Operate oper;

public:
    void execute()
    {
        oper.Edit();
    }
};

class CreateCommand : public Command
{
    Operate oper;

public:
    void execute()
    {
        oper.Create();
    }
};

class MenuItem
{
    Command *command;

public:
    void setCommand(Command *command)
    {
        this->command = command;
    }
    void click()
    {
        command->execute();
    }
};

class Menu
{
    unordered_map<string, MenuItem *> items;

public:
    void add(string name, MenuItem *m)
    {
        items[name] = m;
    }
    void click(string name)
    {
        items[name]->click();
    }
};

int main()
{
    Menu *menu = new Menu();
    MenuItem *open = new MenuItem(), *create = new MenuItem(), *edit = new MenuItem();
    open->setCommand(new OpenCommand());
    menu->add("open", open);
    create->setCommand(new CreateCommand());
    menu->add("create", create);
    edit->setCommand(new EditCommand());
    menu->add("edit", edit);

    menu->click("open");
    menu->click("create");
    menu->click("edit");
    return 0;
}