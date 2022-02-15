#include<iostream>
using namespace std;
class shape{
    public:
        float area();
};
class rectangle:public shape
{
    public:
    float width,length;
        void area()
        {
            cout << width*length;
        }
        rectangle()
        {
            cout << "Please enter length and width" << endl;
            cin >> length >> width;
        }
        rectangle(int x)
        {
            width = length;
        }
};
class square:public rectangle
{
    public:
        square(int x = 0):rectangle(0)
        {
            cout << "Please enter the length of the sides of the square" << endl;
            cin >> length;
            length = width;
        }
};
class circle:public shape
{
    private:
        float radius;
    public:
        circle()
        {
            cout << "Please enter the radius." << endl;
            cin >> radius;
        }
        void area()
        {
            cout << 3.14*radius*radius;
        }
};
int main()
{
    square a;
    circle b;
    a.area();
    b.area();
    return 0;
}