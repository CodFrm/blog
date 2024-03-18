#include <iostream>
using namespace std;

class Pic
{
public:
    string download(string url)
    {
        cout << "图片下载中...";
        return url;
    }
};

class PicProxy
{
protected:
    Pic *pic;

public:
    PicProxy()
    {
        pic = new Pic();
    }
    void download(string url)
    {
        cout<<"显示默认图片到列表\n";
        cout<<"开启线程下载图片\n";
        cout<<this->pic->download(url);
    }
};


int main()
{
    PicProxy* p=new PicProxy();
    p->download("img1");
    return 0;
}
