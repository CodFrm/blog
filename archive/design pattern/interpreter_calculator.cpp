#include <iostream>
#include <string.h>
#include <stack>
using namespace std;

class AbstractExpression
{
public:
    virtual int value() = 0;
};

class ValueExpression : public AbstractExpression
{
    int val;

public:
    ValueExpression(string val)
    {
        this->val = atoi(val.c_str());
    }
    int value()
    {
        return this->val;
    }
};

class SymbolExpression : public AbstractExpression
{
    AbstractExpression *left, *right;
    string symbol;

public:
    SymbolExpression(string symbol, AbstractExpression *left, AbstractExpression *right)
    {
        this->symbol = symbol;
        this->left = left;
        this->right = right;
    }

    int value()
    {
        // 非加即减
        if (symbol == "+")
        {
            return left->value() + right->value();
        }
        else
        {
            return left->value() - right->value();
        }
    }
};
//expression=value|operator
//value=number
//operator='+'|'-'
int main()
{
    char str[] = "1+4+5+6-7";
    AbstractExpression *exp = new SymbolExpression("+", new ValueExpression("1"), new SymbolExpression("+", new ValueExpression("4"), new SymbolExpression("+", new ValueExpression("5"), new SymbolExpression("-", new ValueExpression("6"), new ValueExpression("7"))))); //反了
    cout << "手动构建:" << exp->value() << endl;
    stack<AbstractExpression *> stack;
    AbstractExpression *expression;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] >= '0')
        {
            string val;
            for (; i < strlen(str); i++)
            {
                if (str[i] < '0')
                {
                    break;
                }
                val += str[i];
            }
            i--;
            stack.push(new ValueExpression(val));
        }
        else
        {
            string symbol;
            symbol += str[i++];
            AbstractExpression *exp = stack.top();
            stack.pop();
            string val;
            for (; i < strlen(str); i++)
            {
                if (str[i] < '0')
                {
                    break;
                }
                val += str[i];
            }
            i--;
            stack.push(new SymbolExpression(symbol, exp, new ValueExpression(val)));
        }
    }
    expression = stack.top();
    cout << "value:" << expression->value() << endl;

    return 0;
}