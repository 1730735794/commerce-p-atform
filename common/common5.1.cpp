#include<iostream>
using namespace std;
class shape{
    public:
        virtual void area() = 0;
};
class rectangle:public shape
{
    public:
    float width,length;
        virtual void area()
        {
            cout << width*length << endl;
        }
        rectangle(int w, int l)
        {
            width = w;
            length = l;
        }
        rectangle()
        {
            width = length;
        }
};
class square:public rectangle
{
    public:
        square(int w)
        {
            length = w;
            width = length;
        }
};
class circle:public shape
{
    private:
        float radius;
    public:
        circle(float r)
        {
            radius = r;
        }
        void area()
        {
            cout << 3.14*radius*radius << endl;
        }
};
int main()
{
    square a(2);
    rectangle b(1,2);
    circle c(2.5);
    rectangle *a_ptr = &a;
    rectangle *b_ptr = &b;
    a_ptr->area();
    b_ptr->area();
    c.area();
    return 0;
}