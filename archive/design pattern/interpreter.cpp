#include <iostream>
using namespace std;

//expression=command';'*
//command=action object 'from' databse 'to' database
//action='copy'|'move'
//object='view'|'table' name
//database=string
class AbstractExpression
{
public:
    virtual string interpert() = 0;
};

class Object : public AbstractExpression
{
    string str;

public:
    Object(string str)
    {
        this->str=str;
    }
    string interpert()
    {
        if (str.substr(0, 4) == "view")
        {
            return "视图";
        }
        else
        {
            return "表" + str.substr(6);
        }
    }
};

class Database : public AbstractExpression
{
    string db;

public:
    Database(string db)
    {
        this->db = db;
    }
    string interpert()
    {
        return db;
    }
};

class Action : public AbstractExpression
{
    string action;

public:
    Action(string action)
    {
        this->action = action;
    }
    string interpert()
    {
        if (action == "copy")
        {
            return "复制";
        }
        else
        {
            return "移动";
        }
    }
};

class Command : public AbstractExpression
{
    AbstractExpression *action;
    AbstractExpression *object;
    AbstractExpression *db1;
    AbstractExpression *db2;

public:
    Command(AbstractExpression *action,
            AbstractExpression *object,
            AbstractExpression *db1,
            AbstractExpression *db2)
    {
        this->action = action;
        this->object = object;
        this->db1 = db1;
        this->db2 = db2;
    }
    string interpert()
    {
        return "从" + db1->interpert() + "中" + action->interpert() + object->interpert() + "到" + db2->interpert();
    }
};

int main()
{
    string cmd = "copy view from db1 to db2;move table a from db1 to db2;";
    AbstractExpression *exp = new Command(new Action("copy"), new Object("view"), new Database("db1"), new Database("db2"));
    cout << exp->interpert() << endl;
    return 0;
}