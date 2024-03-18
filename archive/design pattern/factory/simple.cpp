#include <iostream>
#include <string>

class Graph
{
public:
    virtual void show() = 0;
};

class Triangle : public Graph
{
public:
    void show()
    {
        std::cout << "三角形" << std::endl;
    }
};

class Round : public Graph
{
public:
    void show()
    {
        std::cout << "圆形" << std::endl;
    }
};

class GraphFactory
{
public:
    static Graph *CreateGraph(std::string name)
    {
        if (name == "triangle")
        {
            return new Triangle();
        }
        else if (name == "round")
        {
            return new Round();
        }
        return NULL;
    }
};

int main()
{
    Graph *a = GraphFactory::CreateGraph("triangle");
    a->show();
    a = GraphFactory::CreateGraph("round");
    a->show();
    return 0;
}
