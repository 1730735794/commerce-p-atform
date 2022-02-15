#include<iostream>
#include<utility>
#include<math.h>
using namespace std;
class point
{
    public:
    int x;
    int y;
    point()
    {
        x = 0;
        y = 0;
    }
    point(int a, int b)
    {
        x = a;
        y = b;
    }
    double get_distance(point a)
    {
        double cur = sqrt((a.x - x) * (a.x - x) + (a.y - y) * (a.y - y));
        return cur;
    }
    ~point()
    {
        cout << "del point" << endl;
    }
};
class circle
{
    public:
    point center;
    int radius;
    circle()
    {
        cout << "������Բ�ĵ�����" << endl;
        cin >> center.x >> center.y;
        cout << "������Բ�İ뾶" << endl;
        cin >> radius;
    }
    void judge(circle b)
    {
        double dis = radius + b.radius;
        double cur = center.get_distance(b.center);
            if(cur < dis)
                cout << "�ཻ";
            else
                cout << "���ཻ";  
    }
    ~circle()
    {
        cout << "del circle" << endl;
    }
};
int main()
{

    circle a;
    return 0;
}