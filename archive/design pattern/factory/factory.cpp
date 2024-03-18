#include <iostream>
#include <string>
using namespace std;

class Image
{
public:
    virtual void read() = 0;
};

class ImageFactory
{
public:
    virtual Image *create() = 0;
    virtual void read() = 0;
};

class JpgImage : public Image
{
public:
    string filename;

public:
    JpgImage(string filename)
    {
        this->filename = filename;
    }

    void read()
    {
        cout << "读取jpg文件:" << this->filename << endl;
    }
};

class PngImage : public Image
{
public:
    void read()
    {
        cout << "读取png图片" << endl;
    }
};

class JpgImageFactory : public ImageFactory
{
private:
    Image *img;

public:
    Image *create()
    {
        this->img = new JpgImage("file-name");
        return this->img;
    }

    void read()
    {
        img->read();
    }
};

class PngImageFactory : public ImageFactory
{
private:
    Image *img;

public:
    Image *create()
    {
        this->img = new PngImage();
        return this->img;
    }
    void read()
    {
        img->read();
    }
};

int main()
{
    ImageFactory *factory = new JpgImageFactory();
    Image *img = factory->create();
    img->read();
    factory->read();

    factory = new PngImageFactory();
    img = factory->create();
    img->read();
    factory->read();
    return 0;
}
