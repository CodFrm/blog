#include <iostream>
using namespace std;

class DB
{
public:
    virtual string data() = 0;
};

class File
{
protected:
    DB *db;

public:
    void setDb(DB *db)
    {
        this->db = db;
    }
    virtual void print() = 0;
};

class Mysql : public DB
{
public:
    string data()
    {
        return "mysql";
    };
};

class Oracle:public DB{
    public:
    string data(){
        return "oracle";
    }
};

class Txt : public File
{
public:
    void print()
    {
        cout << "从" << this->db->data() << "提取数据转换为TXT格式\n";
    }
};
class Pdf:public File{
    public:
    void print()
    {
        cout << "从" << this->db->data() << "提取数据转换为PDF格式\n";
    }
};
int main()
{
    DB *db = new Mysql();//选择数据库
    File *file=new Txt();//选择输出格式
    file->setDb(db);
    file->print();
    
    file=new Pdf();
    db=new Oracle();
    file->setDb(db);
    file->print();
    return 0;
}