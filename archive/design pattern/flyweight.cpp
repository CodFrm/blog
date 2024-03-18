#include <iostream>
#include <unordered_map>
using namespace std;

class info
{
public:
    int x;
    int y;
    info(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

class resource
{
public:
    virtual void display(info *) = 0;
};

class img : public resource
{
    string image;

public:
    img(string img)
    {
        this->image = img;
    }
    void display(info *i)
    {
        cout << "图片:" << image << "位置:" << i->x << "," << i->y << endl;
    }
};

class video : public resource
{
    string v;

public:
    video(string video)
    {
        this->v = video;
    }
    void display(info *i)
    {
        cout << "视频:" << v << "位置:" << i->x << "," << i->y << endl;
    }
};

class resourceFactory
{
protected:
    static unordered_map<string, resource *> map;

    static resourceFactory *res;

public:
    static resourceFactory *getResourceFactory()
    {
        return resourceFactory::res;
    }

    resource *getResource(string key)
    {
        if (map.find(key) == map.end())
        {
            return NULL;
        }
        return map[key];
    }

    void add(string key, resource *res)
    {
        map[key] = res;
    }
};

unordered_map<string, resource *> resourceFactory::map;
resourceFactory *resourceFactory::res = new resourceFactory();

int main()
{
    resource *img1 = new img("1"), *img2 = new img("2");
    resource *video1 = new video("1"), *video2 = new video("2");
    resourceFactory *factory = resourceFactory::getResourceFactory();
    factory->add("img1", img1);
    factory->add("img2", img2);
    factory->add("video1", video1);
    factory->add("video2", video2);

    factory->getResource("img1")->display(new info(1, 2));
    factory->getResource("video1")->display(new info(4, 2));

    return 0;
}